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
    int id{};
};
struct quit
{
};
using action =
    std::variant<add_book, remove_book, add_chapter, remove_chapter, quit>;
}
}
