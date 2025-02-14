cmake_minimum_required(VERSION 3.11)
project(src)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(avl avl.cpp)
target_sources(avl
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${INCLUDE_DIR}
)

add_executable(set set.cpp)
target_sources(set
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${INCLUDE_DIR}
)

add_library(AVLTree avl.cpp)
target_include_directories(AVLTree PUBLIC
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS AVLTree EXPORT AVLTreeTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY ${INCLUDE_DIR}/AVLTree DESTINATION include)

install(EXPORT AVLTreeTargets
    FILE AVLTreeConfig.cmake
    DESTINATION lib/cmake/AVLTree
)