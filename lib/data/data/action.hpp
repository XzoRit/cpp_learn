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
struct quit
{
};
using action = std::variant<add_book, quit>;
}
}
