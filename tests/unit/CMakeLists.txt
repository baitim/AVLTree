cmake_minimum_required(VERSION 3.11)
project(unit)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${gtest_SOURCE_DIR})

add_executable(unit_avl avl_unit_test.cpp)
target_include_directories(unit_avl PUBLIC ${INCLUDE_DIR})
target_link_libraries(unit_avl gtest gtest_main)

set(RUN_TESTS ./unit_avl --gtest_color=yes)
add_test(
    NAME unit_avl_target
    COMMAND ${RUN_TESTS}
)