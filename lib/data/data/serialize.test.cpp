#include <data/books.hpp>
#include <data/ostream.hpp>
#include <data/serialize.hpp>
#include <data/training.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)

BOOST_AUTO_TEST_SUITE(serialize_tests)

using chapter = ::xzr::learn::data::books::chapter;
using card = ::xzr::learn::data::books::card;
using book = ::xzr::learn::data::books::book;
using training = ::xzr::learn::data::training;

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

BOOST_AUTO_TEST_CASE(training_serialization)
{
    const auto a{training{.cards = {{.front = "Hello", .back = "Hallo"}}}};
    auto ss{std::stringstream{}};
    {
        auto oa{boost::archive::text_oarchive{ss}};
        oa << a;
    }
    auto b{training{}};
    {
        auto ia{boost::archive::text_iarchive{ss}};
        ia >> b;
    }
    BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
}
