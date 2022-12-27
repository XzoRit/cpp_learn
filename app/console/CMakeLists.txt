set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE ${INCLUDE}/console)

add_library(console)
target_sources(
  console
  PRIVATE "${SOURCE}/commands.cpp"
          "${SOURCE}/commands.hpp"
          "${SOURCE}/console.cpp"
          "${SOURCE}/console.hpp"
          "${SOURCE}/model.cpp"
          "${SOURCE}/model.hpp"
          "${SOURCE}/persist.cpp"
          "${SOURCE}/persist.hpp"
)
target_include_directories(console PUBLIC ${INCLUDE})
target_link_libraries(console PUBLIC lib::data Boost::serialization)
add_library(lib::console ALIAS console)

add_executable(console_test)
target_sources(
  console_test PRIVATE "${SOURCE}/main.test.cpp"
                       "${SOURCE}/commands.test.cpp"
)
target_link_libraries(
  console_test PRIVATE lib::console lib::data Boost::unit_test_framework
)
add_test(NAME console::test COMMAND console_test "--color_output=false")
