#include "data/data.hpp"
#include <console/model.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::console::view::model
{
[[nodiscard]] auto update(actions::action view_act, data view_data) -> data
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [&](states::books s, actions::select a) -> data {
                if (a.id)
                    return {.view_state = states::book{a.id.value()},
                            .model_data = view_data.model_data};
                return {.view_state = s, .model_data = view_data.model_data};
            },
            [&](states::books s, actions::add) -> data {
                return {.view_state = states::add_book{},
                        .model_data = view_data.model_data};
            },
            [&](states::books s, actions::remove a) -> data {
                if (a.id)
                    return {.view_state = s,
                            .model_data = learn::data::update(
                                view_data.model_data,
                                learn::data::books::actions::remove_book{
                                    .id = a.id.value()})};
                return {.view_state = s, .model_data = view_data.model_data};
            },
            [&](states::books s, actions::exit) -> data {
                return {.view_state = s, .model_data = view_data.model_data};
            },
            [&](states::add_book s, actions::text_input a) -> data {
                return {
                    .view_state = states::books{},
                    .model_data = learn::data::update(
                        view_data.model_data,
                        learn::data::books::actions::add_book{.name = a.txt})};
            },
            [&](states::book s, actions::select a) -> data {
                if (a.id)
                    return {.view_state =
                                states::chapter{.book_id = s.book_id,
                                                .chapter_id = a.id.value()},
                            .model_data = view_data.model_data};
                return {.view_state = s, .model_data = view_data.model_data};
            },
            [&](states::book s, actions::add) -> data {
                return {.view_state = states::add_chapter{.book_id = s.book_id},
                        .model_data = view_data.model_data};
            },
            [&](states::book s, actions::remove a) -> data {
                if (a.id)
                    return {.view_state = s,
                            .model_data = learn::data::update(
                                view_data.model_data,
                                learn::data::books::actions::remove_chapter{
                                    .book_id = s.book_id,
                                    .id = a.id.value()})};
                return {.view_state = s, .model_data = view_data.model_data};
            },
            [&](states::book, actions::quit) -> data {
                return {.view_state = states::books{},
                        .model_data = view_data.model_data};
            },
            [&](states::add_chapter s, actions::text_input a) -> data {
                return {.view_state = states::book{s.book_id},
                        .model_data = learn::data::update(
                            view_data.model_data,
                            learn::data::books::actions::add_chapter{
                                .book_id = s.book_id,
                                .name = a.txt})};
            },
            [&](states::chapter s, actions::add) -> data {
                return {.view_state =
                            states::add_card_front{.book_id = s.book_id,
                                                   .chapter_id = s.chapter_id},
                        .model_data = view_data.model_data};
            },
            [&](states::chapter s, actions::remove a) -> data {
                if (a.id)
                    return {.view_state = s,
                            .model_data = learn::data::update(
                                view_data.model_data,
                                learn::data::books::actions::remove_card{
                                    .book_id = s.book_id,
                                    .chapter_id = s.chapter_id,
                                    .id = a.id.value()})};
                return {.view_state = s, .model_data = view_data.model_data};
            },
            [&](states::chapter s, actions::start_training) -> data {
                return {.view_state = states::training{},
                        .model_data = learn::data::update(
                            view_data.model_data,
                            learn::data::training::actions::start{
                                .cards =
                                    view_data.model_data.the_books.at(s.book_id)
                                        .chapters.at(s.chapter_id)
                                        .cards})};
            },
            [&](states::training s, actions::text_input a) -> data {
                return {.view_state = s,
                        .model_data = learn::data::update(
                            view_data.model_data,
                            learn::data::training::actions::answer{a.txt})};
            },
            [&](states::training, actions::quit) -> data {
                return {.view_state = states::books{},
                        .model_data = view_data.model_data};
            },
            [&](states::chapter s, actions::quit) -> data {
                return data{.view_state = states::book{s.book_id},
                            .model_data = view_data.model_data};
            },
            [&](states::add_card_front s, actions::text_input a) -> data {
                return {.view_state =
                            states::add_card_back{.front_txt = a.txt,
                                                  .book_id = s.book_id,
                                                  .chapter_id = s.chapter_id},
                        .model_data = view_data.model_data};
            },
            [&](states::add_card_back s, actions::text_input a) -> data {
                return {.view_state =
                            states::chapter{.book_id = s.book_id,
                                            .chapter_id = s.chapter_id},
                        .model_data = learn::data::update(
                            view_data.model_data,
                            learn::data::books::actions::add_card{
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id,
                                .front = s.front_txt,
                                .back = a.txt})};
            },
            [&](auto s, auto) -> data {
                return {.view_state = s, .model_data = view_data.model_data};
            }),
        view_data.view_state,
        view_act);
}
}
