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
using cards = ::xzr::learn::data::chapter;

BOOST_AUTO_TEST_CASE(all_cards_always_correct)
{
    auto cs{cards{}};
    for (int i{}; i < 10;)
    {
        cs.cards.push_back(
            {.front = std::to_string(++i), .back = std::to_string(++i)});
    }

    auto b{box{cs}};
    BOOST_TEST(b.has_next());
    for (std::size_t i{}; i < cs.cards.size(); ++i)
    {
        const auto& c{b.next()};
        const auto it{std::ranges::find(cs.cards, c)};
        BOOST_TEST(*it == c);
        b.move_card(c, it->back);
    }
    BOOST_TEST(!b.has_next());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
