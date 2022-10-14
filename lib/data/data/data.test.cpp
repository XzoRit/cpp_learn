#include <data/data.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/test/unit_test.hpp>

#include <sstream>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)

BOOST_AUTO_TEST_SUITE(card_tests)

using card = ::xzr::learn::data::card;

BOOST_AUTO_TEST_CASE(card_default_ctor)
{
    const auto a{card{}};
    BOOST_TEST(a.front == "");
    BOOST_TEST(a.back == "");
}
BOOST_AUTO_TEST_CASE(card_initialized_ctor)
{
    const auto a{card{.front = "Hello", .back = "Hallo"}};
    BOOST_TEST(a.front == "Hello");
    BOOST_TEST(a.back == "Hallo");
}
BOOST_AUTO_TEST_CASE(card_serialization)
{
    const auto a{card{.front = "Hello", .back = "Hallo"}};
    auto ss{std::stringstream{}};
    {
        auto oa{boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{card{}};
    {
        auto ia{boost::archive::text_iarchive{ss}};
        ia >> b;
    }
    BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(cards_tests)

using cards = ::xzr::learn::data::cards;

BOOST_AUTO_TEST_CASE(cards_default_ctor)
{
    const auto a{cards{}};
    BOOST_TEST(a.content.empty());
    BOOST_TEST(a.name == "");
}
BOOST_AUTO_TEST_CASE(cards_initialized_ctor)
{
    const auto a{cards{.name = "ch_01",
                       .content = {{.front = "Hello", .back = "Hallo"}}}};
    BOOST_TEST(!a.content.empty());
    BOOST_TEST(a.content.size() == 1u);
}
BOOST_AUTO_TEST_CASE(cards_serialization)
{
    const auto a{cards{.name = "ch_01",
                       .content = {{.front = "Hello", .back = "Hallo"}}}};
    auto ss{std::stringstream{}};
    {
        auto oa{boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{cards{}};
    {
        auto ia{boost::archive::text_iarchive{ss}};
        ia >> b;
    }

    BOOST_TEST(a == b);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(package_tests)

using package = ::xzr::learn::data::package;

BOOST_AUTO_TEST_CASE(package_ctor)
{
    const auto a{package{}};
    BOOST_TEST(a.content.empty());
    BOOST_TEST(a.name == "");
}
BOOST_AUTO_TEST_CASE(package_initialized_ctor)
{
    const auto a{package{
        .name = "pack_01",
        .content = {{.name = "ch_01",
                     .content = {{.front = "Hello", .back = "Hallo"}}}}}};
    BOOST_TEST(!a.content.empty());
    BOOST_TEST(a.content.size() == 1u);
}
BOOST_AUTO_TEST_CASE(package_serialization)
{
    const auto a{package{
        .name = "pack_01",
        .content = {{.name = "ch_01",
                     .content = {{.front = "Hello", .back = "Hallo"}}}}}};
    auto ss{std::stringstream{}};
    {
        auto oa{boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{package{}};
    {
        auto ia{boost::archive::text_iarchive{ss}};
        ia >> b;
    }
    BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
}
