#pragma once

#include <data/data.hpp>

#include <optional>
#include <string>
#include <variant>

namespace xzr::learn::console::view::model
{
namespace actions
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
namespace states
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
}
struct data
{
    states::state view_state{};
    learn::data::data model_data{};
};
[[nodiscard]] auto update(actions::action view_act, data view_data) -> data;
}
