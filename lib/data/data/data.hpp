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
    std::string name{};
    std::vector<card> content{};
};
struct package
{
    std::string name{};
    std::vector<cards> content{};
};
}
}
