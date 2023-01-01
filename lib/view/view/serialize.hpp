#pragma once

#include <view/data.hpp>
#include <view/reflect.hpp>

#include <data/serialize.hpp>
#include <data/serialize_generic.hpp>
#include <data/serialize_std_variant.hpp>

#include <boost/serialization/nvp.hpp>

namespace xzr::learn::view::states
{
inline auto serialize(auto& ar, books& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, add_book& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, book& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, add_chapter& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, chapter& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, add_card_front& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, add_card_back& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, training& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
}
namespace xzr::learn::view
{
inline auto serialize(auto& ar, data& d, const unsigned int v)
{
    generic::serialize(ar, d, v);
}
}
