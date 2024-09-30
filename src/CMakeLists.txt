cmake_minimum_required(VERSION 3.11)
project(src)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(avl avl.cpp)
target_include_directories(avl PUBLIC ${INCLUDE_DIR})

add_executable(set set.cpp)
target_include_directories(set PUBLIC ${INCLUDE_DIR})

set(PYTHON_SCRIPT_GEN "${CMAKE_SOURCE_DIR}/tests/end_to_end/generate.py")
set(PYTHON_SCRIPT_RUN "${CMAKE_SOURCE_DIR}/tests/end_to_end/run_tests.py")
add_test(
    NAME end_to_end_avl_target
    COMMAND bash -c "python3 ${PYTHON_SCRIPT_GEN} & \
                     python3 ${PYTHON_SCRIPT_RUN}"
)