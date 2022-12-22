#pragma once

#include "commands.hpp"

#include <data/books.hpp>

#include <iostream>
#include <string>

namespace xzr::learn::console::view
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
struct menu
{
    menu(const std::string& name)
    {
        println("");
        println("=== " + name);
    }
    ~menu()
    {
        println("===");
        print("-> ");
    }
    auto select() -> menu&
    {
        println(::xzr::learn::console::commands::as_str(
            ::xzr::learn::console::commands::select));
        return *this;
    }
    auto add() -> menu&
    {
        println(::xzr::learn::console::commands::as_str(
            ::xzr::learn::console::commands::add));
        return *this;
    }
    auto remove() -> menu&
    {
        println(::xzr::learn::console::commands::as_str(
            ::xzr::learn::console::commands::remove));
        return *this;
    }
    auto start_training() -> menu&
    {
        println(::xzr::learn::console::commands::as_str(
            ::xzr::learn::console::commands::start_training));
        return *this;
    }
    auto quit() -> menu&
    {
        println(::xzr::learn::console::commands::as_str(
            ::xzr::learn::console::commands::quit));
        return *this;
    }
    auto exit() -> menu&
    {
        println(::xzr::learn::console::commands::as_str(
            ::xzr::learn::console::commands::exit));
        return *this;
    }
};
struct content
{
    content()
    {
        println();
    }
    ~content()
    {
        println("===");
        println();
    }
    auto chapter(const ::xzr::learn::data::books::chapter& ch)
    {
        println("=== chapter: ", ch.name);
        println("cards:");
        for (int i{}; const auto& c : ch.cards)
        {
            println(++i, ".\t", c.front);
            println(i, ".\t", c.back);
        }
    }
    auto book(const ::xzr::learn::data::books::book& b)
    {
        println("=== book: ", b.name);
        println("chapters:");
        for (int i{}; const auto& c : b.chapters)
            println(++i, ".\t", c.name);
    }
    auto books(const ::xzr::learn::data::books::books& bs)
    {
        println("=== books:");
        for (int i{}; const auto& b : bs)
            println(++i, ".\t", b.name);
    }
};
}
