#include "model.hpp"

#include <boost/hof/match.hpp>

namespace xzr::learn::console::view::model
{
[[nodiscard]] auto update(actions::action act,
                          states::state state,
                          const learn::data::data& model_data) -> data
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [](states::books s, actions::select a) -> data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .view_state = states::book{a.id.value()}};
                return {.data_act = std::nullopt, .view_state = s};
            },
            [](states::books s, actions::add) -> data {
                return {.data_act = std::nullopt,
                        .view_state = states::add_book{}};
            },
            [](states::books s, actions::remove a) -> data {
                if (a.id)
                    return {.data_act =
                                learn::data::books::actions::remove_book{
                                    .id = a.id.value()},
                            .view_state = s};
                return {.data_act = std::nullopt, .view_state = s};
            },
            [](states::books s, actions::exit) -> data {
                return {.data_act = std::nullopt, .view_state = s};
            },
            [](states::add_book s, actions::text_input a) -> data {
                return {
                    .data_act =
                        learn::data::books::actions::add_book{.name = a.txt},
                    .view_state = states::books{}};
            },
            [](states::book s, actions::select a) -> data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .view_state =
                                states::chapter{.book_id = s.book_id,
                                                .chapter_id = a.id.value()}};
                return {.data_act = std::nullopt, .view_state = s};
            },
            [](states::book s, actions::add) -> data {
                return {.data_act = std::nullopt,
                        .view_state =
                            states::add_chapter{.book_id = s.book_id}};
            },
            [](states::book s, actions::remove a) -> data {
                if (a.id)
                    return {.data_act =
                                learn::data::books::actions::remove_chapter{
                                    .book_id = s.book_id,
                                    .id = a.id.value()},
                            .view_state = s};
                return {.data_act = std::nullopt, .view_state = s};
            },
            [](states::book, actions::quit) -> data {
                return {.data_act = std::nullopt,
                        .view_state = states::books{}};
            },
            [](states::add_chapter s, actions::text_input a) -> data {
                return {.data_act =
                            learn::data::books::actions::add_chapter{
                                .book_id = s.book_id,
                                .name = a.txt},
                        .view_state = states::book{s.book_id}};
            },
            [](states::chapter s, actions::add) -> data {
                return {.data_act = std::nullopt,
                        .view_state =
                            states::add_card_front{.book_id = s.book_id,
                                                   .chapter_id = s.chapter_id}};
            },
            [](states::chapter s, actions::remove a) -> data {
                if (a.id)
                    return {.data_act =
                                learn::data::books::actions::remove_card{
                                    .book_id = s.book_id,
                                    .chapter_id = s.chapter_id,
                                    .id = a.id.value()},
                            .view_state = s};
                return {.data_act = std::nullopt, .view_state = s};
            },
            [&](states::chapter s, actions::start_training) -> data {
                return {.data_act =
                            learn::data::training::actions::start{
                                .cards = model_data.the_books.at(s.book_id)
                                             .chapters.at(s.chapter_id)
                                             .cards},
                        .view_state = states::training{}};
            },
            [](states::training s, actions::text_input a) -> data {
                return {.data_act =
                            learn::data::training::actions::answer{a.txt},
                        .view_state = s};
            },
            [](states::training, actions::quit a) -> data {
                return {.data_act = std::nullopt,
                        .view_state = states::books{}};
            },
            [](states::chapter s, actions::quit) -> data {
                return data{.data_act = std::nullopt,
                            .view_state = states::book{s.book_id}};
            },
            [](states::add_card_front s, actions::text_input a) -> data {
                return {.data_act = std::nullopt,
                        .view_state =
                            states::add_card_back{.front_txt = a.txt,
                                                  .book_id = s.book_id,
                                                  .chapter_id = s.chapter_id}};
            },
            [](states::add_card_back s, actions::text_input a) -> data {
                return {.data_act =
                            learn::data::books::actions::add_card{
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id,
                                .front = s.front_txt,
                                .back = a.txt},
                        .view_state =
                            states::chapter{.book_id = s.book_id,
                                            .chapter_id = s.chapter_id}};
            },
            [](auto s, auto) -> data {
                return {.data_act = std::nullopt, .view_state = s};
            }),
        state,
        act);
}
}
