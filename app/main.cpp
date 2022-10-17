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
using learn_card = ::xzr::learn::data::card;
using learn_cards = ::xzr::learn::data::cards;
using learn_package = ::xzr::learn::data::package;
namespace
{
const auto cards_path{fs::path{"xzr_learn_package.txt"}};
auto print_menue() -> void
{
    std::cout << R"(
l    list package
a    add cards
q    quit program
)";
}
[[nodiscard]] auto package() -> learn_package&
{
    static auto p{learn_package{}};
    return p;
}
[[nodiscard]] auto cards() -> learn_cards&
{
    return package().content.at(0);
}
auto list_package() -> void
{
    for (int i{}; const auto& cs : package().content)
        std::cout << ++i << ".\t" << cs.name << '\n';
}
auto list_cards() -> void
{
    for (const auto& c : cards().content)
        std::cout << c.front << "\t\t" << c.back << '\n';
}
auto add_card() -> void
{
    auto name{std::string{}};
    auto front{std::string{}};
    auto back{std::string{}};
    std::cout << "name: ";
    std::cin >> name;
    package().content.push_back({.name = name, .content = {}});
    std::cout << "front: ";
    std::cin >> front;
    std::cout << "back: ";
    std::cin >> back;
    package().content.back().content.push_back({.front = front, .back = back});
    auto of{std::ofstream{cards_path}};
    auto oa{::boost::archive::text_oarchive{of}};
    oa << cards();
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
        if (!fs::exists(cards_path))
        {
            auto f{std::ofstream{cards_path}};
            auto o{::boost::archive::text_oarchive{f}};
            o << package();
        }
        else
        {
            auto f{std::ifstream{cards_path}};
            auto i{::boost::archive::text_iarchive{f}};
            i >> package();
        }
        //
        auto cmd{char{}};
        do
        {
            ::print_menue();
            std::cin >> cmd;
            if (cmd == 'l')
            {
                ::list_package();
            }
            if (cmd == 'a')
            {
                ::add_card();
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
