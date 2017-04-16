# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake Main--->")

include_directories(../../../include)

add_executable(Main main.cpp)

add_dependencies(Main GameSrc)

target_link_libraries(Main GameSrc)
