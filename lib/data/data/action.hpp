#pragma once

#include <string>
#include <variant>

namespace xzr::learn::data
{
inline namespace v1
{
struct add_book
{
    std::string name{};
};
struct remove_book
{
    int id{};
};
struct add_chapter
{
    int book_id{};
    std::string name{};
};
struct remove_chapter
{
    int book_id{};
    int id{};
};
struct add_card
{
    int book_id{};
    int chapter_id{};
    std::string front{};
    std::string back{};
};
struct remove_card
{
    int book_id{};
    int chapter_id{};
    int id{};
};
struct quit
{
};
using action = std::variant<add_book,
                            remove_book,
                            add_chapter,
                            remove_chapter,
                            add_card,
                            remove_card,
                            quit>;
}
}
