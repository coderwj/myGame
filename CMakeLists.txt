# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myGame--->")

# Set projectname
project(myGame)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd")
set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MDd")

set(CMAKE_CXX_FLAGS_RELEASE "/MD /D NDEBUG")
set(CMAKE_C_FLAGS_RELEASE "/MD /D NDEBUG")



# add dependencies
add_subdirectory(dependencies)

# add Engine sources
add_subdirectory(myEngine)

# add core sources
add_subdirectory(src)
