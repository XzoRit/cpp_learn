set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE ${INCLUDE}/view)
add_library(view)
target_sources(
  view PRIVATE "${SOURCE}/data.cpp" "${SOURCE}/data.hpp"
               "${SOURCE}/persist.cpp" "${SOURCE}/persist.hpp"
)
target_include_directories(view PUBLIC ${INCLUDE})
target_link_libraries(view PUBLIC lib::data Boost::serialization)
add_library(lib::view ALIAS view)

add_executable(view_test)
target_sources(
  view_test PRIVATE "${SOURCE}/main.test.cpp" "${SOURCE}/data.test.cpp"
)
target_link_libraries(
  view_test PRIVATE lib::view lib::data Boost::unit_test_framework
)
add_test(NAME view::test COMMAND view_test "--color_output=false")
