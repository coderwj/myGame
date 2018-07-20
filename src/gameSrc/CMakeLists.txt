# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake GameSrc--->")

# Set projectname
project(GameSrc)

include_directories(../../myEngine/src)
include_directories(../../myEngine/lib/tinyxml2)
include_directories(../../myEngine/lib/imgui)
include_directories(../../dependencies/Lua53/src)
include_directories(../../dependencies/LuaTinker)
include_directories(../../dependencies/tolua)

FILE(GLOB SRC_1 "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
FILE(GLOB_RECURSE HEAD_GAMESRC ${CMAKE_CURRENT_SOURCE_DIR}/*.h )

add_library(GameSrc STATIC ${SRC_1} ${HEAD_GAMESRC})

SET(LIBRARIES myEngine)
target_link_libraries(GameSrc ${LIBRARIES} tinyxml2 LuaTinker Lua53 tolua)

add_dependencies(GameSrc myEngine tinyxml2 LuaTinker Lua53 tolua)
