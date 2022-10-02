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
    using container = std::vector<vocabel>;
    using size_type = container::size_type;

    auto empty() const -> bool;
    auto size() const -> size_type;

    auto add(vocabel v) -> void;

    std::string chapter{};
    container vocs{};
};
}
}
}
