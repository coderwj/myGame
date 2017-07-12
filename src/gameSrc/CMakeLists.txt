# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake GameSrc--->")

# Set projectname
project(GameSrc)

include_directories(../../include)
include_directories(../../myEngine/include)
include_directories(../../myEngine/lib/glm/glm)
include_directories(../../myEngine/lib/tinyxml2)

if(APPLE)
	include_directories(../../myEngine/lib/glew/mac/include)
	include_directories(../../myEngine/lib/glfw/mac/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../myEngine/lib/glew/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../myEngine/lib/glfw/mac/lib)
endif()

if(WIN32)
	include_directories(../../myEngine/lib/glew/win32/include)
	include_directories(../../myEngine/lib/glfw/win32/include)
	include_directories(../../myEngine/lib/assimp/win32/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../myEngine/lib/glew/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../myEngine/lib/glfw/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../myEngine/lib/assimp/win32/lib)
endif()


FILE(GLOB SRC_1 "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(GameSrc STATIC ${SRC_1})

SET(LIBRARIES myEngine)

target_link_libraries(GameSrc ${LIBRARIES} tinyxml2)

add_dependencies(GameSrc myEngine tinyxml2)
