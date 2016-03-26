cmake_minimum_required(VERSION 3.0)

project(HASH CXX C)

find_package(Boost 1.59.0 REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CXX_DfineBDEStyleComponent)

if(NOT TARGET rapidcheck)
  add_subdirectory("../rapidcheck" "${CMAKE_BINARY_DIR}/rapidcheck")
  set_target_properties(
    rapidcheck
    PROPERTIES
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:rapidcheck,INTERFACE_INCLUDE_DIRECTORIES>
    )
  set_target_properties(
    rapidcheck_boost_test
    PROPERTIES
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:rapidcheck_boost_test,INTERFACE_INCLUDE_DIRECTORIES>
    )
endif()

option(
  ENABLE_LIB_FUZZER
  "Enable fuzzing tests with LibFuzzer. You need to either provide a library target
LLVMFUZZER or set a variable LIB_FUZZER_SRC_PATH. See the help documentation for
LIB_FUZZER_SRC_PATH."
  OFF
)

if(ENABLE_LIB_FUZZER)
  set(LIB_FUZZER_SRC_PATH "" CACHE PATH 
    "The path to the root of LibFuzzer directory. This directory must contain
cpp files for LibFuzzer. If you want to use LibFuzzer in the LLVM repository, 
set LIB_FUZZER_SRC_PATH to llvm/lib/Fuzzer."
    )

  if((NOT TARGET LLVMFUZZER) AND LIB_FUZZER_SRC_PATH)
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
    message(SEND_ERROR "To use LibFuzzer, you must either provide a LLVMFUZZER target or set LIB_FUZZER_SRC_PATH.")
  endif()
endif()

# Testing
enable_testing()

add_subdirectory(hash)

# Fuzzing for sha-256
if(ENABLE_LIB_FUZZER)
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
endif()
