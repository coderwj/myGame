# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myEngine--->")

# Set projectname
project(myEngine)

add_subdirectory(lib/tinyxml2)
add_subdirectory(lib/imgui)

include_directories(./include)
include_directories(./lib/glm/glm)
include_directories(./lib/tinyxml2)
include_directories(./lib/imgui)

if(APPLE)
	include_directories(./lib/glew/mac/include)
	include_directories(./lib/glfw/mac/include)
	include_directories(./lib/assimp/mac/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/mac/lib)
endif()

if(WIN32)
	include_directories(./lib/glew/win32/include)
	include_directories(./lib/glfw/win32/include)
	include_directories(./lib/assimp/win32/include)
	include_directories(./lib/fbxsdk/win32/include)

	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/win32/lib)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/win32/lib)
	set(WINDOWS_PLATTYPE .)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(WINDOWS_PLATTYPE x86)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/fbxsdk/win32/lib/vs2015/x86/debug)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(WINDOWS_PLATTYPE x64)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/fbxsdk/win32/lib/vs2015/x64/debug)
		# message("${CMAKE_CURRENT_SOURCE_DIR}/lib/fbxsdk/win32/lib/vs2015/x64/${CMAKE_BUILD_TYPE}")
	endif()
	message("WINDOWS_PLATTYPE----------------->${WINDOWS_PLATTYPE}")
endif()


FILE(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
FILE(GLOB_RECURSE HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.inl)

add_library(myEngine STATIC ${HEAD_ENGINE} ${SRC_ENGINE})
#SET_TARGET_PROPERTIES(myEngine PROPERTIES LINKER_LANGUAGE CPP)

if(APPLE)

	if(IOS)
		message("myEngine---------------choose ios -------")
		target_link_libraries(myEngine
			${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib/libglfw3.a
			${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib/libGLEW.a
			${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/mac/lib/libassimp.dylib
			imgui)
		add_dependencies(myEngine imgui tinyxml2)
	else()
		add_subdirectory(lib/ozz-animation)
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
			${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/mac/lib/libglfw3.a
			${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/mac/lib/libGLEW.a
			${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/mac/lib/libassimp.dylib
			imgui
			ozz_animation)
		add_dependencies(myEngine imgui tinyxml2 ozz_animation)
	endif()
endif()

if(WIN32)
	add_subdirectory(lib/ozz-animation)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/common)

	target_link_libraries(myEngine
		${CMAKE_CURRENT_SOURCE_DIR}/lib/common/OpenGL32.Lib
		${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/win32/lib/glfw3.lib
		${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/win32/lib/glew32.lib
		${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/win32/lib/glew32s.lib
		${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/win32/lib/assimp.lib
		${CMAKE_CURRENT_SOURCE_DIR}/lib/fbxsdk/win32/lib/vs2015/${WINDOWS_PLATTYPE}/debug/libfbxsdk.lib
		imgui
		ozz_animation)
	add_dependencies(myEngine imgui tinyxml2 ozz_animation)
endif()

