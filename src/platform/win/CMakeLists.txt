# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake Main--->")

include_directories(../../../include)
include_directories(../../../myEngine/include)

include_directories(../../../myEngine/lib/glm/glm)

if(APPLE)
	include_directories(../../../myEngine/lib/glew/mac/include)
	include_directories(../../../myEngine/lib/glfw/mac/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../myEngine/lib/glew/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../myEngine/lib/glfw/mac/lib)
endif()

if(WIN32)
	include_directories(../../../myEngine/lib/glew/win32/include)
	include_directories(../../../myEngine/lib/glfw/win32/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../myEngine/lib/glew/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../myEngine/lib/glfw/win32/lib)
endif()

add_executable(Main main.cpp)

target_link_libraries(Main GameSrc myEngine)

add_dependencies(Main GameSrc myEngine)
