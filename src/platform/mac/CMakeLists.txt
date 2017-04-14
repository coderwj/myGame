# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake Main--->")

include_directories(../../../include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../build/src/gameSrc)

add_executable(Main main.cpp)

add_dependencies(Main GameSrc)

target_link_libraries(Main GameSrc)
