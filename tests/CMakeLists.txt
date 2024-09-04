cmake_minimum_required(VERSION 3.11)
project(tests)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(avl avl.cpp)
target_include_directories(avl PUBLIC ${INCLUDE_DIR})

add_executable(set set.cpp)
target_include_directories(set PUBLIC ${INCLUDE_DIR})