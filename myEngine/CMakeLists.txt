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
        add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=31 -DBGFX_CONFIG_MULTITHREADED=0)
    endif()
endif()

add_subdirectory(lib/freetype-2.9.1)
add_subdirectory(lib/tinyxml2)
add_subdirectory(lib/imgui)
add_subdirectory(lib/bgfx.cmake)

set(INC_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/anim
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    )


file(GLOB SRC_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cc)
file(GLOB HEAD_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl)

add_library(myEngine STATIC ${HEAD_ENGINE} ${SRC_ENGINE})

target_include_directories(myEngine PUBLIC ${INC_DIRS})

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
