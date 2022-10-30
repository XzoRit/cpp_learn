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
box::box(chapter c)
    : cs{std::move(c)}
{
    ::shuffle(cs.content);
}
auto box::has_next() const -> bool
{
    return !cs.content.empty();
}
auto box::next() const -> const card&
{
    return cs.content.back();
}
auto box::move_card(const card& c, const std::string& back) -> void
{
    if (c.back != back)
    {
        cs.content.insert(cs.content.begin(), c);
    }
    cs.content.pop_back();
}
}
}
