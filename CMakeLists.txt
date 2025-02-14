cmake_minimum_required(VERSION 3.11)
project(avl_tree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./src)
add_subdirectory(./compare)

option(ENABLE_TESTS "Enable testing" ON)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(./tests)
endif()