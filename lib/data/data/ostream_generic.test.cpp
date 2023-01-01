#include <data/ostream_generic.hpp>
#include <data/ostream_std.hpp>

#include <boost/describe/class.hpp>

#include <boost/test/unit_test.hpp>

#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_SUITE(ostream_generic_tests)
struct classic_streamable
{
    int value{};
};
auto operator<<(std::ostream& o, const classic_streamable& c) -> std::ostream&
{
    o << "classic_streamable{.value = " << c.value << '}';
    return o;
}
using stl_type =
    std::variant<int, std::optional<std::vector<classic_streamable>>>;
struct generic_streamable
{
    std::string name{};
    stl_type stl{};
};
BOOST_DESCRIBE_STRUCT(generic_streamable, (), (name, stl))
auto operator<<(std::ostream& o, const generic_streamable& g) -> std::ostream&
{
    return ::xzr::learn::generic::operator<<(o, g);
}
BOOST_AUTO_TEST_CASE(streaming)
{
    auto a{generic_streamable{.name = "a", .stl = 1}};
    {
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() == "generic_streamable{.name = a, .stl = 1}");
    }
    a.stl = std::nullopt;
    {
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(ostr.str() ==
                   "generic_streamable{.name = a, .stl = nullopt}");
    }
    a.stl = std::vector{classic_streamable{.value = 0},
                        classic_streamable{.value = 1}};
    {
        std::ostringstream ostr{};
        ostr << a;
        BOOST_TEST(
            ostr.str() ==
            "generic_streamable{.name = a, .stl = [classic_streamable{.value = "
            "0}, classic_streamable{.value = 1}]}");
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
