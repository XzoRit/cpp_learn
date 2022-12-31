#include <data/ostream.hpp>

#include <data/ostream_generic.hpp>
#include <data/reflect.hpp>

namespace xzr::learn::data::books
{
auto operator<<(std::ostream& o, const card& c) -> std::ostream&
{
    return generic::operator<<(o, c);
}
auto operator<<(std::ostream& o, const chapter& c) -> std::ostream&
{
    return generic::operator<<(o, c);
}
auto operator<<(std::ostream& o, const book& b) -> std::ostream&
{
    return generic::operator<<(o, b);
}
}
namespace xzr::learn::data::training::states
{
auto operator<<(std::ostream& o, const done& d) -> std::ostream&
{
    return generic::operator<<(o, d);
}
auto operator<<(std::ostream& o, const show_card& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
}
namespace xzr::learn::data::training
{
auto operator<<(std::ostream& o, const training& t) -> std::ostream&
{
    return generic::operator<<(o, t);
}
}
namespace xzr::learn::data
{
auto operator<<(std::ostream& o, const data& d) -> std::ostream&
{
    return generic::operator<<(o, d);
}
}
