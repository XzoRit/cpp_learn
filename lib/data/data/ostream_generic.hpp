#pragma once

#include <boost/describe/operators.hpp>

#include <ostream>
#include <string_view>

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
