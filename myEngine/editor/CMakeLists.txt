# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

project(Editor)

message("--->Cmake Editor--->")

include_directories(./src)
include_directories(../src)
include_directories(../lib/imgui)
#include_directories(../lib/bgfx.cmake/bgfx/include)
#include_directories(../lib/bgfx.cmake/bimg/include)
#include_directories(../lib/bgfx.cmake/bx/include)
include_directories(../lib/tinyxml2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

if(WIN32)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /MDd /Zi /EHsc /W3 /Od")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /MDd /Zi /EHsc /W3 /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /D NDEBUG /Zi /EHsc")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD /D NDEBUG /Zi /EHsc")
endif()

add_subdirectory(../ myEngine.out)


FILE(GLOB SRC_EDITOR "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE HEAD_EDITOR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

add_executable(Editor WIN32 ${SRC_EDITOR} ${HEAD_EDITOR})

target_link_libraries(Editor myEngine tinyxml2)

add_dependencies(Editor myEngine tinyxml2)