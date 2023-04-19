#include <data/books.hpp>
#include <data/ostream.hpp>
#include <data/training.hpp>

#include <boost/hof/match.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_SUITE(training_tests)
using ::boost::hof::match;
using ::xzr::learn::data::books::card;
using ::xzr::learn::data::books::cards_t;
using ::xzr::learn::data::training::training;
using ::xzr::learn::data::training::update;
using ::xzr::learn::data::training::actions::answer;
using ::xzr::learn::data::training::actions::start;
using ::xzr::learn::data::training::states::done;
using ::xzr::learn::data::training::states::show_card;
const auto contains{[](auto cs, auto c) {
    return std::ranges::any_of(cs, std::bind_front(std::equal_to{}, c));
}};
// cards must have unique front
// cards must have back != ""
const auto cs{cards_t{card{.front = "1", .back = "2"},
                      card{.front = "3", .back = "4"},
                      card{.front = "5", .back = "6"}}};
BOOST_AUTO_TEST_CASE(start_with_empty_cards)
{
    const auto empty_cards{cards_t{}};
    auto t{training{}};
    t = update(t, start{empty_cards});
    std::visit(
        match([](done) { BOOST_TEST(true); }, [](auto) { BOOST_TEST(false); }),
        t.state);
    BOOST_TEST(t.cards.empty());
}
BOOST_AUTO_TEST_CASE(cards_are_shuffled_on_start)
{
    auto t{training{}};
    t = update(t, start{cs});
    BOOST_TEST(t.cards.size() == cs.size());
    for (auto c : cs)
        BOOST_TEST(contains(t.cards, c));
}
BOOST_AUTO_TEST_CASE(all_cards_right_first_time)
{
    auto t{training{}};
    t = update(t, start{cs});
    for (int i{1}; i < cs.size(); ++i)
    {
        std::visit(
            match([](auto) { BOOST_TEST(false); },
                  [&](show_card s) { BOOST_TEST(t.cards.front() == s.card); }),
            t.state);
        t = update(t, answer{.txt = t.cards.front().back});
        BOOST_TEST(t.cards.size() == cs.size() - i);
    }
    t = update(t, answer{.txt = t.cards.front().back});
    std::visit(
        match([](done) { BOOST_TEST(true); }, [](auto) { BOOST_TEST(false); }),
        t.state);
    BOOST_TEST(t.cards.empty());
}
BOOST_AUTO_TEST_CASE(all_wrong_cards_are_represented)
{
    auto t{training{}};
    const auto card_with_wrong_answer{[]() {
        auto c{cs.front()};
        c.back = "";
        return c;
    }()};
    t = update(t, start{cs});
    for (int _{}; _ < cs.size() * 2; ++_)
    {
        std::visit(match([](auto) { BOOST_TEST(false); },
                         [](show_card) { BOOST_TEST(true); }),
                   t.state);
        t = update(t, answer{.txt = ""});
        BOOST_TEST(t.cards.size() == cs.size());
        for (auto c : cs)
            BOOST_TEST(contains(t.cards, c));
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
