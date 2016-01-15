cmake_minimum_required(VERSION 3.0)

project(HASH CXX C)

find_package(Boost 1.59.0 REQUIRED)

if(NOT TARGET rapidcheck)
  add_subdirectory("../rapidcheck" "${CMAKE_BINARY_DIR}/rapidcheck")
endif()

set(LIB_FUZZER_SRC_PATH "" CACHE PATH "The path to the root of LibFuzzer directory. This directory must contain
cpp files for LibFuzzer. If you want to use LibFuzzer in the LLVM repository, set LIB_FUZZER_SRC_PATH to 
llvm/lib/Fuzzer.")

if((NOT TARGET libfuzzer) AND LIB_FUZZER_SRC_PATH)
  message(STATUS "Enable LibFuzzer at ${LIB_FUZZER_SRC_PATH}")
  # CMakeLists.txt in the llvm's LibFuzzer requires the variable LLVM_USE_SANITIZE_COVERAGE
  # set to ON.
  set(LLVM_USE_SANITIZE_COVERAGE ON)
  add_subdirectory(${LIB_FUZZER_SRC_PATH} "${CMAKE_BINARY_DIR}/libfuzzer")
  target_compile_options(LLVMFuzzerNoMainObjects
    PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-std=c++11>
    )
  target_compile_options(LLVMFuzzer
    PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-std=c++11>
    )
else()
  message(STATUS "Disable LibFuzzer. To enable it, set LIB_FUZZER_SRC_PATH")
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

set_source_files_properties(
  hash/sha_256.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-padded -Wno-disabled-macro-expansion -Wno-global-constructors"
  )

get_target_property(
  RapidCheckBoostTest_INCLUDE_DIRS
  rapidcheck_boost_test
  INTERFACE_INCLUDE_DIRECTORIES
  )

# Testing
enable_testing()

# Test for message_block
add_executable(message_block_test_driver
  hash/message_block.t.cpp
  )

set_source_files_properties(
  hash/message_block.t.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-disabled-macro-expansion  -Wno-global-constructors"
  )

target_include_directories(message_block_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
  )

target_link_libraries(message_block_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

set_target_properties(message_block_test_driver
  PROPERTIES
  FOLDER "message_block_test_driver"
  )

add_test(
  NAME message_block_test
  COMMAND message_block_test_driver
  )

# Test for sha_256
add_executable(sha_256_test_driver
  hash/sha_256.t.cpp
  )

set_source_files_properties(
  hash/sha_256.t.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-padded -Wno-disabled-macro-expansion -Wno-global-constructors"
  )

target_include_directories(sha_256_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
  )

target_link_libraries(sha_256_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

set_target_properties(sha_256_test_driver
  PROPERTIES
  FOLDER "sha_256_test_driver"
  )

add_test(
  NAME sha_256_test
  COMMAND sha_256_test_driver
  )

# Test for hash
add_executable(hash_test_driver
  hash/hash_traits.t.cpp
  )

set_source_files_properties(
  hash/hash_traits.t.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-padded -Wno-disabled-macro-expansion -Wno-global-constructors"
  )

target_include_directories(hash_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
  )

target_link_libraries(hash_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

set_target_properties(hash_test_driver
  PROPERTIES
  FOLDER "hash_test_driver"
  )

add_test(
  NAME hash_test
  COMMAND hash_test_driver
  )

# Test for hash_append
add_executable(hash_append_test_driver
  hash/hash_append.t.cpp
  )

set_source_files_properties(
  hash/hash_append.t.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-padded -Wno-disabled-macro-expansion -Wno-global-constructors"
  )

target_include_directories(hash_append_test_driver
  SYSTEM
  PRIVATE
  ${RapidCheckBoostTest_INCLUDE_DIRS}
  )

target_link_libraries(hash_append_test_driver
  hash_lib
  rapidcheck
  rapidcheck_boost_test
  )

set_target_properties(hash_append_test_driver
  PROPERTIES
  FOLDER "hash_append_test_driver"
  )

add_test(
  NAME hash_append_test
  COMMAND hash_append_test_driver
  )

# Library for Fuzzing
get_target_property(
  HashLib_SOURCES
  hash_lib
  SOURCES
  )
add_library(hash_lib_for_fuzzing
  ${HashLib_SOURCES}
  )
set_target_properties(hash_lib_for_fuzzing
  PROPERTIES
  FOLDER "hash_lib_for_fuzzing"
  )
target_include_directories(hash_lib_for_fuzzing
  PUBLIC
  ${PROJECT_SOURCE_DIR}
  )
target_include_directories(hash_lib_for_fuzzing
  SYSTEM
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  )
target_compile_options(hash_lib_for_fuzzing
  PUBLIC
  $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -fsanitize-coverage=edge,indirect-calls -fsanitize=address>
  )

# Fuzzing for sha-256
add_executable(sha_256_fuzzer_driver
  fuzzing/sha_256_fuzzer.cpp
  )

target_compile_options(sha_256_fuzzer_driver
  PUBLIC
  $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -fsanitize-coverage=edge,indirect-calls -fsanitize=address>
  )

target_link_libraries(sha_256_fuzzer_driver
  hash_lib_for_fuzzing
  LLVMFuzzer
  )

set_target_properties(sha_256_fuzzer_driver
  PROPERTIES
  LINK_FLAGS "-fsanitize=address -fsanitize-memory-track-origins"
  )

add_test(
  NAME sha_256_fuzzer
  COMMAND sha_256_fuzzer_driver
  )
