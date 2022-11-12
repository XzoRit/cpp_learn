#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/training.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std::string_literals;

namespace fs = std::filesystem;
namespace po = ::boost::program_options;

using app = ::xzr::learn::data::app;
using book = ::xzr::learn::data::book;
using card = ::xzr::learn::data::card;
using cards = ::xzr::learn::data::cards;
using chapter = ::xzr::learn::data::chapter;

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
[[nodiscard]] auto the_book() -> book&
{
    return the_app().the_books.at(0);
}
[[nodiscard]] auto first_chapter_of_the_book() -> chapter&
{
    return the_book().chapters.at(0);
}
[[nodiscard]] auto cards_of_the_first_chapter_of_the_book() -> cards&
{
    return first_chapter_of_the_book().cards;
}
auto list_chapters_of_the_book() -> void
{
    for (int i{}; const auto& cs : the_book().chapters)
        println(++i, ".\t", cs.name);
}
auto list_cards_of_the_first_chapter_of_the_book() -> void
{
    for (const auto& c : cards_of_the_first_chapter_of_the_book())
        println(c.front, "\t\t\t\t\t", c.back);
}
auto save() -> void
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::serialize;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};

    serialize(o, the_app());
}
auto load() -> void
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::serialize;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};

    serialize(i, the_app());
}
auto create_chapter_in_the_book() -> void
{
    print("name: ");
    const auto name{readln()};
    the_book().chapters.push_back({.name = name, .cards = {}});
    save();
}
auto add_card_to_the_first_chapter_of_the_book() -> void
{
    print("front: ");
    const auto front{readln()};
    print("back: ");
    const auto back{readln()};
    cards_of_the_first_chapter_of_the_book().push_back(
        {.front = front, .back = back});
    save();
}
auto start_training() -> void
{
    println("starting training");
    auto t{start_training(cards_of_the_first_chapter_of_the_book())};
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
}
auto main(int ac, char* av[]) -> int
{
    println("xzr learn");
    try
    {
        auto desc{po::options_description{"Allowed options"}};
        desc.add_options()("help,h", "produce help message");
        auto vm{po::variables_map{}};
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
        if (vm.count("help"))
        {
            println(desc);
            return 0;
        }

        if (fs::exists(books_path))
            load();
        else
            save();

        auto cmd{""s};
        do
        {
            ::print_menue();
            cmd = readln();
            if (cmd == "l")
            {
                ::list_chapters_of_the_book();
            }
            if (cmd == "b")
            {
                ::list_cards_of_the_first_chapter_of_the_book();
            }
            if (cmd == "c")
            {
                ::create_chapter_in_the_book();
            }
            if (cmd == "a")
            {
                ::add_card_to_the_first_chapter_of_the_book();
            }
            if (cmd == "s")
            {
                ::start_training();
            }
        } while (cmd != "q");
        println("bye");
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
