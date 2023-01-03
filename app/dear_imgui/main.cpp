#include <dear_imgui.hpp>

#include <iostream>
#include <stdexcept>

auto main(int, char*[]) -> int
{
    try
    {
        ::xzr::learn::dear_imgui::run();
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cout << "exception of unknown type!\n";
        return 2;
    }
    return 0;
}
