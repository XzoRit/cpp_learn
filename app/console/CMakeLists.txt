find_package(Boost REQUIRED COMPONENTS program_options)
add_subdirectory(lib)
add_executable(app_exe)
target_sources(app_exe PRIVATE "main.cpp")
target_link_libraries(
  app_exe PRIVATE lib::console lib::view Boost::program_options
)
add_test(NAME app::exe COMMAND app_exe)
