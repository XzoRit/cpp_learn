#include <data/data.hpp>

#include <utility>

namespace xzr::learn::data
{
inline namespace v1
{
auto cards::empty() const -> bool
{
    return vocs.empty();
}

auto cards::size() const -> size_type
{
    return vocs.size();
}

auto cards::add(card v) -> void
{
    vocs.push_back(std::move(v));
}
}
}
