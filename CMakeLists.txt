# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myGame--->")

# Set projectname
project(myGame)

# if(NOT DEFINED ENV{ENGINE_PATH})
#     message(FATAL_ERROR "can't find ENGINE_PATH")
# endif()
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scriptAndTools/cmake)

if(WIN32)

	set(CMAKE_CONFIGURATION_TYPES Debug Release)

	set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi")
	set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MDd /Zi")

	set(CMAKE_CXX_FLAGS_RELEASE "/MD /D NDEBUG /Zi")
	set(CMAKE_C_FLAGS_RELEASE "/MD /D NDEBUG /Zi")

endif()


# add dependencies
add_subdirectory(dependencies)

# add Engine sources
add_subdirectory(myEngine)

# add core sources
add_subdirectory(src)
