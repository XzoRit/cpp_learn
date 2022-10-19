#include <data/box.hpp>
#include <data/data.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
namespace po = ::boost::program_options;
using word = ::xzr::learn::data::card;
using chapter = ::xzr::learn::data::cards;
using book = ::xzr::learn::data::package;
using box = ::xzr::learn::data::box;
namespace
{
const auto books_path{fs::path{"xzr_learn_books.txt"}};
auto print_menue() -> void
{
    std::cout << R"(
l    list all chapters of the book
b    list all words of the first chapters of the book
c    create chapter in the book
a    add word to the first chapter of the book
s    start training with words of the first chapter of the book
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
    return the_book().content.at(0);
}
auto list_chapters_of_the_book() -> void
{
    for (int i{}; const auto& cs : the_book().content)
        std::cout << ++i << ".\t" << cs.name << '\n';
}
auto list_words_of_the_first_chapter_of_the_book() -> void
{
    for (const auto& c : first_chapter_of_the_book().content)
        std::cout << c.front << "\t\t" << c.back << '\n';
}
auto create_chapter_in_the_book() -> void
{
    auto name{std::string{}};
    std::cout << "name: ";
    std::cin >> name;
    the_book().content.push_back({.name = name, .content = {}});
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
    first_chapter_of_the_book().content.push_back(
        {.front = front, .back = back});
    auto of{std::ofstream{books_path}};
    auto oa{::boost::archive::text_oarchive{of}};
    oa << the_book();
}
auto start_training() -> void
{
    std::cout << "starting training\n";
    auto b{box{first_chapter_of_the_book()}};
    while (b.has_next())
    {
        const auto& c{b.next()};
        std::cout << c.front;
        std::cout << "\ntype back: ";
        std::string back{};
        std::cin >> back;
        b.move_card(c, back);
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
                ::list_words_of_the_first_chapter_of_the_book();
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
