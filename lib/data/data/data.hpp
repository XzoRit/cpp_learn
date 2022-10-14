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
    auto base() const -> const std::vector<A>&
    {
        return *this;
    }
    auto base() -> std::vector<A>&
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

#include <boost/pfr/functions_for.hpp>

#define XZR_LEARN_DATA_OPS_FOR(T)                                              \
    BOOST_PFR_MAYBE_UNUSED inline bool operator==(const T& lhs, const T& rhs)  \
    {                                                                          \
        return ::boost::pfr::eq_fields(lhs, rhs);                              \
    }                                                                          \
    BOOST_PFR_MAYBE_UNUSED inline bool operator!=(const T& lhs, const T& rhs)  \
    {                                                                          \
        return ::boost::pfr::ne_fields(lhs, rhs);                              \
    }                                                                          \
    BOOST_PFR_MAYBE_UNUSED inline bool operator<(const T& lhs, const T& rhs)   \
    {                                                                          \
        return ::boost::pfr::lt_fields(lhs, rhs);                              \
    }                                                                          \
    BOOST_PFR_MAYBE_UNUSED inline bool operator>(const T& lhs, const T& rhs)   \
    {                                                                          \
        return ::boost::pfr::gt_fields(lhs, rhs);                              \
    }                                                                          \
    BOOST_PFR_MAYBE_UNUSED inline bool operator<=(const T& lhs, const T& rhs)  \
    {                                                                          \
        return ::boost::pfr::le_fields(lhs, rhs);                              \
    }                                                                          \
    BOOST_PFR_MAYBE_UNUSED inline bool operator>=(const T& lhs, const T& rhs)  \
    {                                                                          \
        return ::boost::pfr::ge_fields(lhs, rhs);                              \
    }                                                                          \
    template <class Char, class Traits>                                        \
    BOOST_PFR_MAYBE_UNUSED inline ::std::basic_ostream<Char, Traits>&          \
    operator<<(::std::basic_ostream<Char, Traits>& out, const T& value)        \
    {                                                                          \
        return out << ::boost::pfr::io_fields(value);                          \
    }

#include <string>
#include <vector>

namespace xzr::learn::data
{
inline namespace v1
{
struct card
{
    std::string front{};
    std::string back{};
};

XZR_LEARN_DATA_OPS_FOR(card)

struct cards
{
    std::string name{};
    container<card> content{};
};

XZR_LEARN_DATA_OPS_FOR(cards)

struct package
{
    std::string name{};
    container<cards> content{};
};

XZR_LEARN_DATA_OPS_FOR(package)
}
}

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace boost::serialization
{
template <class Ar, class A>
inline auto serialize(Ar& a,
                      ::xzr::learn::data::v1::container<A>& c,
                      const unsigned int) -> void
{
    a& c.base();
}
template <class Ar>
inline auto serialize(Ar& a,
                      ::xzr::learn::data::v1::card& c,
                      const unsigned int) -> void
{
    a& c.front;
    a& c.back;
}
template <class Ar>
inline auto serialize(Ar& a,
                      ::xzr::learn::data::v1::cards& cs,
                      const unsigned int) -> void
{
    a& cs.name;
    a& cs.content;
}
template <class Ar>
inline auto serialize(Ar& a,
                      ::xzr::learn::data::v1::package& p,
                      const unsigned int) -> void
{
    a& p.name;
    a& p.content;
}
}
