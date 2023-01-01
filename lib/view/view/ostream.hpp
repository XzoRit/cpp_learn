#pragma once

#include <view/data.hpp>

#include <data/ostream_std.hpp>

#include <ostream>

namespace xzr::learn::view::actions
{
auto operator<<(std::ostream& o, const select& a) -> std::ostream&;
auto operator<<(std::ostream& o, const add& a) -> std::ostream&;
auto operator<<(std::ostream& o, const remove& a) -> std::ostream&;
auto operator<<(std::ostream& o, const text_input& a) -> std::ostream&;
auto operator<<(std::ostream& o, const start_training& a) -> std::ostream&;
auto operator<<(std::ostream& o, const quit& a) -> std::ostream&;
auto operator<<(std::ostream& o, const exit& a) -> std::ostream&;
}
namespace xzr::learn::view::states
{
auto operator<<(std::ostream& o, const books& s) -> std::ostream&;
auto operator<<(std::ostream& o, const add_book& s) -> std::ostream&;
auto operator<<(std::ostream& o, const book& s) -> std::ostream&;
auto operator<<(std::ostream& o, const add_chapter& s) -> std::ostream&;
auto operator<<(std::ostream& o, const chapter& s) -> std::ostream&;
auto operator<<(std::ostream& o, const add_card_front& s) -> std::ostream&;
auto operator<<(std::ostream& o, const add_card_back& s) -> std::ostream&;
auto operator<<(std::ostream& o, const training& s) -> std::ostream&;
}
namespace xzr::learn::view
{
auto operator<<(std::ostream& o, const data& d) -> std::ostream&;
}
