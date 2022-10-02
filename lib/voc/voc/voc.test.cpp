#include <voc/voc.hpp>

#include <boost/test/unit_test.hpp>

namespace
{
BOOST_AUTO_TEST_SUITE(voc_tests)

BOOST_AUTO_TEST_CASE(vocabel_default_ctor)
{
    using vocabel = xzr::voc::vocabel;

    vocabel a{};
    BOOST_TEST(a.de == "");
    BOOST_TEST(a.en == "");
}

BOOST_AUTO_TEST_CASE(vocabel_initialized_ctor)
{
    using vocabel = xzr::voc::vocabel;

    vocabel a{.de = "Hallo", .en = "Hello"};
    BOOST_TEST(a.de == "Hallo");
    BOOST_TEST(a.en == "Hello");
}

BOOST_AUTO_TEST_CASE(vocabels_ctor)
{
    using vocabels = xzr::voc::vocabels;

    vocabels a{};
    BOOST_TEST(a.empty());
    BOOST_TEST(a.size() == 0u);
    BOOST_TEST(a.chapter == "");
}

BOOST_AUTO_TEST_CASE(add_vocabels)
{
    // using vocabel = xzr::voc::vocabel;
    using vocabels = xzr::voc::vocabels;

    vocabels a{.chapter = "ch_01"};
    a.add({.de = "Hallo", .en = "Hello"});
    BOOST_TEST(!a.empty());
    BOOST_TEST(a.size() == 1u);
}

BOOST_AUTO_TEST_SUITE_END()
}
