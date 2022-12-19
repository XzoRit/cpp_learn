#pragma once

#include <data/books.hpp>
#include <data/training.hpp>

#include <variant>

namespace xzr::learn::data
{
inline namespace v1
{
namespace actions
{
using action = std::variant<books::actions::action>;
}
}
}
namespace xzr::learn::data
{
inline namespace v1
{
struct app
{
    books::books the_books{};
    training the_training{};
    [[nodiscard]] bool operator==(const app&) const = default;
};
}
}
namespace xzr::learn::data
{
inline namespace v1
{
auto update(app a, actions::action act) -> app;
}
}

namespace xzr::learn::data
{
namespace v2
{
namespace actions
{
using action = std::variant<books::actions::action, training::actions::action>;
};
}
}
namespace xzr::learn::data
{
namespace v2
{
struct app
{
    books::books the_books{};
    training::training the_training{};
};
}
}
namespace xzr::learn::data
{
namespace v2
{
auto update(app, actions::action) -> app;
}
}
