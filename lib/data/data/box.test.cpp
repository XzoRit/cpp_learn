#include <data/box.hpp>
#include <data/data.hpp>

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)

BOOST_AUTO_TEST_SUITE(box_tests)

using box = ::xzr::learn::data::box;
using card = ::xzr::learn::data::card;
using cards = ::xzr::learn::data::cards;
using chapter = ::xzr::learn::data::chapter;

auto create_cards()
{
    auto crds{cards{}};
    for (int i{}; i < 10; ++i)
    {
        crds.push_back(
            {.front = std::to_string(i), .back = std::to_string(i + 1)});
    }

    return crds;
}

BOOST_AUTO_TEST_CASE(all_cards_always_correct)
{
    auto crds{create_cards()};
    auto b{box{crds}};
    BOOST_REQUIRE(b.has_next());

    for (std::size_t i{}; i < crds.size(); ++i)
    {
        const auto& crd{b.next()};
        const auto it{std::ranges::find(crds, crd)};
        // correct answer
        b.move_card(crd, it->back);
    }

    BOOST_TEST(!b.has_next());
}

BOOST_AUTO_TEST_CASE(incorrect_cards_are_represented)
{
    auto crds{create_cards()};
    auto b{box{crds}};
    BOOST_REQUIRE(b.has_next());

    for (std::size_t i{}; i < crds.size(); ++i)
    {
        const auto& crd{b.next()};
        const auto it{std::ranges::find(crds, crd)};
        // incorrect answer
        b.move_card(crd, it->front);
    }

    BOOST_TEST(b.has_next());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
