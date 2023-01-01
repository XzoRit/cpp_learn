#pragma once

#include <view/data.hpp>

namespace xzr::learn::persist
{
auto save(const view::data& data) -> void;
[[nodiscard]] auto load() -> view::data;
[[nodiscard]] auto load_or_create_empty_data() -> view::data;
}
