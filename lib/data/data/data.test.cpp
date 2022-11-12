#include <data/data.hpp>
#include <data/serialize.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <sstream>
#include <string>

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
        auto oa{::boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{card{}};
    {
        auto ia{::boost::archive::text_iarchive{ss}};
        ia >> b;
    }
    BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(chapter_tests)

using chapter = ::xzr::learn::data::chapter;

BOOST_AUTO_TEST_CASE(chapter_default_ctor)
{
    const auto a{chapter{}};
    BOOST_TEST(a.cards.empty());
    BOOST_TEST(a.name == "");
}

BOOST_AUTO_TEST_CASE(chapter_initialized_ctor)
{
    const auto a{chapter{.name = "ch_01",
                         .cards = {{.front = "Hello", .back = "Hallo"}}}};
    BOOST_TEST(!a.cards.empty());
    BOOST_TEST(a.cards.size() == 1u);
}

BOOST_AUTO_TEST_CASE(chapter_serialization)
{
    const auto a{chapter{.name = "ch_01",
                         .cards = {{.front = "Hello", .back = "Hallo"}}}};
    auto ss{std::stringstream{}};
    {
        auto oa{boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{chapter{}};
    {
        auto ia{boost::archive::text_iarchive{ss}};
        ia >> b;
    }

    BOOST_TEST(a == b);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(book_tests)

using book = ::xzr::learn::data::book;

BOOST_AUTO_TEST_CASE(book_ctor)
{
    const auto a{book{}};
    BOOST_TEST(a.chapters.empty());
    BOOST_TEST(a.name == "");
}

BOOST_AUTO_TEST_CASE(book_initialized_ctor)
{
    const auto a{
        book{.name = "pack_01",
             .chapters = {{.name = "ch_01",
                           .cards = {{.front = "Hello", .back = "Hallo"}}}}}};
    BOOST_TEST(!a.chapters.empty());
    BOOST_TEST(a.chapters.size() == 1u);
}

BOOST_AUTO_TEST_CASE(book_serialization)
{
    const auto a{
        book{.name = "pack_01",
             .chapters = {{.name = "ch_01",
                           .cards = {{.front = "Hello", .back = "Hallo"}}}}}};
    auto ss{std::stringstream{}};
    {
        auto oa{boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{book{}};
    {
        auto ia{boost::archive::text_iarchive{ss}};
        ia >> b;
    }
    BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
}
