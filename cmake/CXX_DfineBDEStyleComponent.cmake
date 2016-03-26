macro(define_bde_style_component name)
  add_library(
    ${name}
    ${name}.hpp
    ${name}.cpp
    )
  target_compile_options(
    ${name}
    PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Wall -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>    
    )
  target_include_directories(
    ${name}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
  add_executable(
    ${name}.t
    ${name}.t.cpp
    )
  target_compile_options(
    ${name}.t
    PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-std=c++14 -Wall -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wno-disabled-macro-expansion -Wno-global-constructors>
    )
  target_link_libraries(
    ${name}.t
    PRIVATE
    ${name}
    )
  target_include_directories(
    ${name}.t
    SYSTEM
    PRIVATE
    ${Boost_INCLUDE_DIRS}
    )
  add_test(
    NAME ${name}.t
    COMMAND ${name}.t
    )
  add_custom_command(
    TARGET ${name}.t
    POST_BUILD
    COMMAND ctest -R ${name}.t --output-on-failure
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endmacro()
