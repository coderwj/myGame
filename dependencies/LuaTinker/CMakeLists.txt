# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake LuaTinker--->")

# Set projectname
project(LuaTinker)


FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(LuaTinker STATIC ${HEAD_ENGINE} ${SRC_ENGINE})
SET_TARGET_PROPERTIES(LuaTinker PROPERTIES LINKER_LANGUAGE CPP)

FIND_LIBRARY(LUA_LIBRARY lua51)
message("--------find LUA_LIBRARY--->" ${LUA_LIBRARY})

target_link_libraries(LuaTinker ${LUA_LIBRARY})
