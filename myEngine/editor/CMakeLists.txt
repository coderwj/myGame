# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

project(myEditor)

message("--->Cmake Editor--->")

include_directories(./src)
include_directories(../src)
include_directories(../lib/imgui)

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

add_executable(Editor WIN32 src/main.cpp)

target_link_libraries(myEngine)

add_dependencies(Editor myEngine)