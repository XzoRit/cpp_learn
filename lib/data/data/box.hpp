#pragma once

#include <data/data.hpp>

#include <string>

namespace xzr::learn::data
{
inline namespace v1
{
class training
{
  public:
    explicit training(cards c);

    auto has_next() const -> bool;
    auto next() const -> const card&;

    auto move_card(const card& c, const std::string& back) -> void;

  private:
    auto shuffle() -> void;

    cards cs{};
};
}
}
