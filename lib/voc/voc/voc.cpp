#include <voc/voc.hpp>

#include <utility>

namespace xzr
{
namespace voc
{
inline namespace v1
{
auto vocabels::empty() const -> bool
{
    return vocs.empty();
}

auto vocabels::size() const -> size_type
{
    return vocs.size();
}

auto vocabels::add(vocabel v) -> void
{
    vocs.push_back(std::move(v));
}
}
}
}
