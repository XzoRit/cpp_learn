#include "console.hpp"

namespace xzr::learn::console
{
inline namespace v1
{
auto render(const data::app& app_data) -> void
{
    render_books(app_data.the_books);
}
auto render_books(const data::books& bs) -> void
{
    for (int i{}; const auto& b : bs)
        println(++i, ".\t", b.name);
}
auto render_chapters(const data::chapters& cs) -> void
{
    for (int i{}; const auto& c : cs)
        println(++i, ".\t", c.name);
}
auto render_cards(const data::cards& cs) -> void
{
    for (const auto& c : cs)
    {
        println("front:\t", c.front);
        println("back:\t", c.back);
        println();
    }
}
}
}
