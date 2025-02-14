cmake_minimum_required(VERSION 3.11)
project(end_to_end)

set(PYTHON_SCRIPT_RUN "${CMAKE_SOURCE_DIR}/tests/end_to_end/run_tests.py")
add_test(
    NAME end_to_end_avl_target
    COMMAND bash -c "python3 ${PYTHON_SCRIPT_RUN}"
)