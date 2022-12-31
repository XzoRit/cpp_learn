#pragma once

#include <ostream>
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
