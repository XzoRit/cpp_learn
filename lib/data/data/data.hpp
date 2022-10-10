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
    auto add(card c) -> void;
    std::string name{};
    container cont{};
};
struct package
{
    using container = std::vector<cards>;
    using size_type = container::size_type;
    auto empty() const -> bool;
    auto size() const -> size_type;
    auto add(cards cs) -> void;
    std::string name{};
    container cont{};
};
}
}
