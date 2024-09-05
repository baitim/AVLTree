cmake_minimum_required(VERSION 3.11)
project(end_to_end)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(avl avl.cpp)
target_include_directories(avl PUBLIC ${INCLUDE_DIR})