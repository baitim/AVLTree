cmake_minimum_required(VERSION 3.11)
project(unit)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

enable_testing()
include_directories(${gtest_SOURCE_DIR})

add_executable(unit_avl avl_unit_test.cpp custom.h)
target_include_directories(unit_avl PUBLIC ${INCLUDE_DIR})
target_link_libraries(unit_avl gtest gtest_main)

execute_process(COMMAND ./build/tests/unit/unit_avl)