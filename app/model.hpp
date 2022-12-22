#pragma once

#include <data/data.hpp>

#include <optional>
#include <string>
#include <variant>

namespace xzr::learn::console::model
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
    std::optional<::xzr::learn::data::actions::action> data_act{};
    ::xzr::learn::console::model::states::state console_state{};
};
[[nodiscard]] auto update(::xzr::learn::console::model::actions::action act,
                          ::xzr::learn::console::model::states::state state,
                          const ::xzr::learn::data::data& data)
    -> ::xzr::learn::console::model::data;
}
