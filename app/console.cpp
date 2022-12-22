#include "console.hpp"

#include "commands.hpp"
#include "persist.hpp"
#include "view.hpp"

#include <data/books.hpp>
#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/hof/match.hpp>

#include <algorithm>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using ::xzr::learn::console::view::content;
using ::xzr::learn::console::view::menu;
using ::xzr::learn::console::view::print;
using ::xzr::learn::console::view::println;
using ::xzr::learn::console::view::readln;
namespace
{
namespace console::actions
{
struct select
{
    std::optional<int> id{};
};
struct add
{
};
struct remove
{
    std::optional<int> id{};
};
struct text_input
{
    std::string txt{};
};
struct start_training
{
};
struct quit
{
};
struct exit
{
};
using action =
    std::variant<select, add, remove, text_input, start_training, quit, exit>;
}
}
namespace
{
namespace console::states
{
struct books
{
};
struct add_book
{
};
struct book
{
    int book_id{};
};
struct add_chapter
{
    int book_id{};
};
struct chapter
{
    int book_id{};
    int chapter_id{};
};
struct add_card_front
{
    int book_id{};
    int chapter_id{};
};
struct add_card_back
{
    std::string front_txt{};
    int book_id{};
    int chapter_id{};
};
struct training
{
};
using state = std::variant<books,
                           add_book,
                           book,
                           add_chapter,
                           chapter,
                           add_card_front,
                           add_card_back,
                           training>;
struct data
{
    std::optional<::xzr::learn::data::actions::action> data_act{};
    state console_state{};
};
}
}
namespace
{
namespace console
{
auto str_to_id(const std::string& s) -> std::optional<int>
{
    try
    {
        return std::stoi(s) - 1;
    }
    catch (...)
    {
        return std::nullopt;
    }
}
auto extract_id(const std::string& s)
{
    return str_to_id(s.substr(1));
}
auto intent(states::state s, const std::string& cmd_str) -> actions::action
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [&](states::books) -> actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::select))
                    return actions::select{.id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return actions::remove{.id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::exit))
                    return actions::exit{};
                return actions::text_input{cmd_str};
            },
            [&](states::add_book) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::book) -> actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::select))
                    return actions::select{.id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return actions::remove{.id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return actions::quit{};
                return actions::text_input{cmd_str};
            },
            [&](states::add_chapter) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::chapter) -> actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return actions::remove{.id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::start_training))
                    return actions::start_training{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return actions::quit{};
                return actions::text_input{cmd_str};
            },
            [&](states::add_card_front) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::add_card_back) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::training) -> actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return actions::quit{};
                return actions::text_input{cmd_str};
            },
            [&](auto) -> actions::action {
                return actions::text_input{cmd_str};
            }),
        s);
}
auto draw(const ::xzr::learn::data::data& data, states::state s)
{
    using ::boost::hof::match;
    std::visit(
        match(
            [&](states::books) {
                content{}.books(data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](states::book s) {
                content{}.book(data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](states::chapter s) {
                content{}.chapter(
                    data.the_books.at(s.book_id).chapters.at(s.chapter_id));
                menu{"card"}.add().remove().start_training().quit();
            },
            [](states::add_card_front) {
                println("add chard");
                println("front: ");
            },
            [](states::add_card_back) { println("back: "); },
            [&](states::training) {
                std::visit(
                    match(
                        [](xzr::learn::data::training::states::show_card s) {
                            println(s.card.front);
                        },
                        [](xzr::learn::data::training::states::done) {
                            menu{"training"}.quit();
                        }),
                    data.the_training.state);
            },
            [](auto) {}),
        s);
}
auto dispatch(actions::action act,
              states::state state,
              const ::xzr::learn::data::data& data)
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [](states::books s, actions::select a) -> states::data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .console_state = states::book{a.id.value()}};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::books s, actions::add) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state = states::add_book{}};
            },
            [](states::books s, actions::remove a) -> states::data {
                if (a.id)
                    return {.data_act =
                                ::xzr::learn::data::books::actions::remove_book{
                                    .id = a.id.value()},
                            .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::books s, actions::exit) -> states::data {
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::add_book s, actions::text_input a) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_book{
                                .name = a.txt},
                        .console_state = states::books{}};
            },
            [](states::book s, actions::select a) -> states::data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .console_state =
                                states::chapter{.book_id = s.book_id,
                                                .chapter_id = a.id.value()}};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::book s, actions::add) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            states::add_chapter{.book_id = s.book_id}};
            },
            [](states::book s, actions::remove a) -> states::data {
                if (a.id)
                    return {
                        .data_act =
                            ::xzr::learn::data::books::actions::remove_chapter{
                                .book_id = s.book_id,
                                .id = a.id.value()},
                        .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::book, actions::quit) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state = states::books{}};
            },
            [](states::add_chapter s, actions::text_input a) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_chapter{
                                .book_id = s.book_id,
                                .name = a.txt},
                        .console_state = states::book{s.book_id}};
            },
            [](states::chapter s, actions::add) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            states::add_card_front{.book_id = s.book_id,
                                                   .chapter_id = s.chapter_id}};
            },
            [](states::chapter s, actions::remove a) -> states::data {
                if (a.id)
                    return {.data_act =
                                ::xzr::learn::data::books::actions::remove_card{
                                    .book_id = s.book_id,
                                    .chapter_id = s.chapter_id,
                                    .id = a.id.value()},
                            .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [&](states::chapter s, actions::start_training) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::training::actions::start{
                                .cards = data.the_books.at(s.book_id)
                                             .chapters.at(s.chapter_id)
                                             .cards},
                        .console_state = states::training{}};
            },
            [](states::training s, actions::text_input a) -> states::data {
                return {
                    .data_act =
                        ::xzr::learn::data::training::actions::answer{a.txt},
                    .console_state = s};
            },
            [](states::training, actions::quit a) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state = states::books{}};
            },
            [](states::chapter s, actions::quit) -> states::data {
                return states::data{.data_act = std::nullopt,
                                    .console_state = states::book{s.book_id}};
            },
            [](states::add_card_front s,
               actions::text_input a) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            states::add_card_back{.front_txt = a.txt,
                                                  .book_id = s.book_id,
                                                  .chapter_id = s.chapter_id}};
            },
            [](states::add_card_back s, actions::text_input a) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_card{
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id,
                                .front = s.front_txt,
                                .back = a.txt},
                        .console_state =
                            states::chapter{.book_id = s.book_id,
                                            .chapter_id = s.chapter_id}};
            },
            [](auto s, auto) -> states::data {
                return {.data_act = std::nullopt, .console_state = s};
            }),
        state,
        act);
}
}
}
namespace xzr::learn::console
{
auto run() -> void
{
    auto data{::xzr::learn::persist::load_or_create_empty_data()};
    auto console_data{
        ::console::states::data{.data_act = std::nullopt,
                                .console_state = ::console::states::books{}}};
    for (;;)
    {
        ::console::draw(data, console_data.console_state);
        const auto act{::console::intent(console_data.console_state, readln())};
        console_data =
            ::console::dispatch(act, console_data.console_state, data);
        if (const auto data_act{console_data.data_act})
        {
            data = data::update(std::move(data), data_act.value());
            ::xzr::learn::persist::save(data);
        }
        if (std::holds_alternative<::console::actions::exit>(act))
            break;
    }
}
}
