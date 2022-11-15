#include "console.hpp"

#include <data/app.hpp>
#include <data/serialize.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;
namespace po = ::boost::program_options;

using ::xzr::learn::console::println;
using ::xzr::learn::console::run;

using app = ::xzr::learn::data::app;

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
[[nodiscard]] auto read_or_create_app_data() -> app
{
    if (!fs::exists(books_path))
        save(app{});

    return load();
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
