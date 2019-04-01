# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myEngine--->")

# Set projectname
project(myEngine)

if(WIN32)
	add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=31)
elseif(APPLE)
	if(IOS)

	else()
		add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=21)
	endif()
endif()

add_subdirectory(lib/freetype-2.9.1)
add_subdirectory(lib/tinyxml2)
add_subdirectory(lib/imgui)
add_subdirectory(lib/bgfx.cmake)

include_directories(./lib/tinyxml2)
include_directories(./lib/imgui)
include_directories(./lib/bgfx.cmake/brtshaderc)
#include_directories(./lib/freetype-2.9.1/include)
#include_directories(./lib/bgfx.cmake/bgfx/include)
#include_directories(./lib/bgfx.cmake/bx/include)
#include_directories(./lib/bgfx.cmake/bimg/include)

FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl)

add_library(myEngine STATIC ${HEAD_ENGINE} ${SRC_ENGINE})
#SET_TARGET_PROPERTIES(myEngine PROPERTIES LINKER_LANGUAGE CPP)

target_link_libraries(myEngine
	freetype
	tinyxml2
	imgui
	bgfx
	brtshaderc)
add_dependencies(myEngine
	freetype
	tinyxml2
	imgui
	bgfx
	brtshaderc)
