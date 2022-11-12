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
    o << "[ ";
    for (auto&& it : v)
        o << it << ' ';
    o << ']';

    return o;
}
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

using cards = container<card>;

struct chapter
{
    std::string name{};
    cards cards{};

    [[nodiscard]] auto operator<=>(const chapter&) const = default;
};

using chapters = container<chapter>;

struct book
{
    std::string name{};
    chapters chapters{};

    [[nodiscard]] auto operator<=>(const book&) const = default;
};

using books = container<book>;
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
XZR_LEARN_DATA_OSTREAM_FOR(chapter)
XZR_LEARN_DATA_OSTREAM_FOR(book)
}
}
