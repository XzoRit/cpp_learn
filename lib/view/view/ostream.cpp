#include <view/ostream.hpp>

#include <view/reflect.hpp>

#include <data/ostream.hpp>
#include <data/ostream_generic.hpp>

namespace xzr::learn::view::actions
{
auto operator<<(std::ostream& o, const select& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
auto operator<<(std::ostream& o, const add& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
auto operator<<(std::ostream& o, const remove& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
auto operator<<(std::ostream& o, const text_input& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
auto operator<<(std::ostream& o, const start_training& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
auto operator<<(std::ostream& o, const quit& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
auto operator<<(std::ostream& o, const exit& a) -> std::ostream&
{
    return generic::operator<<(o, a);
}
}
namespace xzr::learn::view::states
{
auto operator<<(std::ostream& o, const books& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const add_book& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const book& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const add_chapter& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const chapter& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const add_card_front& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const add_card_back& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
auto operator<<(std::ostream& o, const training& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
}
namespace xzr::learn::view
{
auto operator<<(std::ostream& o, const data& d) -> std::ostream&
{
    return generic::operator<<(o, d);
}
}
