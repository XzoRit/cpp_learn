#include <data/app.hpp>
#include <data/data.hpp>
#include <data/training.hpp>

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
XZR_LEARN_DATA_OSTREAM_FOR(app)
XZR_LEARN_DATA_OSTREAM_FOR(card)
XZR_LEARN_DATA_OSTREAM_FOR(chapter)
XZR_LEARN_DATA_OSTREAM_FOR(book)
XZR_LEARN_DATA_OSTREAM_FOR(training)

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
