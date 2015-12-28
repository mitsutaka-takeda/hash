cmake_minimum_required(VERSION 3.0)

project(HASH CXX C)

find_package(Boost 1.59.0 REQUIRED)

if(NOT TARGET rapidcheck)
  add_subdirectory("../rapidcheck" "${CMAKE_BINARY_DIR}/rapidcheck")
endif()

add_library(hash_lib
  hash/hash_append.cpp
  hash/hash_append.hpp
  hash/hash_traits.cpp
  hash/hash_traits.hpp
  hash/integral.cpp
  hash/integral.hpp
  hash/message_block.cpp
  hash/message_block.hpp
  hash/sha_256.cpp
  hash/sha_256.hpp
  hash/uhash.cpp
  hash/uhash.hpp
  )

set_target_properties(hash_lib
  PROPERTIES
  FOLDER "hash_lib"
  )

target_include_directories(hash_lib
  PUBLIC
  ${PROJECT_SOURCE_DIR}
  )

target_include_directories(hash_lib
  SYSTEM
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  )

target_compile_options(hash_lib
  PUBLIC
  $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
  )

add_executable(message_block_test_driver
  hash/message_block.t.cpp
)

add_executable(sha_256_test_driver
  hash/sha_256.t.cpp
)

add_executable(hash_test_driver
  hash/hash_traits.t.cpp
  )

add_executable(hash_append_test_driver
  hash/hash_append.t.cpp
  )

get_target_property(
  RapidCheckBoostTest_INCLUDE_DIRS
  rapidcheck_boost_test
  INTERFACE_INCLUDE_DIRECTORIES
)

target_include_directories(message_block_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
)

target_include_directories(sha_256_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
)

target_include_directories(hash_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
)

target_include_directories(hash_append_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
)

target_link_libraries(message_block_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

target_link_libraries(sha_256_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

target_link_libraries(hash_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

target_link_libraries(hash_append_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

set_source_files_properties(
  hash/hash_append.t.cpp
  hash/hash_traits.t.cpp
  hash/message_block.t.cpp
  hash/sha_256.t.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-padded -Wno-disabled-macro-expansion -Wno-global-constructors"
  )

set_target_properties(message_block_test_driver
  PROPERTIES
  FOLDER "message_block_test_driver"
  )

set_target_properties(sha_256_test_driver
  PROPERTIES
  FOLDER "sha_256_test_driver"
  )

set_target_properties(hash_test_driver
  PROPERTIES
  FOLDER "hash_test_driver"
  )

set_target_properties(hash_append_test_driver
  PROPERTIES
  FOLDER "hash_append_test_driver"
  )

enable_testing()

add_test(
  NAME message_block_test
  COMMAND message_block_test_driver
  )

add_test(
  NAME sha_256_test
  COMMAND sha_256_test_driver
  )

add_test(
  NAME hash_test
  COMMAND hash_test_driver
  )

add_test(
  NAME hash_append_test
  COMMAND hash_append_test_driver
  )
