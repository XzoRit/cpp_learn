#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/update.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/hof/match.hpp>

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using data_action = xzr::learn::data::action;
using data_add_book = xzr::learn::data::add_book;
using data_quit = xzr::learn::data::quit;
using app = ::xzr::learn::data::app;
using books = ::xzr::learn::data::books;

namespace fs = std::filesystem;

using ::xzr::learn::console::println;

namespace
{
namespace console
{
struct add_book
{
};
struct quit
{
};
using action = std::variant<add_book, quit>;

struct update_result
{
    app data;
    std::optional<data_action> data_act;
};
}
namespace persist
{
const auto books_path{fs::path{"xzr_learn_books.txt"}};
auto save(const app& app_data) -> void
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};

    save(o, app_data);
}
auto load() -> app
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};

    return load(i);
}
[[nodiscard]] auto read_or_create_app_data() -> app
{
    if (!fs::exists(books_path))
        save(app{});

    return load();
}
}
auto readln() -> std::string
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
auto show_add_book() -> data_add_book
{
    println("add book");
    println();
    println("name: ");

    return data_add_book{.name = readln()};
}
auto intent(std::string_view cmd) -> std::optional<console::action>
{
    static const std::map<std::string_view, console::action> cmd_actions{
        {{"b", console::add_book{}}, {"d", console::quit{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
auto update_console(app app_data, console::action act) -> console::update_result
{
    return std::visit(
        boost::hof::match(
            [&](console::add_book) {
                return console::update_result{.data = app_data,
                                              .data_act = show_add_book()};
            },
            [&](console::quit) {
                return console::update_result{.data = app_data,
                                              .data_act = std::nullopt};
            }),
        act);
}
auto update(app app_data, console::action act) -> app
{
    using ::xzr::learn::data::update;

    const auto [data, data_act]{update_console(std::move(app_data), act)};
    if (data_act)
        return update(std::move(data), data_act.value());
    return data;
}
auto render_books(const books& bs) -> void
{
    for (int i{}; const auto& b : bs)
        println(++i, ".\t", b.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");
}
auto render(const app& app_data) -> void
{
    render_books(app_data.the_books);
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
        ::render(app_data);
        const auto cmd{::readln()};
        if (const auto console_act{::intent(cmd)})
        {
            app_data = ::update(std::move(app_data), console_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<::console::quit>(console_act.value()))
                return;
        }
    }
}
}
}
