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
auto cards() -> learn_cards&
{
    static learn_cards cs{};
    return cs;
}
auto list_cards() -> void
{
    for (const auto& v : cards().cont)
        std::cout << v.front << "\t\t" << v.back << '\n';
}
auto add_card() -> void
{
    std::string front{};
    std::string back{};
    std::cout << "front: ";
    std::cin >> front;
    std::cout << "back: ";
    std::cin >> back;
    cards().add({.front = front, .back = back});
}
}
auto main(int ac, char* av[]) -> int
{
    std::cout << "voc trainer\n";
    try
    {
        po::options_description desc("Allowed options");
        desc.add_options()("help,h", "produce help message");
        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }
        char cmd{};
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
