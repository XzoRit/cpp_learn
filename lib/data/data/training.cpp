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
auto start_training(books::cards cs) -> training
{
    ::shuffle(cs);
    return training{.cards = std::move(cs)};
}
auto current_card(const training& t) -> std::optional<books::card>
{
    if (t.cards.empty())
        return std::nullopt;
    return t.cards.back();
}
auto eval_answer(training t, books::card crd, std::string_view back) -> training
{
    if (crd.back != back)
        t.cards.insert(t.cards.begin(), crd);
    t.cards.pop_back();

    return t;
}
}
}

#include <boost/hof/match.hpp>

#include <algorithm>
#include <iterator>

namespace xzr::learn::data
{
namespace v2::training
{
namespace
{
auto eval_answer(books::cards cs, books::card c, std::string_view answer)
{
    if (c.back != answer)
        std::rotate(cs.begin(), cs.begin() + 1, cs.end());
    else
        cs.erase(cs.begin());

    return cs;
}
}
auto update(training t, action::action a) -> training
{
    using ::boost::hof::match;

    return std::visit(
        match(
            [t](states::done, action::start a) {
                if (a.cards.empty())
                    return t;
                ::shuffle(a.cards);
                return training{.state =
                                    states::show_card{.card = a.cards.front()},
                                .cards = a.cards};
            },
            [&](states::done, auto) { return t; },
            [&](states::show_card s, action::answer a) {
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
}
