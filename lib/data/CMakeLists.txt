find_package(Boost REQUIRED COMPONENTS unit_test_framework serialization)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE ${INCLUDE}/data)
add_library(data)
target_sources(
  data
  PRIVATE "${SOURCE}/books.cpp"
          "${SOURCE}/books.hpp"
          "${SOURCE}/training.cpp"
          "${SOURCE}/training.hpp"
          "${SOURCE}/data.cpp"
          "${SOURCE}/data.hpp"
          "${SOURCE}/serialize.cpp"
          "${SOURCE}/serialize.hpp"
          "${SOURCE}/serialize_generic.cpp"
          "${SOURCE}/serialize_generic.hpp"
          "${SOURCE}/serialize_std_variant.cpp"
          "${SOURCE}/serialize_std_variant.hpp"
          "${SOURCE}/ostream.cpp"
          "${SOURCE}/ostream.hpp"
          "${SOURCE}/ostream_generic.cpp"
          "${SOURCE}/ostream_generic.hpp"
          "${SOURCE}/ostream_std.cpp"
          "${SOURCE}/ostream_std.hpp"
          "${SOURCE}/reflect.cpp"
          "${SOURCE}/reflect.hpp"
)
target_include_directories(data PUBLIC ${INCLUDE})
target_link_libraries(data PUBLIC Boost::serialization)
add_library(lib::data ALIAS data)

add_executable(data_test)
target_sources(
  data_test
  PRIVATE "${SOURCE}/books.test.cpp" "${SOURCE}/training.test.cpp"
          "${SOURCE}/serialize.test.cpp" "${SOURCE}/ostream_generic.test.cpp"
          "${SOURCE}/main.test.cpp"
)
target_link_libraries(
  data_test PRIVATE lib::data Boost::unit_test_framework Boost::serialization
)
add_test(NAME data::test COMMAND data_test "--color_output=false")
