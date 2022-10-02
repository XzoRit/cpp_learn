#include <voc/voc.hpp>

#include <boost/test/unit_test.hpp>

namespace
{
BOOST_AUTO_TEST_SUITE(voc_tests)

BOOST_AUTO_TEST_CASE(vocabel)
{
    using vocabel = xzr::voc::vocabel;

    vocabel a{};
    a.de = "";
    a.en = "";
}

BOOST_AUTO_TEST_SUITE_END()
}
