find_package(
  Boost REQUIRED COMPONENTS program_options unit_test_framework serialization
)

if(Boost_FOUND)
  set_target_properties(
    Boost::boost Boost::program_options Boost::unit_test_framework
    Boost::serialization PROPERTIES IMPORTED_GLOBAL true
  )
endif()
