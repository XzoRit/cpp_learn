#include <view/data.hpp>
#include <view/ostream.hpp>

#include <data/data.hpp>

#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>

namespace
{
using namespace std::string_literals;
namespace data = ::xzr::learn::data;
namespace view = ::xzr::learn::view;
namespace view_states = view::states;
BOOST_AUTO_TEST_SUITE(view_tests)
BOOST_AUTO_TEST_SUITE(ostream_tests)
BOOST_AUTO_TEST_CASE(streaming)
{
    {
        const auto a{view_states::state{view_states::books{}}};
        const auto a_str{"books{}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{view_states::state{view_states::add_book{}}};
        const auto a_str{"add_book{}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{view_states::state{view_states::book{.book_id = 0}}};
        const auto a_str{"book{.book_id = 0}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{
            view_states::state{view_states::add_chapter{.book_id = 0}}};
        const auto a_str{"add_chapter{.book_id = 0}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{view_states::state{
            view_states::chapter{.book_id = 0, .chapter_id = 0}}};
        const auto a_str{"chapter{.book_id = 0, .chapter_id = 0}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{view_states::state{
            view_states::add_card_front{.book_id = 0, .chapter_id = 0}}};
        const auto a_str{"add_card_front{.book_id = 0, .chapter_id = 0}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{
            view_states::state{view_states::add_card_back{.book_id = 0,
                                                          .chapter_id = 0,
                                                          .front_txt = ""}}};
        const auto a_str{
            "add_card_back{.book_id = 0, .chapter_id = 0, .front_txt = }"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{view_states::state{view_states::training{}}};
        const auto a_str{"training{}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
    {
        const auto a{view::data{.view_state = view_states::books{},
                                .model_data = data::data{
                                    .the_books = {},
                                    .the_training = data::training::training{
                                        .state = data::training::states::done{},
                                        .cards = {}}}}};
        const auto a_str{
            "data{"
            ".view_state = books{}, "
            ".model_data = data{.the_books = [], .the_training = training{.state = done{}, .cards = []}}}"s};
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == a_str);
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
