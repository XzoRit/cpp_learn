#pragma once

#include <data/books.hpp>
#include <data/training.hpp>

namespace xzr::learn::data
{
inline namespace v1
{
struct app
{
    books the_books{};
    training the_training{};
    [[nodiscard]] bool operator==(const app&) const = default;
};
}
}
