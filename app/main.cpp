#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
namespace po = ::boost::program_options;

using card = ::xzr::learn::data::card;
using cards = ::xzr::learn::data::cards;
using chapter = ::xzr::learn::data::chapter;
using book = ::xzr::learn::data::book;

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
[[nodiscard]] auto the_book() -> book&
{
    static auto p{book{}};
    return p;
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
        std::cout << ++i << ".\t" << cs.name << '\n';
}
auto list_cards_of_the_first_chapter_of_the_book() -> void
{
    for (const auto& c : cards_of_the_first_chapter_of_the_book())
        std::cout << c.front << "\t\t" << c.back << '\n';
}
auto create_chapter_in_the_book() -> void
{
    auto name{std::string{}};
    std::cout << "name: ";
    std::cin >> name;
    the_book().chapters.push_back({.name = name, .cards = {}});
    auto of{std::ofstream{books_path}};
    auto oa{::boost::archive::text_oarchive{of}};
    oa << first_chapter_of_the_book();
}
auto add_card_to_the_first_chapter_of_the_book() -> void
{
    auto front{std::string{}};
    auto back{std::string{}};
    std::cout << "front: ";
    std::cin >> front;
    std::cout << "back: ";
    std::cin >> back;
    cards_of_the_first_chapter_of_the_book().push_back(
        {.front = front, .back = back});
    auto of{std::ofstream{books_path}};
    auto oa{::boost::archive::text_oarchive{of}};
    oa << the_book();
}
auto start_training() -> void
{
    std::cout << "starting training\n";
    auto t{start_training(cards_of_the_first_chapter_of_the_book())};
    while (const auto maybe_crd{current_card(t)})
    {
        const auto& crd{maybe_crd.value()};
        std::cout << "\nfront: " << crd.front;
        std::cout << "\ntype back: ";
        std::string back{};
        std::cin >> back;
        t = eval_answer(t, crd, back);
    }
    std::cout << "end training\n";
}
}
auto main(int ac, char* av[]) -> int
{
    std::cout << "xzr learn\n";
    try
    {
        auto desc{po::options_description{"Allowed options"}};
        desc.add_options()("help,h", "produce help message");
        auto vm{po::variables_map{}};
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }
        //
        if (!fs::exists(books_path))
        {
            auto f{std::ofstream{books_path}};
            auto o{::boost::archive::text_oarchive{f}};
            o << the_book();
        }
        else
        {
            auto f{std::ifstream{books_path}};
            auto i{::boost::archive::text_iarchive{f}};
            i >> the_book();
        }
        //
        auto cmd{char{}};
        do
        {
            ::print_menue();
            std::cin >> cmd;
            if (cmd == 'l')
            {
                ::list_chapters_of_the_book();
            }
            if (cmd == 'b')
            {
                ::list_cards_of_the_first_chapter_of_the_book();
            }
            if (cmd == 'c')
            {
                ::create_chapter_in_the_book();
            }
            if (cmd == 'a')
            {
                ::add_card_to_the_first_chapter_of_the_book();
            }
            if (cmd == 's')
            {
                ::start_training();
            }
        } while (cmd != 'q');
        std::cout << "bye\n";
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Exception of unknown type!\n";
    }

    return 0;
}
