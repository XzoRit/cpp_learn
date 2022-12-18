#pragma once

#include <data/action.hpp>
#include <data/app.hpp>

namespace xzr::learn::data
{
inline namespace v1
{
auto update(app a, action act) -> app;
}
namespace v2
{
auto update(app a, action act) -> app;
}
}
