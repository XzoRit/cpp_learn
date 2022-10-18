#include <data/data.hpp>

#include <algorithm>
#include <random>

namespace xzr::learn::data
{
inline namespace v1
{
auto shuffle(cards cs) -> cards
{
    static std::random_device rd{};
    static std::mt19937 g{rd()};

    std::ranges::shuffle(cs.content, g);

    return cs;
}
}
}
