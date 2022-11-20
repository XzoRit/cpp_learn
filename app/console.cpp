#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/update.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/hof/match.hpp>
#include <boost/hof/result.hpp>

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using ::xzr::learn::console::println;

namespace
{
namespace persist
{
namespace fs = std::filesystem;

const auto books_path{fs::path{"xzr_learn_books.txt"}};

auto save(const ::xzr::learn::data::app& app_data)
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};
    save(o, app_data);
}
[[nodiscard]] auto load()
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};
    return load(i);
}
[[nodiscard]] auto read_or_create_app_data()
{
    if (!fs::exists(books_path))
        save(::xzr::learn::data::app{});
    return load();
}
}
}

namespace
{
namespace console::action
{
struct add_book
{
};
struct quit
{
};
using action = std::variant<add_book, quit>;
}
}

namespace
{
namespace console::state
{
struct books
{
};
struct add_book
{
};
struct quit
{
};
using state = std::variant<books, add_book, quit>;
}
}

namespace
{
namespace console::program
{
struct data
{
    ::xzr::learn::data::app app_data;
    ::console::state::state view_state;
};

using action =
    std::variant<::xzr::learn::data::action, ::console::action::action>;
}
}

namespace
{
namespace console
{
[[nodiscard]] auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
[[nodiscard]] auto update(::console::state::state view_state,
                          ::console::action::action console_act)
{
    using ::boost::hof::match;
    using ::boost::hof::result;

    return std::visit(
        result<::console::state::state>(match(
            [&](::console::action::add_book) {
                return ::console::state::add_book{};
            },
            [&](::console::action::quit) { return ::console::state::quit{}; })),
        console_act);
}
[[nodiscard]] auto update(::console::state::state view_state,
                          ::xzr::learn::data::action act)
{
    using ::boost::hof::match;
    using ::boost::hof::result;

    return std::visit(result<::console::state::state>(match(
                          [&](::console::state::books) { return view_state; },
                          [&](::console::state::add_book) {
                              return ::console::state::books{};
                          },
                          [&](::console::state::quit) { return view_state; })),
                      view_state);
}
[[nodiscard]] auto intent(std::string_view cmd)
    -> std::optional<::console::action::action>
{
    static const std::map<std::string_view, ::console::action::action>
        cmd_actions{{{"b", ::console::action::add_book{}},
                     {"d", ::console::action::quit{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
}
}

namespace
{
namespace console::render
{
[[nodiscard]] auto books(const ::xzr::learn::data::books& bs)
{
    for (int i{}; const auto& b : bs)
        println(++i, ".\t", b.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");
    return std::nullopt;
}
[[nodiscard]] auto add_book()
{
    println("add book");
    println("name: ");
    return ::xzr::learn::data::add_book{.name = readln()};
}
[[nodiscard]] auto console(const ::xzr::learn::data::app& app_data,
                           const ::console::state::state& console_state)
{
    using ::boost::hof::match;
    using ::boost::hof::result;

    return std::visit(result<std::optional<::console::program::action>>(match(
                          [&](::console::state::books) {
                              return ::console::render::books(
                                  app_data.the_books);
                          },
                          [](::console::state::add_book) {
                              return ::console::render::add_book();
                          },
                          [](::console::state::quit) { return std::nullopt; })),
                      console_state);
}
[[nodiscard]] auto console(const ::console::program::data& data)
{
    return console(data.app_data, data.view_state);
}
}
}

namespace xzr::learn::console
{
inline namespace v1
{
auto run() -> int
{
    auto prgrm{::console::program::data{
        .app_data = ::persist::read_or_create_app_data(),
        .view_state = ::console::state::books{}}};
    if (::console::render::console(prgrm))
        return 1;

    for (;;)
    {
        if (const auto console_act{::console::intent(::console::readln())})
        {
            prgrm.view_state = ::console::update(std::move(prgrm.view_state),
                                                 console_act.value());
            if (const auto prgrm_act{::console::render::console(prgrm)})
            {
                std::visit(::boost::hof::match(
                               [&](::xzr::learn::data::action act) {
                                   prgrm.app_data = ::xzr::learn::data::update(
                                       std::move(prgrm.app_data),
                                       act);
                                   prgrm.view_state = ::console::update(
                                       std::move(prgrm.view_state),
                                       act);
                               },
                               [&](::console::action::action act) {
                                   prgrm.view_state = ::console::update(
                                       std::move(prgrm.view_state),
                                       act);
                               }),
                           prgrm_act.value());
            }
            ::persist::save(prgrm.app_data);
            if (::console::render::console(prgrm))
                return 2;
            if (std::holds_alternative<::console::action::quit>(
                    console_act.value()))
                return 0;
        }
        else
        {
            println("sorry, unknown command");
            println();
        }
    }
}
}
}
