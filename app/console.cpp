#include "console.hpp"

#include "commands.hpp"
#include "model.hpp"
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
auto intent(::xzr::learn::console::model::states::state s,
            const std::string& cmd_str)
    -> ::xzr::learn::console::model::actions::action
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [&](::xzr::learn::console::model::states::books)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::select))
                    return ::xzr::learn::console::model::actions::select{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return ::xzr::learn::console::model::actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return ::xzr::learn::console::model::actions::remove{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::exit))
                    return ::xzr::learn::console::model::actions::exit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_book)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::book)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::select))
                    return ::xzr::learn::console::model::actions::select{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return ::xzr::learn::console::model::actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return ::xzr::learn::console::model::actions::remove{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return ::xzr::learn::console::model::actions::quit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_chapter)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::chapter)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return ::xzr::learn::console::model::actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return ::xzr::learn::console::model::actions::remove{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::start_training))
                    return ::xzr::learn::console::model::actions::
                        start_training{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return ::xzr::learn::console::model::actions::quit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_card_front)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_card_back)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::training)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return ::xzr::learn::console::model::actions::quit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](auto) -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            }),
        s);
}
auto draw(const ::xzr::learn::data::data& data,
          ::xzr::learn::console::model::states::state s)
{
    using ::boost::hof::match;
    std::visit(
        match(
            [&](::xzr::learn::console::model::states::books) {
                content{}.books(data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](::xzr::learn::console::model::states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](::xzr::learn::console::model::states::book s) {
                content{}.book(data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](::xzr::learn::console::model::states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](::xzr::learn::console::model::states::chapter s) {
                content{}.chapter(
                    data.the_books.at(s.book_id).chapters.at(s.chapter_id));
                menu{"card"}.add().remove().start_training().quit();
            },
            [](::xzr::learn::console::model::states::add_card_front) {
                println("add chard");
                println("front: ");
            },
            [](::xzr::learn::console::model::states::add_card_back) {
                println("back: ");
            },
            [&](::xzr::learn::console::model::states::training) {
                std::visit(
                    match(
                        [](::xzr::learn::data::training::states::show_card s) {
                            println(s.card.front);
                        },
                        [](::xzr::learn::data::training::states::done) {
                            menu{"training"}.quit();
                        }),
                    data.the_training.state);
            },
            [](auto) {}),
        s);
}
auto dispatch(::xzr::learn::console::model::actions::action act,
              ::xzr::learn::console::model::states::state state,
              const ::xzr::learn::data::data& data)
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [](::xzr::learn::console::model::states::books s,
               ::xzr::learn::console::model::actions::select a)
                -> ::xzr::learn::console::model::data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .console_state =
                                ::xzr::learn::console::model::states::book{
                                    a.id.value()}};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](::xzr::learn::console::model::states::books s,
               ::xzr::learn::console::model::actions::add)
                -> ::xzr::learn::console::model::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            ::xzr::learn::console::model::states::add_book{}};
            },
            [](::xzr::learn::console::model::states::books s,
               ::xzr::learn::console::model::actions::remove a)
                -> ::xzr::learn::console::model::data {
                if (a.id)
                    return {.data_act =
                                ::xzr::learn::data::books::actions::remove_book{
                                    .id = a.id.value()},
                            .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](::xzr::learn::console::model::states::books s,
               ::xzr::learn::console::model::actions::exit)
                -> ::xzr::learn::console::model::data {
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](::xzr::learn::console::model::states::add_book s,
               ::xzr::learn::console::model::actions::text_input a)
                -> ::xzr::learn::console::model::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_book{
                                .name = a.txt},
                        .console_state =
                            ::xzr::learn::console::model::states::books{}};
            },
            [](::xzr::learn::console::model::states::book s,
               ::xzr::learn::console::model::actions::select a)
                -> ::xzr::learn::console::model::data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .console_state =
                                ::xzr::learn::console::model::states::chapter{
                                    .book_id = s.book_id,
                                    .chapter_id = a.id.value()}};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](::xzr::learn::console::model::states::book s,
               ::xzr::learn::console::model::actions::add)
                -> ::xzr::learn::console::model::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            ::xzr::learn::console::model::states::add_chapter{
                                .book_id = s.book_id}};
            },
            [](::xzr::learn::console::model::states::book s,
               ::xzr::learn::console::model::actions::remove a)
                -> ::xzr::learn::console::model::data {
                if (a.id)
                    return {
                        .data_act =
                            ::xzr::learn::data::books::actions::remove_chapter{
                                .book_id = s.book_id,
                                .id = a.id.value()},
                        .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](::xzr::learn::console::model::states::book,
               ::xzr::learn::console::model::actions::quit)
                -> ::xzr::learn::console::model::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            ::xzr::learn::console::model::states::books{}};
            },
            [](::xzr::learn::console::model::states::add_chapter s,
               ::xzr::learn::console::model::actions::text_input a)
                -> ::xzr::learn::console::model::data {
                return {
                    .data_act =
                        ::xzr::learn::data::books::actions::add_chapter{
                            .book_id = s.book_id,
                            .name = a.txt},
                    .console_state =
                        ::xzr::learn::console::model::states::book{s.book_id}};
            },
            [](::xzr::learn::console::model::states::chapter s,
               ::xzr::learn::console::model::actions::add)
                -> ::xzr::learn::console::model::data {
                return {
                    .data_act = std::nullopt,
                    .console_state =
                        ::xzr::learn::console::model::states::add_card_front{
                            .book_id = s.book_id,
                            .chapter_id = s.chapter_id}};
            },
            [](::xzr::learn::console::model::states::chapter s,
               ::xzr::learn::console::model::actions::remove a)
                -> ::xzr::learn::console::model::data {
                if (a.id)
                    return {.data_act =
                                ::xzr::learn::data::books::actions::remove_card{
                                    .book_id = s.book_id,
                                    .chapter_id = s.chapter_id,
                                    .id = a.id.value()},
                            .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [&](::xzr::learn::console::model::states::chapter s,
                ::xzr::learn::console::model::actions::start_training)
                -> ::xzr::learn::console::model::data {
                return {.data_act =
                            ::xzr::learn::data::training::actions::start{
                                .cards = data.the_books.at(s.book_id)
                                             .chapters.at(s.chapter_id)
                                             .cards},
                        .console_state =
                            ::xzr::learn::console::model::states::training{}};
            },
            [](::xzr::learn::console::model::states::training s,
               ::xzr::learn::console::model::actions::text_input a)
                -> ::xzr::learn::console::model::data {
                return {
                    .data_act =
                        ::xzr::learn::data::training::actions::answer{a.txt},
                    .console_state = s};
            },
            [](::xzr::learn::console::model::states::training,
               ::xzr::learn::console::model::actions::quit a)
                -> ::xzr::learn::console::model::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            ::xzr::learn::console::model::states::books{}};
            },
            [](::xzr::learn::console::model::states::chapter s,
               ::xzr::learn::console::model::actions::quit)
                -> ::xzr::learn::console::model::data {
                return ::xzr::learn::console::model::data{
                    .data_act = std::nullopt,
                    .console_state =
                        ::xzr::learn::console::model::states::book{s.book_id}};
            },
            [](::xzr::learn::console::model::states::add_card_front s,
               ::xzr::learn::console::model::actions::text_input a)
                -> ::xzr::learn::console::model::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            ::xzr::learn::console::model::states::add_card_back{
                                .front_txt = a.txt,
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id}};
            },
            [](::xzr::learn::console::model::states::add_card_back s,
               ::xzr::learn::console::model::actions::text_input a)
                -> ::xzr::learn::console::model::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_card{
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id,
                                .front = s.front_txt,
                                .back = a.txt},
                        .console_state =
                            ::xzr::learn::console::model::states::chapter{
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id}};
            },
            [](auto s, auto) -> ::xzr::learn::console::model::data {
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
    auto console_data{::xzr::learn::console::model::data{
        .data_act = std::nullopt,
        .console_state = ::xzr::learn::console::model::states::books{}}};
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
        if (std::holds_alternative<::xzr::learn::console::model::actions::exit>(
                act))
            break;
    }
}
}
