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
namespace console::action
{
struct draw
{
};
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
struct text_input
{
    std::string txt{};
};
struct start_training
{
};
struct quit
{
};
using action =
    std::variant<draw, select, add, remove, text_input, start_training, quit>;
}
}

namespace
{
namespace console::states
{
struct books
{
};
struct add_book
{
};
struct book
{
    int book_id{};
};
struct add_chapter
{
    int book_id{};
};
struct chapter
{
    int book_id{};
    int chapter_id{};
};
struct add_card_front
{
    int book_id{};
    int chapter_id{};
};
struct add_card_back
{
    std::string front_txt{};
    int book_id{};
    int chapter_id{};
};
using state = std::variant<books,
                           add_book,
                           book,
                           add_chapter,
                           chapter,
                           add_card_front,
                           add_card_back>;
struct data
{
    std::optional<::xzr::learn::data::action> data_act{};
    state console_state{};
};
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
auto intent(const std::string& cmd_str) -> std::optional<action::action>
{
    if (::console::command::str::is(cmd_str, ::console::command::str::select))
        return action::select{.id = extract_id(cmd_str)};
    if (::console::command::str::is(cmd_str, ::console::command::str::add))
        return action::add{};
    if (::console::command::str::is(cmd_str, ::console::command::str::remove))
        return action::remove{.id = extract_id(cmd_str)};
    if (::console::command::str::is(cmd_str,
                                    ::console::command::str::start_training))
        return action::start_training{};
    if (::console::command::str::is(cmd_str, ::console::command::str::quit))
        return action::quit{};
    return action::text_input{cmd_str};
}
[[nodiscard]] auto draw(const ::xzr::learn::data::app& app_data,
                        states::state s,
                        action::action act)
{
    using boost::hof::match;

    std::visit(
        match(
            [&](states::books, action::draw) {
                ::console::content{}.books(app_data.the_books);
                ::console::menu{"book"}.select().add().remove().quit();
            },
            [](states::add_book, action::draw) {
                println("add book");
                println("name: ");
            },
            [&](states::book s, action::draw) {
                ::console::content{}.book(app_data.the_books.at(s.book_id));
                ::console::menu{"chapter"}.select().add().remove().quit();
            },
            [](states::add_chapter, action::draw) {
                println("add chapter");
                println("name: ");
            },
            [&](states::chapter s, action::draw) {
                ::console::content{}.chapter(
                    app_data.the_books.at(s.book_id).chapters.at(s.chapter_id));
                ::console::menu{"card"}.add().remove().start_training().quit();
            },
            [](states::add_card_front, action::draw) {
                println("add chard");
                println("front: ");
            },
            [](states::add_card_back, action::draw) { println("back: "); },
            [](auto, auto) {}),
        s,
        act);

    if (const auto cmd{intent(readln())})
    {
        return std::visit(
            match(
                [](states::books s, action::select a) {
                    if (a.id)
                        return states::data{.data_act = std::nullopt,
                                            .console_state =
                                                states::book{a.id.value()}};
                    return states::data{.data_act = std::nullopt,
                                        .console_state = s};
                },
                [](states::books s, action::add) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state = states::add_book{}};
                },
                [](states::books s, action::remove a) {
                    if (a.id)
                        return states::data{.data_act =
                                                ::xzr::learn::data::remove_book{
                                                    .id = a.id.value()},
                                            .console_state = s};
                    else
                        return states::data{.data_act = std::nullopt,
                                            .console_state = s};
                },
                [](states::books s, action::quit) {
                    return states::data{.data_act = ::xzr::learn::data::quit{},
                                        .console_state = s};
                },
                [](states::add_book s, action::text_input a) {
                    return states::data{
                        .data_act = ::xzr::learn::data::add_book{.name = a.txt},
                        .console_state = states::books{}};
                },
                [](states::book s, action::select a) {
                    if (a.id)
                        return states::data{.data_act = std::nullopt,
                                            .console_state = states::chapter{
                                                .book_id = s.book_id,
                                                .chapter_id = a.id.value()}};
                    return states::data{.data_act = std::nullopt,
                                        .console_state = s};
                },
                [](states::book s, action::add) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state = states::add_chapter{
                                            .book_id = s.book_id}};
                },
                [](states::book s, action::remove a) {
                    if (a.id)
                        return states::data{
                            .data_act =
                                ::xzr::learn::data::remove_chapter{
                                    .book_id = s.book_id,
                                    .id = a.id.value()},
                            .console_state = s};
                    else
                        return states::data{.data_act = std::nullopt,
                                            .console_state = s};
                },
                [](states::book, action::quit) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state = states::books{}};
                },
                [](states::add_chapter s, action::text_input a) {
                    return states::data{.data_act =
                                            ::xzr::learn::data::add_chapter{
                                                .book_id = s.book_id,
                                                .name = a.txt},
                                        .console_state =
                                            states::book{s.book_id}};
                },
                [](states::chapter s, action::add) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state = states::add_card_front{
                                            .book_id = s.book_id,
                                            .chapter_id = s.chapter_id}};
                },
                [](states::chapter s, action::remove a) {
                    if (a.id)
                        return states::data{.data_act =
                                                ::xzr::learn::data::remove_card{
                                                    .book_id = s.book_id,
                                                    .chapter_id = s.chapter_id,
                                                    .id = a.id.value()},
                                            .console_state = s};
                    return states::data{.data_act = std::nullopt,
                                        .console_state = s};
                },
                [&](states::chapter s, action::start_training) {
                    auto t{::xzr::learn::data::start_training(
                        app_data.the_books.at(s.book_id)
                            .chapters.at(s.chapter_id)
                            .cards)};
                    while (const auto c{::xzr::learn::data::current_card(t)})
                    {
                        println(c.value().front);
                        t = ::xzr::learn::data::eval_answer(t,
                                                            c.value(),
                                                            readln());
                    }
                    return states::data{.data_act = std::nullopt,
                                        .console_state = s};
                },
                [](states::chapter s, action::quit) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state =
                                            states::book{s.book_id}};
                },
                [](states::add_card_front s, action::text_input a) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state = states::add_card_back{
                                            .front_txt = a.txt,
                                            .book_id = s.book_id,
                                            .chapter_id = s.chapter_id}};
                },
                [](states::add_card_back s, action::text_input a) {
                    return states::data{.data_act =
                                            ::xzr::learn::data::add_card{
                                                .book_id = s.book_id,
                                                .chapter_id = s.chapter_id,
                                                .front = s.front_txt,
                                                .back = a.txt},
                                        .console_state = states::chapter{
                                            .book_id = s.book_id,
                                            .chapter_id = s.chapter_id}};
                },
                [](auto s, auto) {
                    return states::data{.data_act = std::nullopt,
                                        .console_state = s};
                }),
            s,
            cmd.value());
    }
    return states::data{.data_act = std::nullopt, .console_state = s};
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
    auto console_data{
        ::console::states::data{.data_act = std::nullopt,
                                .console_state = ::console::states::books{}}};
    for (;;)
    {
        console_data = ::console::draw(app_data,
                                       console_data.console_state,
                                       ::console::action::draw{});
        if (const auto data_act{console_data.data_act})
        {
            app_data = data::update(std::move(app_data), data_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<data::quit>(data_act.value()))
                break;
        }
    }
}
}
}
