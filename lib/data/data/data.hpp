#pragma once

#include <ostream>
#include <vector>

namespace xzr::learn::data
{
inline namespace v1
{
template <class A>
struct container : public std::vector<A>
{
  public:
    using std::vector<A>::vector;

    [[nodiscard]] auto base() const -> const std::vector<A>&
    {
        return *this;
    }

    [[nodiscard]] auto base() -> std::vector<A>&
    {
        return *this;
    }
};

template <class A>
inline auto operator<<(std::ostream& o, const container<A>& v) -> std::ostream&
{
    o << "{ ";
    for (auto&& it : v)
        o << it << ' ';
    o << '}';

    return o;
}
}
}

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

#include <string>

namespace xzr::learn::data
{
inline namespace v1
{
struct card
{
    std::string front{};
    std::string back{};

    [[nodiscard]] auto operator<=>(const card&) const = default;
};

struct cards
{
    std::string name{};
    container<card> content{};

    [[nodiscard]] auto operator<=>(const cards&) const = default;
};

struct package
{
    std::string name{};
    container<cards> content{};

    [[nodiscard]] auto operator<=>(const package&) const = default;
};
}
}

#include <boost/pfr/functions_for.hpp>

#include <ostream>

#define XZR_LEARN_DATA_OSTREAM_FOR(A)                                          \
    inline ::std::ostream& operator<<(::std::ostream& out, const A& value)     \
    {                                                                          \
        return out << ::boost::pfr::io_fields(value);                          \
    }

namespace xzr::learn::data
{
inline namespace v1
{
XZR_LEARN_DATA_OSTREAM_FOR(card)
XZR_LEARN_DATA_OSTREAM_FOR(cards)
XZR_LEARN_DATA_OSTREAM_FOR(package)
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

#include <boost/serialization/string.hpp>

XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::card)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::cards)
XZR_LEARN_DATA_SERIALIZE_FOR(::xzr::learn::data::package)
