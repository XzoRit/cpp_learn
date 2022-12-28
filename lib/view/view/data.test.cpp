#include <view/data.hpp>

#include <data/data.hpp>

#include <boost/test/unit_test.hpp>

namespace
{
namespace view_actions = ::xzr::learn::view::actions;
// namespace view_states = ::xzr::learn::view::states;
using ::xzr::learn::view::update;
using view_data = ::xzr::learn::view::data;
using model_data = ::xzr::learn::data::data;
BOOST_AUTO_TEST_SUITE(view_tests)
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_CASE(with_action)
{
    const auto a{update(view_data{}, view_actions::action{})};
    BOOST_TEST(a.model_data.the_books.empty());
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
