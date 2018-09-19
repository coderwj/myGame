# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myEngine--->")

# Set projectname
project(myEngine)

add_subdirectory(lib/tinyxml2)
add_subdirectory(lib/imgui)
add_subdirectory(lib/bgfx.cmake)

include_directories(./lib/tinyxml2)
include_directories(./lib/imgui)
include_directories(./lib/bgfx.cmake/include)

FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl)

add_library(myEngine STATIC ${HEAD_ENGINE} ${SRC_ENGINE})
#SET_TARGET_PROPERTIES(myEngine PROPERTIES LINKER_LANGUAGE CPP)

if(APPLE)
	if(IOS)
		message("myEngine---------------choose ios -------")
		add_dependencies(myEngine imgui tinyxml2)
	else()
		message("myEngine---------------choose mac -------")
		FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
		message("--------find OPENGL_LIBRARY--->" ${OPENGL_LIBRARY})

		FIND_LIBRARY(COCOA_LIBRARY Cocoa)
		message("--------find COCOA_LIBRARY--->" ${COCOA_LIBRARY})

		FIND_LIBRARY(IOKIT_LIBRARY IOKit)
		message("--------find IOKIT_LIBRARY--->" ${IOKIT_LIBRARY})

		FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
		message("--------find COREVIDEO_LIBRARY--->" ${COREVIDEO_LIBRARY})

		target_link_libraries(myEngine ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY}
			tinyxml2
			imgui
			bgfx)
		add_dependencies(myEngine imgui tinyxml2 bgfx)
	endif()
endif()

if(WIN32)
	#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/common)

	target_link_libraries(myEngine
		tinyxml2
		imgui
		bgfx)
	add_dependencies(myEngine imgui tinyxml2 bgfx)
endif()
