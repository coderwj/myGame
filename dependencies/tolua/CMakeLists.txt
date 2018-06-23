# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake LuaTinker--->")

# Set projectname
project(tolua)

include_directories(../Lua53/src)

FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(tolua STATIC ${HEAD_ENGINE} ${SRC_ENGINE})

target_link_libraries(tolua Lua53)

add_dependencies(tolua Lua53)