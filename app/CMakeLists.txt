add_executable(app_exe)
target_sources(app_exe PRIVATE "main.cpp" "console.hpp" "console.cpp" "persist.hpp" "persist.cpp" "commands.hpp" "commands.cpp")
target_link_libraries(app_exe PRIVATE lib::data Boost::program_options)
add_test(NAME app::exe COMMAND app_exe)
