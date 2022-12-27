#include <console.hpp>
#include <data/data.hpp>
#include <view/persist.hpp>

#include <boost/program_options.hpp>

#include <stdexcept>

using ::xzr::learn::console::println;
auto main(int ac, char* av[]) -> int
{
    std::cout << R"(
TODO:
- serialize/save training
- serialize/save view state
- test view state machine
- test intent function
)";
    try
    {
        {
            namespace po = ::boost::program_options;
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
            auto view_model_data{::xzr::learn::console::view::model::data{
                .view_state =
                    ::xzr::learn::console::view::model::states::books{},
                .model_data =
                    ::xzr::learn::persist::load_or_create_empty_data()}};
            for (;;)
            {
                ::xzr::learn::console::draw(view_model_data);
                const auto view_model_act{::xzr::learn::console::intent(
                    ::xzr::learn::console::readln())};
                view_model_data =
                    ::xzr::learn::console::view::model::update(view_model_data,
                                                               view_model_act);
                ::xzr::learn::persist::save(view_model_data.model_data);
                if (std::holds_alternative<
                        ::xzr::learn::console::view::model::actions::exit>(
                        view_model_act) &&
                    std::holds_alternative<
                        ::xzr::learn::console::view::model::states::books>(
                        view_model_data.view_state))
                    break;
            }
        }
        println("bye from xzr.learn");
    }
    catch (const std::exception& e)
    {
        println("error: ", e.what());
        return 1;
    }
    catch (...)
    {
        println("Exception of unknown type!");
        return 2;
    }
    return 0;
}
