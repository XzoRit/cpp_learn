set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE ${INCLUDE}/data)

add_library(data)
target_sources(
  data
  PRIVATE "${SOURCE}/books.cpp"
          "${SOURCE}/books.hpp"
          "${SOURCE}/training.cpp"
          "${SOURCE}/training.hpp"
          "${SOURCE}/action.cpp"
          "${SOURCE}/action.hpp"
          "${SOURCE}/update.cpp"
          "${SOURCE}/update.hpp"
          "${SOURCE}/app.cpp"
          "${SOURCE}/app.hpp"
          "${SOURCE}/serialize.cpp"
          "${SOURCE}/serialize.hpp"
          "${SOURCE}/ostream.cpp"
          "${SOURCE}/ostream.hpp"
)
target_include_directories(data PUBLIC ${INCLUDE})
target_link_libraries(data PUBLIC Boost::serialization)
add_library(lib::data ALIAS data)

add_executable(data_test)
target_sources(
  data_test PRIVATE "${SOURCE}/books.test.cpp" "${SOURCE}/training.test.cpp" "${SOURCE}/update.test.cpp"
                    "${SOURCE}/serialize.test.cpp" "${SOURCE}/main.test.cpp"
)
target_link_libraries(
  data_test PRIVATE lib::data Boost::unit_test_framework Boost::serialization
)
add_test(NAME data::test COMMAND data_test "--color_output=false")
