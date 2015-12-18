cmake_minimum_required(VERSION 3.0)

project(HASH CXX C)

find_package(Boost 1.59.0 REQUIRED)

if(NOT TARGET rapidcheck)
  add_subdirectory("../rapidcheck" "${CMAKE_BINARY_DIR}/rapidcheck")
endif()

add_executable(hash_test_driver
  hash/hash_traits.cpp
  hash/hash_traits.t.cpp
  )

target_include_directories(hash_test_driver
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  )

get_target_property(
  RapidCheckBoostTest_INCLUDE_DIRS
  rapidcheck_boost_test
  INTERFACE_INCLUDE_DIRECTORIES
)

target_include_directories(hash_test_driver
  SYSTEM
  PRIVATE
  ${Boost_INCLUDE_DIRS}
  ${RapidCheckBoostTest_INCLUDE_DIRS}
)

target_link_libraries(hash_test_driver
  rapidcheck
  rapidcheck_boost_test
  )

target_compile_options(hash_test_driver
  PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
  )
set_source_files_properties(
  hash/hash_traits.t.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-padded -Wno-disabled-macro-expansion -Wno-global-constructors"
  )
set_target_properties(hash_test_driver
  PROPERTIES
  FOLDER "hash_test_driver"
  )

enable_testing()

add_test(
  NAME hash_test
  COMMAND hash_test_driver
  )
