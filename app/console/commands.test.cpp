#include <commands.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

namespace
{
using namespace std::string_literals;
using ::xzr::learn::console::commands::as_str;
using ::xzr::learn::console::commands::command;
using ::xzr::learn::console::commands::extract_id;
using ::xzr::learn::console::commands::is;
BOOST_AUTO_TEST_SUITE(console_tests)
BOOST_AUTO_TEST_SUITE(command_tests)
BOOST_AUTO_TEST_CASE(cmd_as_str)
{
    const auto cmd{command{.cmd = "abc", .desc = "123"}};
    const auto str{as_str(cmd)};
    BOOST_TEST(!str.empty());
    BOOST_TEST(str.starts_with("abc"));
    BOOST_TEST(str.ends_with("123"));
}
BOOST_AUTO_TEST_CASE(is_simple_cmd)
{
    const auto cmd{command{.cmd = "abc"}};
    BOOST_TEST(is("abc", cmd));
    BOOST_TEST(!is("a", cmd));
    BOOST_TEST(!is("ab", cmd));
    BOOST_TEST(!is("def", cmd));
}
BOOST_AUTO_TEST_CASE(is_num_cmd)
{
    const auto cmd{command{.cmd = "abc<n>"}};
    BOOST_TEST(is("abc123", cmd));
    BOOST_TEST(!is("a1", cmd));
    BOOST_TEST(!is("ab1", cmd));
    BOOST_TEST(!is("def123", cmd));
    BOOST_TEST(!is("abcd", cmd));
    BOOST_TEST(!is("abc²", cmd));
}
BOOST_AUTO_TEST_CASE(extract_valid_id)
{
    const auto cmd{"abc123"s};
    const auto id{extract_id(cmd)};
    BOOST_REQUIRE(id);
    BOOST_TEST(id.value() == 122);
}
BOOST_AUTO_TEST_CASE(extract_invalid_id)
{
    const auto cmd{"abc123d"s};
    const auto id{extract_id(cmd)};
    BOOST_REQUIRE(!id);
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
