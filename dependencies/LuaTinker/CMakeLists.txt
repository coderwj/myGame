# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake LuaTinker--->")

# Set projectname
project(LuaTinker)

include_directories(../Lua53/src)

FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(LuaTinker STATIC ${HEAD_ENGINE} ${SRC_ENGINE})
SET_TARGET_PROPERTIES(LuaTinker PROPERTIES LINKER_LANGUAGE CPP)

target_link_libraries(LuaTinker Lua53)

add_dependencies(LuaTinker Lua53)