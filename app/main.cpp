#include "console.hpp"

#include <boost/program_options.hpp>

#include <stdexcept>

namespace po = ::boost::program_options;

using ::xzr::learn::console::println;

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
        const auto ret{::xzr::learn::console::run()};
        println("bye from xzr.learn");

        return ret;
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
