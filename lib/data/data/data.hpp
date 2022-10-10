#pragma once

#include <string>
#include <vector>

namespace xzr::learn::data
{
inline namespace v1
{
struct card
{
    std::string front{};
    std::string back{};
};
struct cards
{
    using container = std::vector<card>;
    using size_type = container::size_type;

    auto empty() const -> bool;
    auto size() const -> size_type;

    auto add(card v) -> void;

    std::string chapter{};
    container vocs{};
};
}
}
