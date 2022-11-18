#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>
#include <data/update.hpp>

#include <boost/hof/match.hpp>

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
auto update(app app_data, console::action act) -> console::update_result
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
auto run(app app_data) -> void
{
    auto data_and_action{::console::update_result{.data = std::move(app_data),
                                                  .data_act = std::nullopt}};
    for (;;)
    {
        ::render(data_and_action.data);
        const auto cmd{::readln()};
        if (const auto console_act{::intent(cmd)})
        {
            data_and_action =
                ::update(std::move(data_and_action.data), console_act.value());
            if (const auto data_act{data_and_action.data_act})
                data_and_action.data = ::xzr::learn::update::update(
                    std::move(data_and_action.data),
                    data_act.value());
            if (std::holds_alternative<::console::quit>(console_act.value()))
                return;
        }
    }
}
}
}
