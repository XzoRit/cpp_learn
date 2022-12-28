#pragma once

#include <data/data.hpp>

namespace xzr::learn::persist
{
auto save(const data::data& data) -> void;
[[nodiscard]] auto load() -> data::data;
[[nodiscard]] auto load_or_create_empty_data() -> data::data;
}
