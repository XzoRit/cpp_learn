#include <data/app.hpp>
#include <data/data.hpp>

#include <iostream>

namespace xzr::learn::console
{
inline namespace v1
{
inline auto print(auto&&... txt)
{
    (std::cout << ... << txt);
}
inline auto println(auto&&... txt)
{
    print(txt..., '\n');
}
auto render(const data::app& app_data) -> void;
auto render_books(const data::books& bs) -> void;
auto render_chapters(const data::chapters& cs) -> void;
auto render_cards(const data::cards& cs) -> void;
}
}
