// std::string_view xzr::learn::generic::type_to_str() [T =
// xzr::learn::data::books::card]
#include <data/books.hpp>
#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/describe/class.hpp>
#include <boost/describe/operators.hpp>

#include <ostream>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace std
{
template <class... A>
auto operator<<(ostream& o, const variant<A...>& v) -> ostream&;
template <class A, class Alloc>
auto operator<<(ostream& o, const vector<A, Alloc>& v) -> ostream&;
template <class... A>
inline auto operator<<(ostream& o, const variant<A...>& v) -> ostream&
{
    std::visit([&](const auto& a) { o << a; }, v);
    return o;
}
template <class A, class Alloc>
inline auto operator<<(ostream& o, const vector<A, Alloc>& v) -> ostream&
{
    o << "[";
    const auto* sep{""};
    for (auto&& it : v)
    {
        o << exchange(sep, ", ");
        o << it;
    }
    o << ']';
    return o;
}
}
namespace xzr::learn::generic
{
template <class A>
[[nodiscard]] auto full_type_name() -> std::string_view
{
#if defined(_MSC_VER) and not defined(__clang__)
    return {&__FUNCSIG__[100], sizeof(__FUNCSIG__) - 108};
#elif defined(__clang__)
    return {std::string_view{&__PRETTY_FUNCTION__[60],
                             sizeof(__PRETTY_FUNCTION__) - 62}};
#elif defined(__GNUC__)
    return {&__PRETTY_FUNCTION__[65], sizeof(__PRETTY_FUNCTION__) - 116};
#else
    return "";
#endif
}
template <class A>
[[nodiscard]] auto type_name() -> std::string_view
{
    const auto fn{full_type_name<A>()};
    return fn.substr(fn.find_last_of(':') + 1);
}
template <class A>
inline auto operator<<(std::ostream& o, const A& a) -> std::ostream&
{
    std::operator<<(o, type_name<A>());
    return ::boost::describe::operators::operator<<(o, a);
}
}
namespace xzr::learn::data::books
{
BOOST_DESCRIBE_STRUCT(card, (), (front, back))
BOOST_DESCRIBE_STRUCT(chapter, (), (name, cards))
BOOST_DESCRIBE_STRUCT(book, (), (name, chapters))
inline auto operator<<(std::ostream& o, const card& c) -> std::ostream&
{
    return generic::operator<<(o, c);
}
inline auto operator<<(std::ostream& o, const chapter& c) -> std::ostream&
{
    return generic::operator<<(o, c);
}
inline auto operator<<(std::ostream& o, const book& b) -> std::ostream&
{
    return generic::operator<<(o, b);
}
}
namespace xzr::learn::data::training::states
{
BOOST_DESCRIBE_STRUCT(done, (), ())
BOOST_DESCRIBE_STRUCT(show_card, (), (card))
inline auto operator<<(std::ostream& o, const done& d) -> std::ostream&
{
    return generic::operator<<(o, d);
}
inline auto operator<<(std::ostream& o, const show_card& s) -> std::ostream&
{
    return generic::operator<<(o, s);
}
}
namespace xzr::learn::data::training
{
BOOST_DESCRIBE_STRUCT(training, (), (state, cards))
inline auto operator<<(std::ostream& o, const training& t) -> std::ostream&
{
    return generic::operator<<(o, t);
}
}
namespace xzr::learn::data
{
BOOST_DESCRIBE_STRUCT(data, (), (the_books, the_training))
inline auto operator<<(std::ostream& o, const data& d) -> std::ostream&
{
    return generic::operator<<(o, d);
}
}
