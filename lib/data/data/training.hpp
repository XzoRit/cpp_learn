#pragma once

#include <data/books.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace xzr::learn::data
{
inline namespace v1
{
struct training
{
    cards cards{};

    [[nodiscard]] bool operator==(const training&) const = default;
};

[[nodiscard]] auto start_training(cards cs) -> training;
[[nodiscard]] auto current_card(const training& t) -> std::optional<card>;
[[nodiscard]] auto eval_answer(training t, card crd, std::string_view back)
    -> training;
}
}

#include <variant>

namespace xzr::learn::data
{
namespace v2
{
namespace training
{
namespace action
{
struct start
{
    cards cards{};
};
struct answer
{
    std::string txt{};
};
using action = std::variant<start, answer>;
}
namespace states
{
struct done
{
};
struct show_card
{
    card card{};
};
using state = std::variant<done, show_card>;
}
struct training
{
    states::state state{};
    cards cards{};
};
[[nodiscard]] auto update(training, action::action) -> training;
}
}
}
