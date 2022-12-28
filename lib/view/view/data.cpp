#include <view/data.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::view
{
struct on_books
{
    auto operator()(actions::select a) const -> data
    {
        if (a.id)
            return {.view_state = states::book{a.id.value()},
                    .model_data = data};
        return {.view_state = cur_state, .model_data = data};
    }
    auto operator()(actions::add) const -> data
    {
        return {.view_state = states::add_book{}, .model_data = data};
    }
    auto operator()(actions::remove a) const -> data
    {
        if (a.id)
            return {.view_state = cur_state,
                    .model_data =
                        update(data,
                               learn::data::books::actions::remove_book{
                                   .id = a.id.value()})};
        return {.view_state = cur_state, .model_data = data};
    }
    auto operator()(actions::exit) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::books cur_state{};
    learn::data::data data{};
};
struct on_add_book
{
    auto operator()(actions::text_input a) const -> data
    {
        return {.view_state = states::books{},
                .model_data = learn::data::update(
                    data,
                    learn::data::books::actions::add_book{.name = a.txt})};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::add_book cur_state{};
    learn::data::data data{};
};
struct on_book
{
    auto operator()(actions::select a) const -> data
    {
        if (a.id)
            return {.view_state = states::chapter{.book_id = cur_state.book_id,
                                                  .chapter_id = a.id.value()},
                    .model_data = data};
        return {.view_state = cur_state, .model_data = data};
    }
    auto operator()(actions::add) const -> data
    {
        return {.view_state = states::add_chapter{.book_id = cur_state.book_id},
                .model_data = data};
    }
    auto operator()(actions::remove a) const -> data
    {
        if (a.id)
            return {.view_state = cur_state,
                    .model_data = learn::data::update(
                        data,
                        learn::data::books::actions::remove_chapter{
                            .book_id = cur_state.book_id,
                            .id = a.id.value()})};
        return {.view_state = cur_state, .model_data = data};
    }
    auto operator()(actions::quit) const -> data
    {
        return {.view_state = states::books{}, .model_data = data};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::book cur_state{};
    learn::data::data data{};
};
struct on_add_chapter
{
    auto operator()(actions::text_input a) const -> data
    {
        return {.view_state = states::book{cur_state.book_id},
                .model_data = learn::data::update(
                    data,
                    learn::data::books::actions::add_chapter{
                        .book_id = cur_state.book_id,
                        .name = a.txt})};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::add_chapter cur_state{};
    learn::data::data data{};
};
struct on_chapter
{
    auto operator()(actions::add) const -> data
    {
        return {.view_state =
                    states::add_card_front{.book_id = cur_state.book_id,
                                           .chapter_id = cur_state.chapter_id},
                .model_data = data};
    }
    auto operator()(actions::remove a) const -> data
    {
        if (a.id)
            return {.view_state = cur_state,
                    .model_data = learn::data::update(
                        data,
                        learn::data::books::actions::remove_card{
                            .book_id = cur_state.book_id,
                            .chapter_id = cur_state.chapter_id,
                            .id = a.id.value()})};
        return {.view_state = cur_state, .model_data = data};
    }
    auto operator()(actions::start_training) const -> data
    {
        return {.view_state = states::training{},
                .model_data = learn::data::update(
                    data,
                    learn::data::training::actions::start{
                        .cards = data.the_books.at(cur_state.book_id)
                                     .chapters.at(cur_state.chapter_id)
                                     .cards})};
    }
    auto operator()(actions::quit) const -> data
    {
        return {.view_state = states::book{.book_id = cur_state.book_id},
                .model_data = data};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::chapter cur_state{};
    learn::data::data data{};
};
struct on_add_card_front
{
    auto operator()(actions::text_input a) const -> data
    {
        return {.view_state =
                    states::add_card_back{.book_id = cur_state.book_id,
                                          .chapter_id = cur_state.chapter_id,
                                          .front_txt = a.txt},
                .model_data = data};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::add_card_front cur_state{};
    learn::data::data data{};
};
struct on_add_card_back
{
    auto operator()(actions::text_input a) const -> data
    {
        return {.view_state =
                    states::chapter{.book_id = cur_state.book_id,
                                    .chapter_id = cur_state.chapter_id},
                .model_data =
                    learn::data::update(data,
                                        learn::data::books::actions::add_card{
                                            .book_id = cur_state.book_id,
                                            .chapter_id = cur_state.chapter_id,
                                            .front = cur_state.front_txt,
                                            .back = a.txt})};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::add_card_back cur_state{};
    learn::data::data data{};
};
struct on_training
{
    auto operator()(actions::text_input a) const -> data
    {
        return {.view_state = cur_state,
                .model_data = learn::data::update(
                    data,
                    learn::data::training::actions::answer{a.txt})};
    }
    auto operator()(actions::quit) const -> data
    {
        return {.view_state = states::books{}, .model_data = data};
    }
    auto operator()(auto) const -> data
    {
        return {.view_state = cur_state, .model_data = data};
    }
    states::training cur_state{};
    learn::data::data data{};
};
[[nodiscard]] auto update(data view_data, actions::action view_act) -> data
{
    using ::boost::hof::match;
    const auto view_state{view_data.view_state};
    const auto model_data{view_data.model_data};
    return std::visit(
        match(
            [&](states::books s, auto a) -> data {
                return on_books{.cur_state = s, .data = model_data}(a);
            },
            [&](states::add_book s, auto a) -> data {
                return on_add_book{.cur_state = s, .data = model_data}(a);
            },
            [&](states::book s, auto a) -> data {
                return on_book{.cur_state = s, .data = model_data}(a);
            },
            [&](states::add_chapter s, auto a) -> data {
                return on_add_chapter{.cur_state = s, .data = model_data}(a);
            },
            [&](states::chapter s, auto a) -> data {
                return on_chapter{.cur_state = s, .data = model_data}(a);
            },
            [&](states::add_card_front s, auto a) -> data {
                return on_add_card_front{.cur_state = s, .data = model_data}(a);
            },
            [&](states::add_card_back s, auto a) -> data {
                return on_add_card_back{.cur_state = s, .data = model_data}(a);
            },
            [&](states::training s, auto a) -> data {
                return on_training{.cur_state = s, .data = model_data}(a);
            }),
        view_state,
        view_act);
}
}
