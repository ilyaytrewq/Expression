cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Expression)

include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

include_directories(include)

add_executable(tests test/test.cpp)
target_link_libraries(tests gtest gtest_main)

add_executable(differentiator differentiator.cpp)

add_custom_target(test
    COMMAND tests
    DEPENDS tests 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests"
)

add_custom_target(build
    DEPENDS differentiator tests
    COMMENT "Building all targets"
)