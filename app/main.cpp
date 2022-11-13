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

namespace
{
const auto books_path{fs::path{"xzr_learn_books.txt"}};
auto print_menue() -> void
{
    std::cout << R"(
l    list all chapters of the book
b    list all cards of the first chapters of the book
c    create chapter in the book
a    add card to the first chapter of the book
s    start training with cards of the first chapter of the book
q    quit program
)";
}
auto print(auto&&... txt)
{
    (std::cout << ... << txt);
}
auto println(auto&&... txt)
{
    print(txt..., '\n');
}
[[nodiscard]] auto readln() -> std::string
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
[[nodiscard]] auto the_app() -> app&
{
    static auto a{app{.the_books{book{}}}};
    return a;
}
[[nodiscard]] auto the_books() -> books&
{
    return the_app().the_books;
}
[[nodiscard]] auto the_first_book() -> book&
{
    return the_books().at(0);
}
[[nodiscard]] auto the_chapters_of_the_first_book() -> chapters&
{
    return the_first_book().chapters;
}
[[nodiscard]] auto first_chapter_of_the_first_book() -> chapter&
{
    return the_chapters_of_the_first_book().at(0);
}
[[nodiscard]] auto cards_of_the_first_chapter_of_the_first_book() -> cards&
{
    return first_chapter_of_the_first_book().cards;
}
auto list_chapters_of_the_first_book() -> void
{
    for (int i{}; const auto& cs : the_chapters_of_the_first_book())
        println(++i, ".\t", cs.name);
}
auto list_cards_of_the_first_chapter_of_the_first_book() -> void
{
    for (const auto& c : cards_of_the_first_chapter_of_the_first_book())
        println(c.front, "\t\t\t\t\t", c.back);
}
auto save() -> void
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};

    save(o, the_app());
}
auto load() -> void
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};

    the_app() = load(i);
}
auto create_chapter_in_the_first_book() -> void
{
    print("name: ");
    const auto name{readln()};
    the_chapters_of_the_first_book().push_back({.name = name, .cards = {}});
    save();
}
auto add_card_to_the_first_chapter_of_the_first_book() -> void
{
    print("front: ");
    const auto front{readln()};
    print("back: ");
    const auto back{readln()};
    cards_of_the_first_chapter_of_the_first_book().push_back(
        {.front = front, .back = back});
    save();
}
auto start_training() -> void
{
    println("starting training");
    auto t{start_training(cards_of_the_first_chapter_of_the_first_book())};
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
auto update_app() -> void
{
    if (fs::exists(books_path))
        load();
    else
        save();
}
namespace action
{
struct list_chapters
{
};
struct list_cards
{
};
struct create_chapter
{
};
struct add_card
{
};
struct start_training
{
};
using action = std::variant<list_chapters,
                            list_cards,
                            create_chapter,
                            add_card,
                            start_training>;
}
auto intent(std::string_view cmd) -> std::optional<action::action>
{
    static const std::map<std::string_view, action::action> cmd_actions{
        {{"l", action::list_chapters{}},
         {"b", action::list_cards{}},
         {"c", action::create_chapter{}},
         {"a", action::add_card{}},
         {"s", action::start_training{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
auto run() -> void
{
    for (;;)
    {
        ::print_menue();
        const auto cmd{readln()};
        if (cmd == "q")
            break;
        if (const auto act{intent(cmd)})
        {
            std::visit(
                hof::match(
                    [](action::list_chapters) {
                        list_chapters_of_the_first_book();
                    },
                    [](action::list_cards) {
                        list_cards_of_the_first_chapter_of_the_first_book();
                    },
                    [](action::create_chapter) {
                        create_chapter_in_the_first_book();
                    },
                    [](action::add_card) {
                        add_card_to_the_first_chapter_of_the_first_book();
                    },
                    [](action::start_training) { start_training(); }),
                act.value());
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
            ::update_app();
            ::run();
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
