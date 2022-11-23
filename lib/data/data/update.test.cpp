#include "data/data.hpp"
#include <data/action.hpp>
#include <data/app.hpp>
#include <data/ostream.hpp>
#include <data/update.hpp>

#include <boost/test/unit_test.hpp>
#include <libs/test/include/boost/test/tools/old/interface.hpp>

namespace
{
BOOST_AUTO_TEST_SUITE(update_tests)

using ::xzr::learn::data::update;

using app = ::xzr::learn::data::app;
using book = ::xzr::learn::data::book;
using books = ::xzr::learn::data::books;
using chapter = ::xzr::learn::data::chapter;
using chapters = ::xzr::learn::data::chapters;
using add_book = ::xzr::learn::data::add_book;
using remove_book = ::xzr::learn::data::remove_book;
using add_chapter = ::xzr::learn::data::add_chapter;
using remove_chapter = ::xzr::learn::data::remove_chapter;

auto app_data()
{
    return app{};
}
auto app_data(const std::string& book_name)
{
    return app{.the_books = books{book{.name = book_name}}};
}
auto app_data(const std::string& book_name, const std::string& chapter_name)
{
    return app{.the_books =
                   books{book{.name = book_name,
                              .chapters{chapter{.name = chapter_name}}}}};
}
BOOST_AUTO_TEST_CASE(update_with_add_book)
{
    const auto the_app{app_data()};

    const auto new_app{update(the_app, add_book{.name = "book name"})};

    BOOST_TEST(new_app != the_app);
    BOOST_REQUIRE(new_app.the_books.size() == 1u);
    BOOST_TEST(new_app.the_books[0].name == "book name");
}

BOOST_AUTO_TEST_CASE(update_with_remove_book)
{
    const auto the_app{app_data("book name")};

    const auto new_app{update(the_app, remove_book{.id = 0})};

    BOOST_TEST(new_app != the_app);
    BOOST_TEST(new_app.the_books.empty());
}

BOOST_AUTO_TEST_CASE(update_with_removal_on_empty_books)
{
    const auto the_app{app_data()};
    BOOST_REQUIRE(the_app.the_books.empty());

    const auto new_app{update(the_app, remove_book{.id = 0})};

    BOOST_TEST(new_app == the_app);
}

BOOST_AUTO_TEST_CASE(update_with_removal_wrong_book_id)
{
    const auto the_app{app_data("book name")};
    {
        const auto new_app{update(the_app, remove_book{.id = 1})};

        BOOST_TEST(new_app == the_app);
    }
    {
        const auto new_app{update(the_app, remove_book{.id = -1})};

        BOOST_TEST(new_app == the_app);
    }
}

BOOST_AUTO_TEST_CASE(update_with_add_chapter_to_book)
{
    const auto the_app{app_data("book name")};

    const auto new_app{
        update(the_app, add_chapter{.book_id = 0, .name = "chapter name"})};

    BOOST_TEST(new_app != the_app);
    BOOST_REQUIRE(new_app.the_books.at(0).chapters.size() == 1u);
    BOOST_REQUIRE(new_app.the_books.at(0).chapters.at(0).name ==
                  "chapter name");
}

BOOST_AUTO_TEST_CASE(update_with_remove_chapter)
{
    const auto the_app{app_data("book name", "chapter name")};

    const auto new_app{update(the_app, remove_chapter{.book_id = 0, .id = 0})};

    BOOST_TEST(new_app != the_app);
    BOOST_TEST(new_app.the_books.at(0).chapters.empty());
}

BOOST_AUTO_TEST_CASE(update_with_removal_on_empty_chapters)
{
    const auto the_app{app_data("book name")};
    BOOST_REQUIRE(the_app.the_books.at(0).chapters.empty());

    const auto new_app{update(the_app, remove_chapter{.book_id = 0, .id = 0})};

    BOOST_TEST(new_app == the_app);
}

BOOST_AUTO_TEST_CASE(update_with_removal_wrong_chapter_id)
{
    const auto the_app{app_data("book name", "chapter name")};
    {
        const auto new_app{update(the_app, remove_chapter{.id = 1})};

        BOOST_TEST(new_app == the_app);
    }
    {
        const auto new_app{update(the_app, remove_chapter{.id = -1})};

        BOOST_TEST(new_app == the_app);
    }
}

BOOST_AUTO_TEST_SUITE_END()
}
