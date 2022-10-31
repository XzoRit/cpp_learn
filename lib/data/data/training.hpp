#pragma once

#include <data/data.hpp>

#include <optional>
#include <string_view>

namespace xzr::learn::data
{
inline namespace v1
{
struct training
{
    cards cards{};
};
[[nodiscard]] auto start_training(cards cs) -> training;
[[nodiscard]] auto current_card(const training& t) -> std::optional<card>;
[[nodiscard]] auto eval_answer(training t, card crd, std::string_view back)
    -> training;
}
}
