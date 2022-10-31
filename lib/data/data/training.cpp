#include <data/training.hpp>

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
auto start_training(cards cs) -> training
{
    ::shuffle(cs);
    return training{.cards = std::move(cs)};
}
auto current_card(const training& t) -> std::optional<card>
{
    if (t.cards.empty())
        return std::nullopt;
    return t.cards.back();
}
auto eval_answer(training t, card crd, std::string_view back) -> training
{
    if (crd.back != back)
        t.cards.insert(t.cards.begin(), crd);
    t.cards.pop_back();

    return t;
}
}
}
