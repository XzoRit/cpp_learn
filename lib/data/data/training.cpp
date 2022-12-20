#include <data/training.hpp>

#include <boost/hof/match.hpp>

#include <algorithm>
#include <random>
#include <string_view>

namespace
{
template <class Range>
auto shuffle(Range& r) -> void
{
    static auto rd{std::random_device{}};
    static auto g{std::mt19937{rd()}};
    std::ranges::shuffle(r.begin(), r.end(), g);
}
auto eval_answer(::xzr::learn::data::books::cards cs,
                 ::xzr::learn::data::books::card c,
                 std::string_view answer)
{
    if (c.back != answer)
        std::rotate(cs.begin(), cs.begin() + 1, cs.end());
    else
        cs.erase(cs.begin());
    return cs;
}
}
namespace xzr::learn::data::training
{
auto update(training t, actions::action a) -> training
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [t](states::done, actions::start a) {
                if (a.cards.empty())
                    return t;
                ::shuffle(a.cards);
                return training{.state =
                                    states::show_card{.card = a.cards.front()},
                                .cards = a.cards};
            },
            [&](states::done, auto) { return t; },
            [&](states::show_card s, actions::answer a) {
                const auto new_cards{eval_answer(t.cards, s.card, a.txt)};
                if (new_cards.empty())
                    return training{.state = states::done{},
                                    .cards = new_cards};
                return training{
                    .state = states::show_card{.card = new_cards.front()},
                    .cards = new_cards};
            },
            [&](states::show_card, auto) { return t; }),
        t.state,
        a);
}
}
