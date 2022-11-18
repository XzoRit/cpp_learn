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
auto save(const ::xzr::learn::data::app& app_data);
[[nodiscard]] auto load();
[[nodiscard]] auto read_or_create_app_data();
}
namespace console
{
namespace action
{
struct add_book
{
};
struct quit
{
};
using action = std::variant<add_book, quit>;
}
namespace dialog
{
[[nodiscard]] auto add_book();
[[nodiscard]] auto update(const ::xzr::learn::data::app&, action::action act);
}
[[nodiscard]] auto readln();
[[nodiscard]] auto intent(std::string_view cmd)
    -> std::optional<action::action>;
[[nodiscard]] auto update(::xzr::learn::data::app app_data,
                          ::xzr::learn::data::action act);
namespace render
{
auto app(const ::xzr::learn::data::app& app_data);
auto books(const ::xzr::learn::data::books& bs);
}
}
}
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
namespace console
{
[[nodiscard]] auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
namespace dialog
{
[[nodiscard]] auto add_book()
{
    println("add book");
    println("name: ");
    return ::xzr::learn::data::add_book{.name = readln()};
}
[[nodiscard]] auto update(const ::xzr::learn::data::app&, action::action act)
{
    using ::boost::hof::match;
    using ::boost::hof::result;

    return std::visit(result<std::optional<::xzr::learn::data::action>>(
                          match([&](action::add_book) { return add_book(); },
                                [&](action::quit) { return std::nullopt; })),
                      act);
}
}
[[nodiscard]] auto intent(std::string_view cmd) -> std::optional<action::action>
{
    static const std::map<std::string_view, action::action> cmd_actions{
        {{"b", action::add_book{}}, {"d", action::quit{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
[[nodiscard]] auto update(::xzr::learn::data::app app_data, action::action act)
{
    using ::xzr::learn::data::update;

    if (const auto data_act{dialog::update(app_data, act)})
        return update(std::move(app_data), data_act.value());
    return app_data;
}
namespace render
{
auto books(const ::xzr::learn::data::books& bs)
{
    for (int i{}; const auto& b : bs)
        println(++i, ".\t", b.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");
}
auto app(const ::xzr::learn::data::app& app_data)
{
    books(app_data.the_books);
}
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
    for (;;)
    {
        ::console::render::app(app_data);
        if (const auto console_act{::console::intent(::console::readln())})
        {
            app_data =
                ::console::update(std::move(app_data), console_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<::console::action::quit>(
                    console_act.value()))
                return;
        }
    }
}
}
}
