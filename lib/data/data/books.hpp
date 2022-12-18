#pragma once

#include <string>
#include <vector>

namespace xzr::learn::data::books
{
inline namespace v1
{
template <class A>
struct container : public std::vector<A>
{
  public:
    using std::vector<A>::vector;

    [[nodiscard]] auto base() const -> const std::vector<A>&
    {
        return *this;
    }

    [[nodiscard]] auto base() -> std::vector<A>&
    {
        return *this;
    }
};

struct card
{
    std::string front{};
    std::string back{};

    [[nodiscard]] auto operator<=>(const card&) const = default;
};

using cards = container<card>;

struct chapter
{
    std::string name{};
    cards cards{};

    [[nodiscard]] auto operator<=>(const chapter&) const = default;
};

using chapters = container<chapter>;

struct book
{
    std::string name{};
    chapters chapters{};

    [[nodiscard]] auto operator<=>(const book&) const = default;
};

using books = container<book>;
}
}
