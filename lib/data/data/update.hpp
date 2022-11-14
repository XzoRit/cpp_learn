#pragma once

#include <data/action.hpp>
#include <data/app.hpp>

namespace xzr::learn::update
{
inline namespace v1
{
auto update(data::app a, action::action act) -> data::app;
}
}
