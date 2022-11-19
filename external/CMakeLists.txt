add_subdirectory(boost)

add_library(
  zug
  INTERFACE
)

target_include_directories(
  zug
  INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/zug"
)

target_compile_features(
  zug
  INTERFACE
    cxx_std_20
)

add_library(
  lager
  INTERFACE
)

target_include_directories(
  lager
  INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/lager"
)

target_compile_features(
  lager
  INTERFACE
    cxx_std_20
)

target_link_libraries(
  lager
  INTERFACE
    Boost::boost
    zug
)

add_library(
  immer
  INTERFACE
)

target_include_directories(
  immer
  INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/immer"
)

target_compile_features(
  immer
  INTERFACE
    cxx_std_20
)
