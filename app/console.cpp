#include "console.hpp"

#include <algorithm>
#include <data/action.hpp>
#include <data/app.hpp>
#include <data/books.hpp>
#include <data/serialize.hpp>
#include <data/training.hpp>
#include <data/update.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/hof/match.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
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
const auto exit{command{.cmd = "e", .desc = "exit"}};
auto is(const std::string& str_cmd, const command& c)
{
    if (str_cmd == c.cmd)
        return true;
    const auto num_start_idx{c.cmd.find_first_of('<')};
    return (num_start_idx != std::string::npos) &&
           (str_cmd.substr(0, num_start_idx) ==
            c.cmd.substr(0, num_start_idx)) &&
           std::all_of(std::next(str_cmd.cbegin(), num_start_idx),
                       str_cmd.cend(),
                       [](auto a) { return !!std::isdigit(a); });
}
auto as_str(const command& c)
{
    return c.cmd + ":\t" + c.desc;
}
}
}

namespace
{
namespace console::actions
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
struct exit
{
};
using action =
    std::variant<select, add, remove, text_input, start_training, quit, exit>;
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
    std::optional<::xzr::learn::data::books::actions::action> data_act{};
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
    auto exit() -> menu&
    {
        println(::console::command::str::as_str(::console::command::str::exit));
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
auto intent(states::state s, const std::string& cmd_str) -> actions::action
{
    using ::boost::hof::match;

    return std::visit(
        match(
            [&](states::books) -> actions::action {
                if (command::str::is(cmd_str, command::str::select))
                    return actions::select{.id = extract_id(cmd_str)};
                if (command::str::is(cmd_str, command::str::add))
                    return actions::add{};
                if (command::str::is(cmd_str, command::str::remove))
                    return actions::remove{.id = extract_id(cmd_str)};
                if (command::str::is(cmd_str, command::str::exit))
                    return actions::exit{};
                return actions::text_input{cmd_str};
            },
            [&](states::add_book) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::book) -> actions::action {
                if (command::str::is(cmd_str, command::str::select))
                    return actions::select{.id = extract_id(cmd_str)};
                if (command::str::is(cmd_str, command::str::add))
                    return actions::add{};
                if (command::str::is(cmd_str, command::str::remove))
                    return actions::remove{.id = extract_id(cmd_str)};
                if (command::str::is(cmd_str, command::str::quit))
                    return actions::quit{};
                return actions::text_input{cmd_str};
            },
            [&](states::add_chapter) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::chapter) -> actions::action {
                if (command::str::is(cmd_str, command::str::add))
                    return actions::add{};
                if (command::str::is(cmd_str, command::str::remove))
                    return actions::remove{.id = extract_id(cmd_str)};
                if (command::str::is(cmd_str, command::str::start_training))
                    return actions::start_training{};
                if (command::str::is(cmd_str, command::str::quit))
                    return actions::quit{};
                return actions::text_input{cmd_str};
            },
            [&](states::add_card_front) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](states::add_card_back) -> actions::action {
                return actions::text_input{cmd_str};
            },
            [&](auto) -> actions::action {
                return actions::text_input{cmd_str};
            }),
        s);
}
auto draw(const ::xzr::learn::data::app& app_data, states::state s)
{
    using ::boost::hof::match;

    std::visit(
        match(
            [&](states::books) {
                content{}.books(app_data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](states::book s) {
                content{}.book(app_data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](states::chapter s) {
                content{}.chapter(
                    app_data.the_books.at(s.book_id).chapters.at(s.chapter_id));
                menu{"card"}.add().remove().start_training().quit();
            },
            [](states::add_card_front) {
                println("add chard");
                println("front: ");
            },
            [](states::add_card_back) { println("back: "); },
            [](auto, auto) {}),
        s);
}
auto dispatch(actions::action act,
              states::state state,
              const ::xzr::learn::data::app& app_data)
{
    using ::boost::hof::match;

    return std::visit(
        match(
            [](states::books s, actions::select a) -> states::data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .console_state = states::book{a.id.value()}};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::books s, actions::add) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state = states::add_book{}};
            },
            [](states::books s, actions::remove a) -> states::data {
                if (a.id)
                    return {.data_act =
                                ::xzr::learn::data::books::actions::remove_book{
                                    .id = a.id.value()},
                            .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::books s, actions::exit) -> states::data {
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::add_book s, actions::text_input a) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_book{
                                .name = a.txt},
                        .console_state = states::books{}};
            },
            [](states::book s, actions::select a) -> states::data {
                if (a.id)
                    return {.data_act = std::nullopt,
                            .console_state =
                                states::chapter{.book_id = s.book_id,
                                                .chapter_id = a.id.value()}};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::book s, actions::add) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            states::add_chapter{.book_id = s.book_id}};
            },
            [](states::book s, actions::remove a) -> states::data {
                if (a.id)
                    return {
                        .data_act =
                            ::xzr::learn::data::books::actions::remove_chapter{
                                .book_id = s.book_id,
                                .id = a.id.value()},
                        .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::book, actions::quit) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state = states::books{}};
            },
            [](states::add_chapter s, actions::text_input a) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_chapter{
                                .book_id = s.book_id,
                                .name = a.txt},
                        .console_state = states::book{s.book_id}};
            },
            [](states::chapter s, actions::add) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            states::add_card_front{.book_id = s.book_id,
                                                   .chapter_id = s.chapter_id}};
            },
            [](states::chapter s, actions::remove a) -> states::data {
                if (a.id)
                    return {.data_act =
                                ::xzr::learn::data::books::actions::remove_card{
                                    .book_id = s.book_id,
                                    .chapter_id = s.chapter_id,
                                    .id = a.id.value()},
                            .console_state = s};
                return {.data_act = std::nullopt, .console_state = s};
            },
            [&](states::chapter s, actions::start_training) -> states::data {
                auto t{::xzr::learn::data::start_training(
                    app_data.the_books.at(s.book_id)
                        .chapters.at(s.chapter_id)
                        .cards)};
                while (const auto c{::xzr::learn::data::current_card(t)})
                {
                    println(c.value().front);
                    t = ::xzr::learn::data::eval_answer(t, c.value(), readln());
                }
                return {.data_act = std::nullopt, .console_state = s};
            },
            [](states::chapter s, actions::quit) -> states::data {
                return states::data{.data_act = std::nullopt,
                                    .console_state = states::book{s.book_id}};
            },
            [](states::add_card_front s,
               actions::text_input a) -> states::data {
                return {.data_act = std::nullopt,
                        .console_state =
                            states::add_card_back{.front_txt = a.txt,
                                                  .book_id = s.book_id,
                                                  .chapter_id = s.chapter_id}};
            },
            [](states::add_card_back s, actions::text_input a) -> states::data {
                return {.data_act =
                            ::xzr::learn::data::books::actions::add_card{
                                .book_id = s.book_id,
                                .chapter_id = s.chapter_id,
                                .front = s.front_txt,
                                .back = a.txt},
                        .console_state =
                            states::chapter{.book_id = s.book_id,
                                            .chapter_id = s.chapter_id}};
            },
            [](auto s, auto) -> states::data {
                return {.data_act = std::nullopt, .console_state = s};
            }),
        state,
        act);
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
        ::console::draw(app_data, console_data.console_state);
        const auto act{
            ::console::intent(console_data.console_state, ::console::readln())};
        console_data =
            ::console::dispatch(act, console_data.console_state, app_data);
        if (const auto data_act{console_data.data_act})
        {
            app_data = data::update(std::move(app_data), data_act.value());
            ::persist::save(app_data);
        }
        if (std::holds_alternative<::console::actions::exit>(act))
            break;
    }
}
}
}
