#include <data/ostream_generic.hpp>
#include <data/ostream_std.hpp>
#include <data/serialize_generic.hpp>
#include <data/serialize_std_variant.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/describe/class.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/test/unit_test.hpp>

#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace
{
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_SUITE(serialize_generic_tests)
struct serializeable
{
    std::variant<std::string, int> var{};
    bool operator==(const serializeable&) const = default;
};
BOOST_DESCRIBE_STRUCT(serializeable, (), (var))
auto operator<<(std::ostream& o, const serializeable& s) -> std::ostream&
{
    return ::xzr::learn::generic::operator<<(o, s);
}
auto serialize(auto& ar, serializeable& s, const unsigned int v) -> void
{
    ::xzr::learn::generic::serialize(ar, s, v);
}
BOOST_AUTO_TEST_CASE(serialization)
{
    const auto a{serializeable{.var = 1}};
    {
        auto ss{std::stringstream{}};
        {
            auto oa{::boost::archive::text_oarchive{ss}};
            oa << a;
        }
        auto b{serializeable{}};
        {
            auto ia{::boost::archive::text_iarchive{ss}};
            ia >> b;
        }
        BOOST_TEST(a == b);
    }
    {
        auto ss{std::stringstream{}};
        {
            auto oa{::boost::archive::binary_oarchive{ss}};
            oa << a;
        }
        auto b{serializeable{}};
        {
            auto ia{::boost::archive::binary_iarchive{ss}};
            ia >> b;
        }
        BOOST_TEST(a == b);
    }
    {
        auto ss{std::stringstream{}};
        {
            auto oa{::boost::archive::xml_oarchive{ss}};
            oa << ::boost::serialization::make_nvp("serializeable", a);
        }
        auto b{serializeable{}};
        {
            auto ia{::boost::archive::xml_iarchive{ss}};
            ia >> ::boost::serialization::make_nvp("serializeable", b);
        }
        BOOST_TEST(a == b);
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
