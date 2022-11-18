#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>

#include <boost/hof/match.hpp>

#include <map>
#include <optional>
#include <string>
#include <string_view>
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
}
auto readln() -> std::string
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
auto intent(std::string_view cmd) -> std::optional<console::action>
{
    static const std::map<std::string_view, console::action> cmd_actions{
        {{"a", console::add_book{}}, {"d", console::quit{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
auto update(app app_data, console::action act) -> app
{
    std::visit(
        boost::hof::match([&](console::add_book) {}, [&](console::quit) {}),
        act);

    return app_data;
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
    ::render(app_data);
    for (;;)
    {
        const auto cmd{::readln()};
        if (const auto act{::intent(cmd)})
        {
            app_data = ::update(std::move(app_data), act.value());
            ::render(app_data);
            if (std::holds_alternative<::console::quit>(act.value()))
                return;
        }
    }
}
}
}
