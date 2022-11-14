#include <data/action.hpp>
#include <data/app.hpp>
#include <data/update.hpp>

#include <boost/test/unit_test.hpp>
#include <libs/test/include/boost/test/tools/old/interface.hpp>

namespace
{
BOOST_AUTO_TEST_SUITE(update_tests)

using ::xzr::learn::update::update;

using app = ::xzr::learn::data::app;
using add_book = ::xzr::learn::action::add_book;

BOOST_AUTO_TEST_CASE(update_with_add_book)
{
    const auto the_app{app{}};

    const auto new_app{update(the_app, add_book{.name = "book 2"})};

    BOOST_REQUIRE(new_app.the_books.size() == 1u);
    BOOST_TEST(new_app.the_books[0].name == "book 2");
}

BOOST_AUTO_TEST_SUITE_END()
}
