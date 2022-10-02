#pragma once

#include <string>
#include <vector>

namespace xzr
{
namespace voc
{
inline namespace v1
{
struct vocabel
{
    std::string de{};
    std::string en{};
};
struct vocabels
{
    std::string chapter{};
    std::vector<vocabel> vocs{};
};
}
}
}
