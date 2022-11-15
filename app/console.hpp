#include <data/app.hpp>

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
auto run(data::app app_data) -> void;
}
}
