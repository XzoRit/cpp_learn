#include <data/books.hpp>
#include <data/ostream.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_SUITE(books_tests)
using ::xzr::learn::data::books::book;
using ::xzr::learn::data::books::books;
using ::xzr::learn::data::books::card;
using ::xzr::learn::data::books::cards;
using ::xzr::learn::data::books::chapter;
using ::xzr::learn::data::books::chapters;
using ::xzr::learn::data::books::update;
using ::xzr::learn::data::books::actions::add_book;
using ::xzr::learn::data::books::actions::add_card;
using ::xzr::learn::data::books::actions::add_chapter;
using ::xzr::learn::data::books::actions::remove_book;
using ::xzr::learn::data::books::actions::remove_card;
using ::xzr::learn::data::books::actions::remove_chapter;
auto make_empty_books()
{
    return books{};
}
auto make_books(const std::string& book_name)
{
    return books{book{.name = book_name}};
}
auto make_books(const std::string& book_name, const std::string& chapter_name)
{
    return books{
        book{.name = book_name, .chapters{chapter{.name = chapter_name}}}};
}
auto make_books(const std::string& book_name,
                const std::string& chapter_name,
                const std::string& card_front,
                const std::string& card_back)
{
    return books{book{.name = book_name,
                      .chapters{chapter{.name = chapter_name,
                                        .cards{card{.front = card_front,
                                                    .back = card_back}}}}}};
}
BOOST_AUTO_TEST_CASE(with_add_book)
{
    const auto bs{make_empty_books()};
    const auto new_bs{update(bs, add_book{.name = "book name"})};
    BOOST_TEST(new_bs != bs);
    BOOST_REQUIRE(new_bs.size() == 1u);
    BOOST_TEST(new_bs[0].name == "book name");
}
BOOST_AUTO_TEST_CASE(with_remove_book)
{
    const auto bs{make_books("book name")};
    const auto new_bs{update(bs, remove_book{.id = 0})};
    BOOST_TEST(new_bs != bs);
    BOOST_TEST(new_bs.empty());
}
BOOST_AUTO_TEST_CASE(with_removal_on_empty_books)
{
    const auto bs{make_empty_books()};
    BOOST_REQUIRE(bs.empty());
    const auto new_bs{update(bs, remove_book{.id = 0})};
    BOOST_TEST(new_bs == bs);
}
BOOST_AUTO_TEST_CASE(with_removal_wrong_book_id)
{
    const auto bs{make_books("book name")};
    {
        const auto new_bs{update(bs, remove_book{.id = 1})};
        BOOST_TEST(new_bs == bs);
    }
    {
        const auto new_bs{update(bs, remove_book{.id = -1})};
        BOOST_TEST(new_bs == bs);
    }
}
BOOST_AUTO_TEST_CASE(with_add_chapter_to_book)
{
    const auto bs{make_books("book name")};
    const auto new_bs{
        update(bs, add_chapter{.book_id = 0, .name = "chapter name"})};
    BOOST_TEST(new_bs != bs);
    BOOST_REQUIRE(new_bs.at(0).chapters.size() == 1u);
    BOOST_TEST(new_bs.at(0).chapters.at(0).name == "chapter name");
}
BOOST_AUTO_TEST_CASE(with_remove_chapter)
{
    const auto bs{make_books("book name", "chapter name")};
    const auto new_bs{update(bs, remove_chapter{.book_id = 0, .id = 0})};
    BOOST_TEST(new_bs != bs);
    BOOST_TEST(new_bs.at(0).chapters.empty());
}
BOOST_AUTO_TEST_CASE(with_removal_on_empty_chapters)
{
    const auto bs{make_books("book name")};
    BOOST_REQUIRE(bs.at(0).chapters.empty());
    const auto new_bs{update(bs, remove_chapter{.book_id = 0, .id = 0})};
    BOOST_TEST(new_bs == bs);
}
BOOST_AUTO_TEST_CASE(with_removal_wrong_chapter_id)
{
    const auto bs{make_books("book name", "chapter name")};
    {
        const auto new_bs{update(bs, remove_chapter{.id = 1})};
        BOOST_TEST(new_bs == bs);
    }
    {
        const auto new_bs{update(bs, remove_chapter{.id = -1})};
        BOOST_TEST(new_bs == bs);
    }
}
BOOST_AUTO_TEST_CASE(with_add_card_to_chapter)
{
    const auto bs{make_books("book name", "chapter name")};
    const auto new_bs{update(bs,
                             add_card{.book_id = 0,
                                      .chapter_id = 0,
                                      .front = "front",
                                      .back = "back"})};
    BOOST_TEST(new_bs != bs);
    BOOST_REQUIRE(new_bs.at(0).chapters.at(0).cards.size() == 1u);
    BOOST_TEST(new_bs.at(0).chapters.at(0).cards.at(0).front == "front");
    BOOST_TEST(new_bs.at(0).chapters.at(0).cards.at(0).back == "back");
}
BOOST_AUTO_TEST_CASE(with_remove_card)
{
    const auto bs{make_books("book name", "chapter name", "front", "back")};
    const auto new_bs{
        update(bs, remove_card{.book_id = 0, .chapter_id = 0, .id = 0})};
    BOOST_TEST(new_bs != bs);
    BOOST_TEST(new_bs.at(0).chapters.at(0).cards.empty());
}
BOOST_AUTO_TEST_CASE(with_removal_on_empty_cards)
{
    const auto bs{make_books("book name")};
    BOOST_REQUIRE(bs.at(0).chapters.empty());
    const auto new_bs{update(bs, remove_chapter{.book_id = 0, .id = 0})};
    BOOST_TEST(new_bs == bs);
}
BOOST_AUTO_TEST_CASE(with_removal_wrong_card_id)
{
    const auto bs{make_books("book name", "chapter name")};
    {
        const auto new_bs{update(bs, remove_chapter{.id = 1})};
        BOOST_TEST(new_bs == bs);
    }
    {
        const auto new_bs{update(bs, remove_chapter{.id = -1})};
        BOOST_TEST(new_bs == bs);
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
