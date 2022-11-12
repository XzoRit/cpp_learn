#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)

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

}
