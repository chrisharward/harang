cmake_minimum_required (VERSION 3.5)

project(harang_graph_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(catch2 REQUIRED)

# Create the executable
add_executable(${PROJECT_NAME} main.cpp graph_test.cpp)
include_directories(../include/harang)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)