# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myGame--->")

# Set projectname
project(myGame)

# add dependencies
add_subdirectory(dependencies)

# add Engine sources
add_subdirectory(myEngine)

# add core sources
add_subdirectory(src)
