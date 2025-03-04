cmake_minimum_required(VERSION 3.19)
project(Expression)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Expression)
add_subdirectory(Expression)

add_executable(main main.cpp)
target_link_libraries(main Expression)

# add_executable(test tests.cpp)
# target_link_libraries(test Expression)