#pragma once

#include <boost/describe/bases.hpp>
#include <boost/describe/members.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/serialization/nvp.hpp>

#include <type_traits>
#include <variant>

namespace xzr::learn::generic
{
template <
    class A,
    class BaseDescr =
        ::boost::describe::describe_bases<A, ::boost::describe::mod_any_access>,
    class MemberDescr = ::boost::describe::
        describe_members<A, ::boost::describe::mod_any_access>,
    class En = std::enable_if_t<!std::is_union<A>::value>>
inline auto serialize(auto& ar, A& a, const unsigned int)
{
    // ::boost::mp11::mp_for_each<BaseDescr>([&](auto base) {
    //     using base_t = typename decltype(base)::type;
    //     ar&(base_t&)a;
    // });
    ::boost::mp11::mp_for_each<MemberDescr>([&](auto member) {
        ar& ::boost::serialization::make_nvp(member.name, a.*member.pointer);
    });
}
}
