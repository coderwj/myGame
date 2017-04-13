# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake Main--->")

add_executable(Main main.cpp)

target_link_libraries(Main GameSrc)
