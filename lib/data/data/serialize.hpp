#pragma once

#include <data/data.hpp>

#include <boost/serialization/vector.hpp>

namespace boost::serialization
{
template <class Ar, class A>
inline auto serialize(Ar& ar,
                      ::xzr::learn::data::v1::container<A>& a,
                      const unsigned int) -> void
{
    ar& a.base();
}
}

#include <boost/pfr/core.hpp>

#define XZR_LEARN_DATA_SERIALIZE_FOR(A)                                        \
    namespace boost::serialization                                             \
    {                                                                          \
    template <class Ar>                                                        \
    inline auto serialize(Ar& ar, A& a, const unsigned int)                    \
    {                                                                          \
        ::boost::pfr::for_each_field(a, [&ar](auto& field) { ar& field; });    \
    }                                                                          \
    }

#include <data/app.hpp>
#include <data/training.hpp>

#include <boost/serialization/string.hpp>

XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::app)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::book)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::card)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::chapter)

namespace xzr::learn::data
{
inline namespace v1
{
inline auto serialize(auto& ar, app& a) -> void
{
    ar& a;
}
}
}
