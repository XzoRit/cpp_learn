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

using action = ::xzr::learn::action::action;
using add_book = xzr::learn::action::add_book;
using add_chapter = xzr::learn::action::add_chapter;
using add_card = xzr::learn::action::add_card;
using start_training = xzr::learn::action::start_training;
using quit = xzr::learn::action::quit;
using app = ::xzr::learn::data::app;
using books = ::xzr::learn::data::books;

using ::xzr::learn::console::println;

namespace
{
auto readln() -> std::string
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
auto intent(std::string_view cmd) -> std::optional<action>
{
    static const std::map<std::string_view, action> cmd_actions{
        {{"a", add_book{}},
         {"b", add_chapter{}},
         {"c", add_card{}},
         {"d", quit{}},
         {"z", start_training{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
auto update(app app_data, action act) -> app
{
    std::visit(boost::hof::match([&](add_book) {},
                                 [&](add_chapter) {},
                                 [&](add_card) {},
                                 [&](start_training) {},
                                 [&](quit) {}),
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
            if (std::holds_alternative<quit>(act.value()))
                return;
        }
    }
}
}
}
