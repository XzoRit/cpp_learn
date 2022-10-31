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
training::training(cards c)
    : cs{std::move(c)}
{
    ::shuffle(cs);
}
auto training::has_next() const -> bool
{
    return !cs.empty();
}
auto training::next() const -> const card&
{
    return cs.back();
}
auto training::move_card(const card& c, const std::string& back) -> void
{
    if (c.back != back)
    {
        cs.insert(cs.begin(), c);
    }
    cs.pop_back();
}
}
}
