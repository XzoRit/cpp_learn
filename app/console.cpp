#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/update.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/hof/match.hpp>
#include <boost/hof/result.hpp>

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using ::xzr::learn::console::println;

namespace
{
namespace persist
{
namespace fs = std::filesystem;

const auto books_path{fs::path{"xzr_learn_books.txt"}};

auto save(const ::xzr::learn::data::app& app_data)
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};
    save(o, app_data);
}
[[nodiscard]] auto load()
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};
    return load(i);
}
[[nodiscard]] auto read_or_create_app_data()
{
    if (!fs::exists(books_path))
        save(::xzr::learn::data::app{});
    return load();
}
}
}

namespace
{
namespace console
{
[[nodiscard]] auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
struct state
{
    std::optional<int> book_id{};
};
struct data
{
    std::optional<::xzr::learn::data::action> data_act{};
    state console_state{};
};
auto draw_book(const ::xzr::learn::data::book& book, int book_id, state s)
    -> ::console::data
{
    println("name: ", book.name);
    for (int i{}; const auto& c : book.chapters)
        println(++i, ".\t", c.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");

    const auto& book_cmd{readln()};
    if (book_cmd == "b")
    {
        println("add chapter");
        println("name: ");
        return {.data_act = ::xzr::learn::data::add_chapter{.book_id = book_id,
                                                            .name = readln()},
                .console_state = s};
    }
    else if (book_cmd == "d")
        s.book_id.reset();
    return {.data_act = std::nullopt, .console_state = s};
}
[[nodiscard]] auto draw(const ::xzr::learn::data::app& app_data, state s)
    -> ::console::data
{

    if (s.book_id)
    {
        return draw_book(app_data.the_books.at(s.book_id.value()),
                         s.book_id.value(),
                         s);
    }
    else
    {
        for (int i{}; const auto& b : app_data.the_books)
            println(++i, ".\t", b.name);

        println("");
        println("a<n>:\tselect");
        println("b:\tadd");
        println("c<n>:\tremove");
        println("d:\tquit");

        const auto books_cmd{readln()};
        if (books_cmd.starts_with('a'))
        {
            s.book_id = std::stoi(books_cmd.substr(1)) - 1;
            return draw_book(app_data.the_books.at(s.book_id.value()),
                             s.book_id.value(),
                             s);
        }
        else if (books_cmd == "b")
        {
            println("add book");
            println("name: ");
            return {.data_act = ::xzr::learn::data::add_book{.name = readln()},
                    .console_state = s};
        }
        else if (books_cmd.starts_with('c'))
        {
            const auto book_id{std::stoi(books_cmd.substr(1)) - 1};
            return {.data_act = ::xzr::learn::data::remove_book{.id = book_id},
                    .console_state = s};
        }
        else if (books_cmd == "d")
            return {.data_act = ::xzr::learn::data::quit{}, .console_state = s};
        else
            return {.data_act = std::nullopt, .console_state = s};
    }
}
}
}

namespace xzr::learn::console
{
inline namespace v1
{
auto run() -> int
{
    auto app_data{::persist::read_or_create_app_data()};
    auto console_data{::console::data{}};

    for (;;)
    {
        console_data = ::console::draw(app_data, console_data.console_state);
        if (const auto data_act{console_data.data_act})
        {
            app_data = ::xzr::learn::data::update(std::move(app_data),
                                                  data_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<::xzr::learn::data::quit>(
                    data_act.value()))
                return 0;
        }
    }
}
}
}
