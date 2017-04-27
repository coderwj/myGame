# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myEngine--->")

# Set projectname
project(myEngine)

include_directories(./include)
include_directories(./lib/glm/glm)

if(APPLE)
	include_directories(./lib/glew/mac/include)
	include_directories(./lib/glfw/mac/include)
	include_directories(./lib/tinyobjloader/mac/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader/mac/lib)
endif()

if(WIN32)
	include_directories(./lib/glew/win32/include)
	include_directories(./lib/glfw/win32/include)
	include_directories(./lib/tinyobjloader/win32/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader/win32/lib)
endif()


FILE(GLOB SRC_ENGINE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(myEngine STATIC ${SRC_ENGINE})
SET_TARGET_PROPERTIES(myEngine PROPERTIES LINKER_LANGUAGE CPP)

if(APPLE)
	target_link_libraries(myEngine ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib/libglfw3.a ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib/libGLEW.a
		${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader/mac/lib/libtinyobjloader.a)


	FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
	message("--------find OPENGL_LIBRARY--->" ${OPENGL_LIBRARY})

	FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	message("--------find COCOA_LIBRARY--->" ${COCOA_LIBRARY})

	FIND_LIBRARY(IOKIT_LIBRARY IOKit)
	message("--------find IOKIT_LIBRARY--->" ${IOKIT_LIBRARY})

	FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
	message("--------find COREVIDEO_LIBRARY--->" ${COREVIDEO_LIBRARY})

	target_link_libraries(myEngine ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif()

if(WIN32)
	target_link_libraries(myEngine ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/win32/lib/glfw3.lib ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/win32/lib/GLEW.lib
		${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader/win32/lib/tinyobjloader.lib)
endif()
