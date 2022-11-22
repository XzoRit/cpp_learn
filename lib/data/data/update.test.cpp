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
using add_book = ::xzr::learn::data::add_book;
using add_book = ::xzr::learn::data::add_book;
using remove_book = ::xzr::learn::data::remove_book;

BOOST_AUTO_TEST_CASE(update_with_add_book)
{
    const auto the_app{app{}};

    const auto new_app{update(the_app, add_book{.name = "book 2"})};

    BOOST_REQUIRE(new_app.the_books.size() == 1u);
    BOOST_TEST(new_app.the_books[0].name == "book 2");
}

BOOST_AUTO_TEST_CASE(update_with_remove_book)
{
    const auto the_app{app{.the_books = books{book{.name = "book 2"}}}};

    const auto new_app{update(the_app, remove_book{.id = 0})};

    BOOST_TEST(new_app.the_books.size() == 0u);
}

BOOST_AUTO_TEST_CASE(update_with_removal_on_empty_books)
{
    const auto the_app{app{.the_books = books{}}};

    const auto new_app{update(the_app, remove_book{.id = 0})};

    BOOST_TEST(new_app == the_app);
}

BOOST_AUTO_TEST_CASE(update_with_removal_wrong_id)
{
    const auto the_app{app{.the_books = books{book{.name = "book 2"}}}};
    {
        const auto new_app{update(the_app, remove_book{.id = 1})};

        BOOST_TEST(new_app == the_app);
    }
    {
        const auto new_app{update(the_app, remove_book{.id = -1})};

        BOOST_TEST(new_app == the_app);
    }
}

BOOST_AUTO_TEST_SUITE_END()
}
