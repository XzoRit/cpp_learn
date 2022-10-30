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
using chapter = ::xzr::learn::data::chapter;

auto create_chapter()
{
    auto chptr{chapter{}};
    for (int i{}; i < 10;)
    {
        chptr.cards.push_back(
            {.front = std::to_string(++i), .back = std::to_string(++i)});
    }

    return chptr;
}

BOOST_AUTO_TEST_CASE(all_cards_always_correct)
{
    auto chptr{create_chapter()};
    for (int i{}; i < 10;)
    {
        chptr.cards.push_back(
            {.front = std::to_string(++i), .back = std::to_string(++i)});
    }

    auto b{box{chptr}};
    BOOST_TEST(b.has_next());
    for (std::size_t i{}; i < chptr.cards.size(); ++i)
    {
        const auto& crd{b.next()};
        const auto it{std::ranges::find(chptr.cards, crd)};
        BOOST_TEST(*it == crd);
        b.move_card(crd, it->back);
    }
    BOOST_TEST(!b.has_next());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
