# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake Main--->")

include_directories(../../../src/gameSrc)
include_directories(../../../myEngine/src)

include_directories(../../../myEngine/lib/imgui)

add_executable(Main WIN32 main.cpp)

target_link_libraries(Main GameSrc myEngine)

add_dependencies(Main GameSrc myEngine)
