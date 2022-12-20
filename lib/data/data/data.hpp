#pragma once

#include <data/books.hpp>
#include <data/training.hpp>

#include <variant>

namespace xzr::learn::data::actions
{
using action = std::variant<books::actions::action, training::actions::action>;
}
namespace xzr::learn::data
{
struct app
{
    books::books the_books{};
    training::training the_training{};
};
auto update(app, actions::action) -> app;
}
