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

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using ::xzr::learn::console::print;
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
namespace console::command::str
{
struct command
{
    std::string cmd{};
    std::string desc{};
};
const auto select{command{.cmd = "a<n>", .desc = "select"}};
const auto add{command{.cmd = "b", .desc = "add"}};
const auto remove{command{.cmd = "c<n>", .desc = "remove"}};
const auto start_training{command{.cmd = "s", .desc = "start training"}};
const auto quit{command{.cmd = "d", .desc = "quit"}};
auto is(const std::string& str_cmd, const command& c)
{
    return str_cmd.starts_with(c.cmd.at(0));
}
auto as_str(const command& c)
{
    return c.cmd + ":\t" + c.desc;
}
}
}
namespace
{
namespace console::command
{
struct select
{
    std::optional<int> id{};
};
struct add
{
};
struct remove
{
    std::optional<int> id{};
};
struct start_training
{
};
struct quit
{
};
using cmd = std::variant<select, add, remove, start_training, quit>;
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
auto create(const std::string& cmd_str) -> std::optional<cmd>
{
    if (::console::command::str::is(cmd_str, ::console::command::str::select))
        return select{.id = extract_id(cmd_str)};
    if (::console::command::str::is(cmd_str, ::console::command::str::add))
        return add{};
    if (::console::command::str::is(cmd_str, ::console::command::str::remove))
        return remove{.id = extract_id(cmd_str)};
    if (::console::command::str::is(cmd_str,
                                    ::console::command::str::start_training))
        return start_training{};
    if (::console::command::str::is(cmd_str, ::console::command::str::quit))
        return quit{};
    return std::nullopt;
}
}
}

namespace
{
namespace console
{
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
[[nodiscard]] auto readln()
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
        println(
            ::console::command::str::as_str(::console::command::str::select));
        return *this;
    }
    auto add() -> menu&
    {
        println(::console::command::str::as_str(::console::command::str::add));
        return *this;
    }
    auto remove() -> menu&
    {
        println(
            ::console::command::str::as_str(::console::command::str::remove));
        return *this;
    }
    auto start_training() -> menu&
    {
        println(::console::command::str::as_str(
            ::console::command::str::start_training));
        return *this;
    }
    auto quit() -> menu&
    {
        println(::console::command::str::as_str(::console::command::str::quit));
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
    auto chapter(const ::xzr::learn::data::chapter& ch)
    {
        println("=== chapter: ", ch.name);
        println("cards:");
        for (int i{}; const auto& c : ch.cards)
        {
            println(++i, ".\t", c.front);
            println(i, ".\t", c.back);
        }
    }
    auto book(const ::xzr::learn::data::book& b)
    {
        println("=== book: ", b.name);
        println("chapters:");
        for (int i{}; const auto& c : b.chapters)
            println(++i, ".\t", c.name);
    }
    auto books(const ::xzr::learn::data::books& bs)
    {
        println("=== books:");
        for (int i{}; const auto& b : bs)
            println(++i, ".\t", b.name);
    }
};
auto draw_chapter(const ::xzr::learn::data::chapter& chapter, state s)
    -> ::console::data
{
    using ::boost::hof::match;

    ::console::content{}.chapter(chapter);
    ::console::menu{"card"}.add().remove().start_training().quit();

    if (const auto cmd{::console::command::create(readln())})
        return std::visit(
            match(
                [&](::console::command::add) -> ::console::data {
                    println("add chard");
                    println("front: ");
                    const auto& front{readln()};
                    println("back: ");
                    const auto& back{readln()};
                    return {.data_act =
                                ::xzr::learn::data::add_card{
                                    .book_id = s.book_id.value(),
                                    .chapter_id = s.chapter_id.value(),
                                    .front = front,
                                    .back = back},
                            .console_state = s};
                },
                [&](::console::command::remove a) -> ::console::data {
                    if (a.id)
                        return ::console::data{
                            .data_act =
                                ::xzr::learn::data::remove_card{
                                    .book_id = s.book_id.value(),
                                    .chapter_id = s.chapter_id.value(),
                                    .id = a.id.value()},
                            .console_state = s};
                    return {.data_act = std::nullopt, .console_state = s};
                },
                [&](::console::command::start_training) -> ::console::data {
                    auto t{::xzr::learn::data::start_training(chapter.cards)};
                    while (const auto c{::xzr::learn::data::current_card(t)})
                    {
                        println(c.value().front);
                        t = ::xzr::learn::data::eval_answer(t,
                                                            c.value(),
                                                            readln());
                    }
                    return {.data_act = std::nullopt, .console_state = s};
                },
                [&](::console::command::quit) -> ::console::data {
                    s.chapter_id.reset();
                    return {.data_act = std::nullopt, .console_state = s};
                },
                [&](auto) -> ::console::data {
                    return {.data_act = std::nullopt, .console_state = s};
                }),
            cmd.value());
    else
        return {.data_act = std::nullopt, .console_state = s};
}
auto draw_book(const ::xzr::learn::data::book& book, state s) -> ::console::data
{
    using ::boost::hof::match;

    if (s.chapter_id)
        return draw_chapter(book.chapters.at(s.chapter_id.value()), s);
    else
    {
        ::console::content{}.book(book);
        ::console::menu{"chapter"}.select().add().remove().quit();

        if (const auto cmd{::console::command::create(readln())})
            return std::visit(
                match(
                    [&](::console::command::select a) -> ::console::data {
                        if (a.id)
                        {
                            s.chapter_id = a.id.value();
                            return draw_chapter(book.chapters.at(a.id.value()),
                                                s);
                        }
                        return {.data_act = std::nullopt, .console_state = s};
                    },
                    [&](::console::command::add) -> ::console::data {
                        println("add chapter");
                        println("name: ");
                        return {.data_act =
                                    ::xzr::learn::data::add_chapter{
                                        .book_id = s.book_id.value(),
                                        .name = readln()},
                                .console_state = s};
                    },
                    [&](::console::command::remove a) -> ::console::data {
                        if (a.id)
                            return {.data_act =
                                        ::xzr::learn::data::remove_chapter{
                                            .book_id = s.book_id.value(),
                                            .id = a.id.value()},
                                    .console_state = s};
                        else
                            return {.data_act = std::nullopt,
                                    .console_state = s};
                    },
                    [&](::console::command::quit) -> ::console::data {
                        s.book_id.reset();

                        return {.data_act = std::nullopt, .console_state = s};
                    },
                    [&](auto) -> ::console::data {
                        return {.data_act = std::nullopt, .console_state = s};
                    }),
                cmd.value());
        else
            return {.data_act = std::nullopt, .console_state = s};
    }
}
[[nodiscard]] auto draw_books(const ::xzr::learn::data::books& books, state s)
    -> ::console::data
{
    using ::boost::hof::match;

    ::console::content{}.books(books);
    ::console::menu{"book"}.select().add().remove().quit();

    if (const auto cmd{::console::command::create(readln())})
        return std::visit(
            match(
                [&](::console::command::select a) -> ::console::data {
                    if (a.id)
                    {
                        s.book_id = a.id.value();
                        return draw_book(books.at(a.id.value()), s);
                    }
                    return {.data_act = std::nullopt, .console_state = s};
                },
                [&](::console::command::add) -> ::console::data {
                    println("add book");
                    println("name: ");
                    return {.data_act =
                                ::xzr::learn::data::add_book{.name = readln()},
                            .console_state = s};
                },
                [&](::console::command::remove a) -> ::console::data {
                    if (a.id)
                        return {.data_act =
                                    ::xzr::learn::data::remove_book{
                                        .id = a.id.value()},
                                .console_state = s};
                    else
                        return {.data_act = std::nullopt, .console_state = s};
                },
                [&](::console::command::quit) -> ::console::data {
                    return {.data_act = ::xzr::learn::data::quit{},
                            .console_state = s};
                },
                [&](auto) -> ::console::data {
                    return {.data_act = std::nullopt, .console_state = s};
                }),
            cmd.value());
    else
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
