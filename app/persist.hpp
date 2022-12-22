#pragma once

#include <data/data.hpp>

namespace xzr::learn::persist
{
auto save(const ::xzr::learn::data::data& data) -> void;
[[nodiscard]] auto load() -> ::xzr::learn::data::data;
[[nodiscard]] auto load_or_create_empty_data() -> ::xzr::learn::data::data;
}
