#pragma once

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/throw_exception.hpp>

#include <boost/archive/archive_exception.hpp>

#include <variant>

namespace boost
{
namespace serialization
{
template <class Archive>
struct std_variant_save_visitor
{
    std_variant_save_visitor(Archive& ar)
        : m_ar(ar)
    {
    }
    template <class T>
    void operator()(T const& value) const
    {
        m_ar << BOOST_SERIALIZATION_NVP(value);
    }

  private:
    Archive& m_ar;
};
template <class Archive>
struct std_variant_load_visitor
{
    std_variant_load_visitor(Archive& ar)
        : m_ar(ar)
    {
    }
    template <class T>
    void operator()(T& value) const
    {
        m_ar >> BOOST_SERIALIZATION_NVP(value);
    }

  private:
    Archive& m_ar;
};
template <class Archive, class... Types>
void save(Archive& ar, std::variant<Types...> const& v, unsigned int /*version*/
)
{
    const std::size_t which = v.index();
    ar << BOOST_SERIALIZATION_NVP(which);
    std_variant_save_visitor<Archive> visitor(ar);
    std::visit(visitor, v);
}
namespace mp
{
namespace detail
{
template <typename Seq>
struct front_impl;

template <template <typename...> class Seq, typename T, typename... Ts>
struct front_impl<Seq<T, Ts...>>
{
    using type = T;
};
template <typename Seq>
struct pop_front_impl;
template <template <typename...> class Seq, typename T, typename... Ts>
struct pop_front_impl<Seq<T, Ts...>>
{
    using type = Seq<Ts...>;
};
}
template <typename... Ts>
struct typelist
{
};
template <typename Seq>
using front = typename detail::front_impl<Seq>::type;
template <typename Seq>
using pop_front = typename detail::pop_front_impl<Seq>::type;
}
template <std::size_t N, class Seq>
struct variant_impl
{
    template <class Archive, class V>
    static void load(Archive& ar,
                     std::size_t which,
                     V& v,
                     const unsigned int version)
    {
        if (which == 0)
        {
            using type = mp::front<Seq>;
            type value;
            ar >> BOOST_SERIALIZATION_NVP(value);
            v = std::move(value);
            type* new_address = &std::get<type>(v);
            ar.reset_object_address(new_address, &value);
            return;
        }
        using types = mp::pop_front<Seq>;
        variant_impl<N - 1, types>::load(ar, which - 1, v, version);
    }
};

template <class Seq>
struct variant_impl<0, Seq>
{
    template <class Archive, class V>
    static void load(Archive&, std::size_t, V&, const unsigned int)
    {
    }
};
template <class Archive, class... Types>
void load(Archive& ar, std::variant<Types...>& v, const unsigned int version)
{
    std::size_t which;
    ar >> BOOST_SERIALIZATION_NVP(which);
    if (which >= sizeof...(Types))
        boost::serialization::throw_exception(boost::archive::archive_exception(
            boost::archive::archive_exception::unsupported_version));
    variant_impl<sizeof...(Types), mp::typelist<Types...>>::load(ar,
                                                                 which,
                                                                 v,
                                                                 version);
}
template <class Archive, class... Types>
inline void serialize(Archive& ar,
                      std::variant<Types...>& v,
                      const unsigned int file_version)
{
    split_free(ar, v, file_version);
}
template <class Archive>
void serialize(Archive& ar, std::monostate&, const unsigned int /*version*/)
{
}
}
}

#include <boost/serialization/tracking.hpp>

namespace boost
{
namespace serialization
{
template <class... Types>
struct tracking_level<std::variant<Types...>>
{
    typedef mpl::integral_c_tag tag;
    typedef mpl::int_<::boost::serialization::track_always> type;
    BOOST_STATIC_CONSTANT(int, value = type::value);
};
}
}
