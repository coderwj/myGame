# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myEngine--->")

# Set projectname
project(myEngine)

include_directories(./include)

if(APPLE)
	include_directories(./lib/glew/mac/include)
	include_directories(./lib/glfw/mac/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib)
endif()


FILE(GLOB SRC_ENGINE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(myEngine STATIC ${SRC_ENGINE})
SET_TARGET_PROPERTIES(myEngine PROPERTIES LINKER_LANGUAGE CPP)

if(APPLE)
	target_link_libraries(myEngine ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib/libglfw3.a ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib/libGLEW.a)
endif()
