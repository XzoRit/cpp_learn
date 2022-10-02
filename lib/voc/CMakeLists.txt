set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE ${INCLUDE}/voc)

add_library(voc)

target_sources(voc PRIVATE "${SOURCE}/voc.cpp" "${SOURCE}/voc.hpp")

target_include_directories(voc PUBLIC ${INCLUDE})

add_library(lib::voc ALIAS voc)

add_executable(voc_test)

target_sources(
  voc_test PRIVATE "${SOURCE}/voc.test.cpp" "${SOURCE}/main.test.cpp"
)

target_link_libraries(voc_test PRIVATE lib::voc Boost::unit_test_framework)

add_test(NAME voc::test COMMAND voc_test "--color_output=false")
