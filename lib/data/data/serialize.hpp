#pragma once

#include <data/books.hpp>
#include <data/data.hpp>

#include <boost/pfr/core.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace boost::serialization
{
template <class A>
inline auto serialize(auto& ar,
                      ::xzr::learn::data::books::v1::container<A>& a,
                      const unsigned int) -> void
{
    ar& a.base();
}
}
#define XZR_LEARN_DATA_SERIALIZE_FOR(A)                                        \
    namespace boost::serialization                                             \
    {                                                                          \
    inline auto serialize(auto& ar, A& a, const unsigned int)                  \
    {                                                                          \
        ::boost::pfr::for_each_field(a, [&ar](auto& field) { ar& field; });    \
    }                                                                          \
    }

XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::books::book)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::books::card)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::books::chapter)
namespace boost::serialization
{
inline auto serialize(auto& ar, ::xzr::learn::data::app& a, const unsigned int)
{
    ar& a.the_books;
}
}
namespace xzr::learn::data
{
inline auto serialize(auto& ar, app& a)
{
    ar& a;
}
inline auto save(auto& ar, const app& a)
{
    ar& a;
}
inline auto load(auto& ar)
{
    app a{};
    ar& a;
    return a;
}
}
