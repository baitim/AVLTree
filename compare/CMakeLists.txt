cmake_minimum_required(VERSION 3.11)
project(compare)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(cmp_avl avl_time.cpp benchmark.hpp)
target_include_directories(cmp_avl PUBLIC ${INCLUDE_DIR})

add_executable(cmp_set set_time.cpp benchmark.hpp)
target_include_directories(cmp_set PUBLIC ${INCLUDE_DIR})