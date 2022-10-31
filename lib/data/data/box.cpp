#include <data/box.hpp>

#include <algorithm>
#include <random>
#include <utility>

namespace
{
template <class Range>
auto shuffle(Range& r) -> void
{
    static auto rd{std::random_device{}};
    static auto g{std::mt19937{rd()}};

    std::ranges::shuffle(r.begin(), r.end(), g);
}
}
namespace xzr::learn::data
{
inline namespace v1
{
box::box(cards c)
    : cs{std::move(c)}
{
    ::shuffle(cs);
}
auto box::has_next() const -> bool
{
    return !cs.empty();
}
auto box::next() const -> const card&
{
    return cs.back();
}
auto box::move_card(const card& c, const std::string& back) -> void
{
    if (c.back != back)
    {
        cs.insert(cs.begin(), c);
    }
    cs.pop_back();
}
}
}
