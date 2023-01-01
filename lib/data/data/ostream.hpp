#pragma once

#include <data/books.hpp>
#include <data/data.hpp>
#include <data/ostream_std.hpp>
#include <data/training.hpp>

#include <ostream>

namespace xzr::learn::data::books::actions
{
auto operator<<(std::ostream& o, const add_book& a) -> std::ostream&;
auto operator<<(std::ostream& o, const remove_book& a) -> std::ostream&;
auto operator<<(std::ostream& o, const add_chapter& a) -> std::ostream&;
auto operator<<(std::ostream& o, const remove_chapter& a) -> std::ostream&;
auto operator<<(std::ostream& o, const add_card& a) -> std::ostream&;
auto operator<<(std::ostream& o, const remove_card& a) -> std::ostream&;
}
namespace xzr::learn::data::books
{
auto operator<<(std::ostream& o, const card& c) -> std::ostream&;
auto operator<<(std::ostream& o, const chapter& c) -> std::ostream&;
auto operator<<(std::ostream& o, const book& b) -> std::ostream&;
}
namespace xzr::learn::data::training::states
{
auto operator<<(std::ostream& o, const done& d) -> std::ostream&;
auto operator<<(std::ostream& o, const show_card& s) -> std::ostream&;
}
namespace xzr::learn::data::training::actions
{
auto operator<<(std::ostream& o, const start& a) -> std::ostream&;
auto operator<<(std::ostream& o, const answer& a) -> std::ostream&;
}
namespace xzr::learn::data::training
{
auto operator<<(std::ostream& o, const training& t) -> std::ostream&;
}
namespace xzr::learn::data
{
auto operator<<(std::ostream& o, const data& d) -> std::ostream&;
}
