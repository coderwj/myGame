# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

project(Editor)

message("--->Cmake Editor--->")

add_definitions(-D_EDITOR)

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
else()
	set(CMAKE_CXX_STANDARD 14) #set use C++14
	# set(CMAKE_CXX_FLAGS "-ObjC++ --std=c++14" )
    set(CMAKE_CXX_FLAGS "--std=c++14" )
endif()

add_subdirectory(../ myEngine.out)



if(WIN32)

FILE(GLOB SRC_EDITOR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/win32/*.cpp)
FILE(GLOB_RECURSE HEAD_EDITOR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

add_executable(Editor WIN32 ${SRC_EDITOR} ${HEAD_EDITOR})

else()

FILE(GLOB SRC_EDITOR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/src/mac/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/mac/*.mm ${CMAKE_CURRENT_SOURCE_DIR}/src/mac/*.m)
FILE(GLOB_RECURSE HEAD_EDITOR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

add_executable(
    Editor
    MACOSX_BUNDLE
    ${SRC_EDITOR}
    ${HEAD_EDITOR}
)
set_source_files_properties(
    PROPERTIES
    MACOSX_PACKAGE_LOCATION
    Resources
)
set_target_properties(
    Editor
    PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mac/Info.plist
)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AppKit")

endif()

target_link_libraries(Editor myEngine tinyxml2)

add_dependencies(Editor myEngine tinyxml2)