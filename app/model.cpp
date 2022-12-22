#include "model.hpp"

#include <boost/hof/match.hpp>

namespace xzr::learn::console::model
{
[[nodiscard]] auto update(::xzr::learn::console::model::actions::action act,
                          ::xzr::learn::console::model::states::state state,
                          const ::xzr::learn::data::data& data)
    -> ::xzr::learn::console::model::data
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
