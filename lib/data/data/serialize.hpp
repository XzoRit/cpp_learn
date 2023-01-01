#pragma once

#include <data/books.hpp>
#include <data/data.hpp>
#include <data/reflect.hpp>
#include <data/serialize_generic.hpp>
#include <data/serialize_std_variant.hpp>
#include <data/training.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include <type_traits>
#include <variant>

namespace xzr::learn::data::books
{
inline auto serialize(auto& ar, card& c, const unsigned int v)
{
    generic::serialize(ar, c, v);
}
inline auto serialize(auto& ar, chapter& c, const unsigned int v)
{
    generic::serialize(ar, c, v);
}
inline auto serialize(auto& ar, book& b, const unsigned int v)
{
    generic::serialize(ar, b, v);
}
}
namespace xzr::learn::data::training::states
{
inline auto serialize(auto& ar, done& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
inline auto serialize(auto& ar, show_card& s, const unsigned int v)
{
    generic::serialize(ar, s, v);
}
}
namespace xzr::learn::data::training
{
inline auto serialize(auto& ar, training& t, const unsigned int v)
{
    generic::serialize(ar, t, v);
}
}
namespace xzr::learn::data
{
inline auto serialize(auto& ar, data& d, const unsigned int v)
{
    generic::serialize(ar, d, v);
}
}
