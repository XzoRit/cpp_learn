cmake_minimum_required(VERSION 3.21)

project(
  cpp_learn
  VERSION 0.0
  DESCRIPTION "learning with cards"
)

enable_testing()

add_subdirectory(app/console)
add_subdirectory(lib/data)
add_subdirectory(lib/view)
add_subdirectory(external)
