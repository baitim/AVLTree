cmake_minimum_required(VERSION 3.11)
project(compare)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(cmp_avl avl_time.cpp benchmark.hpp)
target_sources(cmp_avl
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${INCLUDE_DIR}
)

add_executable(cmp_set set_time.cpp benchmark.hpp)
target_sources(cmp_set
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${INCLUDE_DIR}
)