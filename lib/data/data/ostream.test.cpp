#include <data/books.hpp>
#include <data/data.hpp>
#include <data/ostream.hpp>
#include <data/training.hpp>

#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>

namespace
{
using namespace std::string_literals;
namespace data = ::xzr::learn::data;
namespace data_books = data::books;
namespace data_training = data::training;
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_SUITE(ostream_tests)
BOOST_AUTO_TEST_CASE(streaming)
{
    const auto c1{data_books::card{.front = "1", .back = "2"}};
    std::ostringstream ostr{};
    ostr << c1;
    const auto c1_str{"card{.front = 1, .back = 2}"s};
    BOOST_TEST(ostr.str() == c1_str);
    {
        const auto c2{data_books::card{.front = "3", .back = "4"}};
        const auto c2_str{"card{.front = 3, .back = 4}"s};
        const auto cs{data_books::cards{{c1, c2}}};
        const auto cs_str{"[" + c1_str + ", " + c2_str + "]"};
        std::ostringstream ostr{};
        ostr << cs;
        BOOST_TEST(ostr.str() == cs_str);
        {
            const auto ch1{data_books::chapter{.name = "ch1", .cards = cs}};
            const auto ch1_str{"chapter{.name = ch1, .cards = " + cs_str + "}"};
            std::ostringstream ostr{};
            ostr << ch1;
            BOOST_TEST(ostr.str() == ch1_str);
            {
                const auto ch2{data_books::chapter{.name = "ch2", .cards = cs}};
                const auto ch2_str{"chapter{.name = ch2, .cards = " + cs_str +
                                   "}"};
                const auto chs{data_books::chapters{{ch1, ch2}}};
                const auto chs_str{"[" + ch1_str + ", " + ch2_str + "]"};
                std::ostringstream ostr{};
                ostr << chs;
                BOOST_TEST(ostr.str() == chs_str);
                {
                    const auto b1{
                        data_books::book{.name = "b1", .chapters = chs}};
                    const auto b1_str{
                        "book{.name = b1, .chapters = " + chs_str + "}"};
                    std::ostringstream ostr{};
                    ostr << b1;
                    BOOST_TEST(ostr.str() == b1_str);
                    {
                        const auto b2{
                            data_books::book{.name = "b2", .chapters = chs}};
                        const auto b2_str{
                            "book{.name = b2, .chapters = " + chs_str + "}"};
                        const auto bs{data_books::books{{b1, b2}}};
                        const auto bs_str{"[" + b1_str + ", " + b2_str + "]"};
                        std::ostringstream ostr{};
                        ostr << bs;
                        BOOST_TEST(ostr.str() == bs_str);
                        {
                            const auto t_done{data_training::training{
                                .state = data_training::states::done{},
                                .cards = cs}};
                            const auto t_done_str{
                                "training{.state = done{}, .cards = " + cs_str +
                                "}"};
                            {
                                std::ostringstream ostr{};
                                ostr << t_done;
                                BOOST_TEST(ostr.str() == t_done_str);
                            }
                            const auto t_show_card{data_training::training{
                                .state = data_training::states::show_card{c1},
                                .cards = cs}};
                            const auto t_show_card_str{
                                "training{.state = show_card{.card = " +
                                c1_str + "}, .cards = " + cs_str + "}"};
                            {
                                std::ostringstream ostr{};
                                ostr << t_show_card;
                                BOOST_TEST(ostr.str() == t_show_card_str);
                            }
                            {
                                const auto d{
                                    data::data{.the_books = bs,
                                               .the_training = t_done}};
                                const auto d_str{
                                    "data{.the_books = " + bs_str +
                                    ", .the_training = " + t_done_str + "}"};
                                std::ostringstream ostr{};
                                ostr << d;
                                BOOST_TEST(ostr.str() == d_str);
                            }
                        }
                    }
                }
            }
        }
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
