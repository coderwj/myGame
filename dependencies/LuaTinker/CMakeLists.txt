# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake LuaTinker--->")

# Set projectname
project(LuaTinker)

include_directories(../Lua51/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Lua51/lib)

FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(LuaTinker STATIC ${HEAD_ENGINE} ${SRC_ENGINE})
SET_TARGET_PROPERTIES(LuaTinker PROPERTIES LINKER_LANGUAGE CPP)

target_link_libraries(LuaTinker ${CMAKE_CURRENT_SOURCE_DIR}/../Lua51/lib/lua51.lib)
