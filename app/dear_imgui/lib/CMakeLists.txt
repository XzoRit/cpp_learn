find_package(Boost REQUIRED COMPONENTS unit_test_framework)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE ${INCLUDE})
add_library(dear_imgui)
target_sources(
  dear_imgui PRIVATE "${SOURCE}/dear_imgui.cpp" "${SOURCE}/dear_imgui.hpp"
)
target_include_directories(dear_imgui PUBLIC ${INCLUDE})
target_link_libraries(dear_imgui)
add_library(lib::dear_imgui ALIAS dear_imgui)

add_executable(dear_imgui_test)
target_sources(
  dear_imgui_test PRIVATE "${SOURCE}/main.test.cpp"
                          "${SOURCE}/dear_imgui.test.cpp"
)
target_link_libraries(
  dear_imgui_test PRIVATE lib::dear_imgui Boost::unit_test_framework
)
add_test(NAME dear_imgui::test COMMAND dear_imgui_test "--color_output=false")
