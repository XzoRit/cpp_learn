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

BOOST_AUTO_TEST_SUITE(training_v1)

using training = ::xzr::learn::data::training;
using card = ::xzr::learn::data::card;
using cards = ::xzr::learn::data::cards;
using chapter = ::xzr::learn::data::chapter;

struct training_tests
{
    static auto create_cards() -> cards
    {
        auto cs{cards{}};

        for (int i{}; i < 10; ++i)
        {
            cs.push_back(
                {.front = std::to_string(i), .back = std::to_string(i + 1)});
        }

        return cs;
    }

    cards crds{create_cards()};
    training t{start_training(crds)};
};

BOOST_FIXTURE_TEST_SUITE(suite_name, training_tests);

BOOST_AUTO_TEST_CASE(dafault_constructed_training)
{
    {
        const auto empty{training{}};
        BOOST_TEST(!current_card(empty).has_value());
        BOOST_TEST(current_card(eval_answer(t, card{}, "")).has_value());
    }
    {
        const auto empty{training{.cards = cards{}}};
        BOOST_TEST(!current_card(empty).has_value());
        BOOST_TEST(current_card(eval_answer(t, card{}, "")).has_value());
    }
}

BOOST_AUTO_TEST_CASE(all_cards_always_correct)
{
    for (std::size_t i{}; i < crds.size(); ++i)
    {
        BOOST_REQUIRE(current_card(t).has_value());
        const auto crd{current_card(t).value()};
        const auto it{std::ranges::find(crds, crd)};
        // correct answer
        t = eval_answer(t, crd, it->back);
    }

    BOOST_TEST(!current_card(t).has_value());
}

BOOST_AUTO_TEST_CASE(incorrect_cards_are_represented)
{

    for (std::size_t i{}; i < crds.size(); ++i)
    {
        BOOST_REQUIRE(current_card(t).has_value());
        const auto crd{current_card(t).value()};
        const auto it{std::ranges::find(crds, crd)};
        // incorrect answer
        t = eval_answer(t, crd, it->front);
    }

    BOOST_TEST(current_card(t).has_value());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(training_v2)

using ::xzr::learn::data::card;
using ::xzr::learn::data::cards;
using ::xzr::learn::data::v2::training::training;
using ::xzr::learn::data::v2::training::update;
using ::xzr::learn::data::v2::training::action::answer;
using ::xzr::learn::data::v2::training::action::start;
using ::xzr::learn::data::v2::training::states::done;
using ::xzr::learn::data::v2::training::states::show_card;

using ::boost::hof::match;

const auto contains{[](auto cs, auto c) {
    return std::ranges::any_of(cs, std::bind_front(std::equal_to{}, c));
}};
// cards must have unique front
// cards must have back != ""
const auto cs{cards{card{.front = "1", .back = "2"},
                    card{.front = "3", .back = "4"},
                    card{.front = "5", .back = "6"}}};

BOOST_AUTO_TEST_CASE(start_with_empty_cards)
{
    const auto empty_cards{cards{}};
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
