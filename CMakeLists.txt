cmake_minimum_required(VERSION 3.0)

project(HASH CXX C)

add_executable(hash_test_driver hash/hash_traits.cpp)

target_include_directories(hash_test_driver
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  )

target_compile_options(hash_test_driver
  PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Werror -Weverything -Wno-c++98-compat>
  )

set_target_properties(hash_test_driver
  PROPERTIES
  FOLDER "hash_test_driver"
  )

enable_testing()

add_test(
  NAME hash_test
  COMMAND hash_test_driver)
