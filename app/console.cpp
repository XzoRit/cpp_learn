#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/training.hpp>
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
auto str_to_id(const std::string& s) -> std::optional<int>
{
    try
    {
        return std::stoi(s) - 1;
    }
    catch (...)
    {
        return std::nullopt;
    }
}
auto extract_id(const std::string& s)
{
    return str_to_id(s.substr(1));
}
}
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
    std::optional<int> chapter_id{};
};
struct data
{
    std::optional<::xzr::learn::data::action> data_act{};
    state console_state{};
};
auto draw_chapter(const ::xzr::learn::data::chapter& chapter, state s)
    -> ::console::data
{
    println("name: ", chapter.name);
    for (int i{}; const auto& c : chapter.cards)
    {
        println(++i, ".\t", c.front);
        println(i, ".\t", c.back);
    }

    println("");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("s:\tstart training");
    println("d:\tquit");

    const auto& card_cmd{readln()};
    if (card_cmd == "b")
    {
        println("add chard");
        println("front: ");
        const auto& front{readln()};
        println("back: ");
        const auto& back{readln()};
        return {
            .data_act =
                ::xzr::learn::data::add_card{.book_id = s.book_id.value(),
                                             .chapter_id = s.chapter_id.value(),
                                             .front = front,
                                             .back = back},
            .console_state = s};
    }
    else if (card_cmd.starts_with('c'))
    {
        if (const auto card_id{::extract_id(card_cmd)})
            return {.data_act =
                        ::xzr::learn::data::remove_card{
                            .book_id = s.book_id.value(),
                            .chapter_id = s.chapter_id.value(),
                            .id = card_id.value()},
                    .console_state = s};
    }
    else if (card_cmd == "s")
    {
        auto t{::xzr::learn::data::start_training(chapter.cards)};
        while (const auto c{::xzr::learn::data::current_card(t)})
        {
            println(c.value().front);
            t = ::xzr::learn::data::eval_answer(t, c.value(), readln());
        }
    }
    else if (card_cmd == "d")
        s.chapter_id.reset();

    return {.data_act = std::nullopt, .console_state = s};
}
auto draw_book(const ::xzr::learn::data::book& book, state s) -> ::console::data
{
    if (s.chapter_id)
        return draw_chapter(book.chapters.at(s.chapter_id.value()), s);
    else
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
        if (book_cmd.starts_with('a'))
        {
            if ((s.chapter_id = ::extract_id(book_cmd)))
                return draw_chapter(book.chapters.at(s.chapter_id.value()), s);
        }
        else if (book_cmd == "b")
        {
            println("add chapter");
            println("name: ");
            return {.data_act =
                        ::xzr::learn::data::add_chapter{.book_id =
                                                            s.book_id.value(),
                                                        .name = readln()},
                    .console_state = s};
        }
        else if (book_cmd.starts_with('c'))
        {
            if (const auto chapter_id{::extract_id(book_cmd)})
                return {.data_act =
                            ::xzr::learn::data::remove_chapter{
                                .book_id = s.book_id.value(),
                                .id = chapter_id.value()},
                        .console_state = s};
        }
        else if (book_cmd == "d")
            s.book_id.reset();

        return {.data_act = std::nullopt, .console_state = s};
    }
}
[[nodiscard]] auto draw_books(const ::xzr::learn::data::books& books, state s)
    -> ::console::data
{
    for (int i{}; const auto& b : books)
        println(++i, ".\t", b.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");

    const auto books_cmd{readln()};
    if (books_cmd.starts_with('a'))
    {
        if ((s.book_id = ::extract_id(books_cmd)))
            return draw_book(books.at(s.book_id.value()), s);
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
        if (const auto book_id{::extract_id(books_cmd)})
            return {.data_act =
                        ::xzr::learn::data::remove_book{.id = book_id.value()},
                    .console_state = s};
    }
    else if (books_cmd == "d")
        return {.data_act = ::xzr::learn::data::quit{}, .console_state = s};

    return {.data_act = std::nullopt, .console_state = s};
}
[[nodiscard]] auto draw(const ::xzr::learn::data::app& app_data, state s)
    -> ::console::data
{

    if (s.book_id)
        return draw_book(app_data.the_books.at(s.book_id.value()), s);
    else
        return draw_books(app_data.the_books, s);
}
}
}

namespace xzr::learn::console
{
inline namespace v1
{
auto run() -> void
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
                break;
        }
    }
}
}
}
