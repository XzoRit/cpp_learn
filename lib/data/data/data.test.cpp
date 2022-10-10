#include <data/data.hpp>

#include <boost/test/unit_test.hpp>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_SUITE(card_tests)
using card = ::xzr::learn::data::card;
BOOST_AUTO_TEST_CASE(card_default_ctor)
{
    card a{};
    BOOST_TEST(a.front == "");
    BOOST_TEST(a.back == "");
}
BOOST_AUTO_TEST_CASE(card_initialized_ctor)
{
    card a{.front = "Hello", .back = "Hallo"};
    BOOST_TEST(a.front == "Hello");
    BOOST_TEST(a.back == "Hallo");
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(cards_tests)
using cards = ::xzr::learn::data::cards;
BOOST_AUTO_TEST_CASE(cards_ctor)
{
    cards a{};
    BOOST_TEST(a.empty());
    BOOST_TEST(a.size() == 0u);
    BOOST_TEST(a.name == "");
}
BOOST_AUTO_TEST_CASE(add_cards)
{
    cards a{.name = "ch_01"};
    a.add({.front = "Hello", .back = "Hallo"});
    BOOST_TEST(!a.empty());
    BOOST_TEST(a.size() == 1u);
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(package_tests)
using package = ::xzr::learn::data::package;
BOOST_AUTO_TEST_CASE(package_ctor)
{
    package a{};
    BOOST_TEST(a.empty());
    BOOST_TEST(a.size() == 0u);
    BOOST_TEST(a.name == "");
}
BOOST_AUTO_TEST_CASE(add_package)
{
    package a{.name = "pack_01"};
    a.add({.name = "ch_01", .cont = {{.front = "Hello", .back = "Hallo"}}});
    BOOST_TEST(!a.empty());
    BOOST_TEST(a.size() == 1u);
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
