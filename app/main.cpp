#include "console.hpp"

#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/training.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/hof/match.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <variant>

using namespace std::string_literals;

namespace fs = std::filesystem;
namespace po = ::boost::program_options;
namespace hof = ::boost::hof;

using app = ::xzr::learn::data::app;
using book = ::xzr::learn::data::book;
using books = ::xzr::learn::data::books;
using card = ::xzr::learn::data::card;
using cards = ::xzr::learn::data::cards;
using chapter = ::xzr::learn::data::chapter;
using chapters = ::xzr::learn::data::chapters;
using training = ::xzr::learn::data::training;

using ::xzr::learn::console::print;
using ::xzr::learn::console::println;
using ::xzr::learn::console::render;

namespace
{
const auto books_path{fs::path{"xzr_learn_books.txt"}};
auto save(const app& app_data) -> void
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};

    save(o, app_data);
}
auto load() -> app
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};

    return load(i);
}
[[nodiscard]] auto readln() -> std::string
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
auto add_book(books& bs) -> void
{
    print("name: ");
    const auto name{readln()};
    bs.push_back({.name = name, .chapters = {}});
}
auto add_chapter(chapters& cs) -> void
{
    print("name: ");
    const auto name{readln()};
    cs.push_back({.name = name, .cards = {}});
}
auto add_card(cards& cs) -> void
{
    print("front: ");
    const auto front{readln()};
    print("back: ");
    const auto back{readln()};
    cs.push_back({.front = front, .back = back});
}
auto start_training(training t) -> void
{
    println("starting training");
    while (const auto maybe_crd{current_card(t)})
    {
        const auto& crd{maybe_crd.value()};
        println("\nfront: ", crd.front);
        print("type back: ");
        const auto back{readln()};
        t = eval_answer(t, crd, back);
        println("back: ", crd.back);
    }
    println("end training\n");
}
[[nodiscard]] auto read_or_create_app_data() -> app
{
    if (!fs::exists(books_path))
        save(app{});

    return load();
}
namespace action
{
struct add_book
{
};
struct add_chapter
{
};
struct add_card
{
};
struct start_training
{
};
using action = std::variant<add_book, add_chapter, add_card, start_training>;
}
[[nodiscard]] auto intent(std::string_view cmd) -> std::optional<action::action>
{
    static const std::map<std::string_view, action::action> cmd_actions{
        {{"a", action::add_book{}},
         {"b", action::add_chapter{}},
         {"c", action::add_card{}},
         {"z", action::start_training{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
auto update(app app_data, action::action act) -> app
{
    std::visit(
        hof::match(
            [&](action::add_book) {
                add_book(app_data.the_books);
                save(app_data);
            },
            [&](action::add_chapter) {
                add_chapter(app_data.the_books.at(0).chapters);
                save(app_data);
            },
            [&](action::add_card) {
                add_card(app_data.the_books.at(0).chapters.at(0).cards);
                save(app_data);
            },
            [&](action::start_training) {
                start_training(training{
                    .cards = app_data.the_books.at(0).chapters.at(0).cards});
            }),
        act);

    return app_data;
}
auto run(app app_data) -> void
{
    render(app_data);
    for (;;)
    {
        const auto cmd{readln()};
        if (cmd == "q")
            break;
        if (const auto act{intent(cmd)})
        {
            app_data = update(std::move(app_data), act.value());
            render(app_data);
        }
    }
}
}
auto main(int ac, char* av[]) -> int
{
    try
    {
        {
            auto desc{
                po::options_description{"xzr.learn command line options"}};
            desc.add_options()("help,h", "produce help message");
            auto vm{po::variables_map{}};
            po::store(po::parse_command_line(ac, av, desc), vm);
            po::notify(vm);
            if (vm.count("help"))
            {
                println(desc);
                return 0;
            }
        }
        println("welcome to xzr::learn");
        {

            ::run(::read_or_create_app_data());
        }
        println("bye from xzr.learn");
        return 0;
    }
    catch (const std::exception& e)
    {
        println("error: ", e.what());
        return 1;
    }
    catch (...)
    {
        println("Exception of unknown type!");
    }

    return 0;
}
