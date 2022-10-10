#include <data/data.hpp>

#include <utility>

namespace xzr::learn::data
{
inline namespace v1
{
auto cards::empty() const -> bool
{
    return cont.empty();
}
auto cards::size() const -> size_type
{
    return cont.size();
}
auto cards::add(card c) -> void
{
    cont.push_back(std::move(c));
}
auto package::empty() const -> bool
{
    return cont.empty();
}
auto package::size() const -> size_type
{
    return cont.size();
}
auto package::add(cards cs) -> void
{
    cont.push_back(std::move(cs));
}
}
}
