#pragma once

#include <data/books.hpp>

#include <string>
#include <variant>

namespace xzr::learn::data::training::actions
{
struct start
{
    books::cards cards{};
};
struct answer
{
    std::string txt{};
};
using action = std::variant<start, answer>;
}
namespace xzr::learn::data::training::states
{
struct done
{
};
struct show_card
{
    books::card card{};
};
using state = std::variant<done, show_card>;
}
namespace xzr::learn::data::training
{
struct training
{
    states::state state{};
    books::cards cards{};
    bool operator==(const training&) const noexcept = default;
};
[[nodiscard]] auto update(training, actions::action) -> training;
}
