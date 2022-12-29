#include <data/books.hpp>
#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/pfr/functions_for.hpp>

#include <ostream>
#include <variant>

#define XZR_LEARN_DATA_OSTREAM_FOR(A)                                          \
    inline ::std::ostream& operator<<(::std::ostream& out, const A& value)     \
    {                                                                          \
        return out << ::boost::pfr::io_fields(value);                          \
    }

namespace xzr::learn::data::books
{
XZR_LEARN_DATA_OSTREAM_FOR(card)
XZR_LEARN_DATA_OSTREAM_FOR(chapter)
XZR_LEARN_DATA_OSTREAM_FOR(book)
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
namespace xzr::learn::data::training::states
{
XZR_LEARN_DATA_OSTREAM_FOR(done)
XZR_LEARN_DATA_OSTREAM_FOR(show_card)
inline auto operator<<(std::ostream& o, const state& s) -> std::ostream&
{
    std::visit([&](const auto& a) { o << a; }, s);
    return o;
}
}
namespace xzr::learn::data::training
{
inline auto operator<<(std::ostream& o, const training& t) -> std::ostream&
{
    o << "{.state = " << t.state << ", .cards = " << t.cards << '}';
    return o;
}
}
namespace xzr::learn::data
{
XZR_LEARN_DATA_OSTREAM_FOR(data)
}
