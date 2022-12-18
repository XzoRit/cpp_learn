#pragma once

#include <data/app.hpp>

namespace xzr::learn::data
{
inline namespace v1
{
auto update(app a, books::actions::action act) -> app;
}
namespace v2
{
auto update(app a, books::actions::action act) -> app;
}
}
