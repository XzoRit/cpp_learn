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
namespace console
{
[[nodiscard]] auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
[[nodiscard]] auto update(::console::state::state,
                          ::console::action::action act)
{
    using ::boost::hof::match;
    using ::boost::hof::result;

    return std::visit(
        result<::console::state::state>(match(
            [&](::console::action::add_book) {
                return ::console::state::add_book{};
            },
            [&](::console::action::quit) { return ::console::state::quit{}; })),
        act);
}
[[nodiscard]] auto intent(std::string_view cmd) -> std::optional<action::action>
{
    static const std::map<std::string_view, action::action> cmd_actions{
        {{"b", action::add_book{}}, {"d", action::quit{}}}};

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

    return std::visit(result<std::optional<::xzr::learn::data::action>>(match(
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
}
}

namespace xzr::learn::console
{
inline namespace v1
{
auto run() -> void
{
    auto app_data{::persist::read_or_create_app_data()};
    auto console_state{::console::state::state{::console::state::books{}}};
    static_cast<void>(::console::render::console(app_data, console_state));
    for (;;)
    {
        if (const auto console_act{::console::intent(::console::readln())})
        {
            console_state = ::console::update(std::move(console_state),
                                              console_act.value());
            if (const auto data_act{
                    ::console::render::console(app_data, console_state)})
                app_data = ::xzr::learn::data::update(std::move(app_data),
                                                      data_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<::console::action::quit>(
                    console_act.value()))
                return;
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
