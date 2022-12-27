#pragma once

#include <data/books.hpp>
#include <view/model.hpp>

#include <iostream>
#include <string>

namespace xzr::learn::console
{
inline auto print(auto&&... txt)
{
    (std::cout << ... << txt);
}
inline auto println(auto&&... txt)
{
    print(txt..., '\n');
}
[[nodiscard]] inline auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
auto intent(const std::string& cmd_str) -> view::model::actions::action;
auto draw(const view::model::data view_model) -> void;
}
