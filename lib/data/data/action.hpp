#pragma once

#include <string>
#include <variant>

namespace xzr::learn::action
{
inline namespace v1
{
struct add_book
{
    std::string name{};
};
struct add_chapter
{
};
struct add_card
{
};
struct start_training
{
};
struct quit
{
};
using action =
    std::variant<add_book, add_chapter, add_card, start_training, quit>;
}
}
