#include <data/data.hpp>

#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace po = boost::program_options;
using learn_card = ::xzr::learn::data::card;
using learn_cards = ::xzr::learn::data::cards;
namespace
{
auto print_menue() -> void
{
    std::cout << R"(
l    list cards
a    add cards
q    quit program
)";
}
[[nodiscard]] auto cards() -> learn_cards&
{
    static auto cs{learn_cards{}};
    return cs;
}
auto list_cards() -> void
{
    for (const auto& v : cards().content)
        std::cout << v.front << "\t\t" << v.back << '\n';
}
auto add_card() -> void
{
    auto front{std::string{}};
    auto back{std::string{}};
    std::cout << "front: ";
    std::cin >> front;
    std::cout << "back: ";
    std::cin >> back;
    cards().content.push_back({.front = front, .back = back});
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
        auto cmd{char{}};
        do
        {
            ::print_menue();
            std::cin >> cmd;
            if (cmd == 'l')
            {
                ::list_cards();
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
