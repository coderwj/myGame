# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

# add dependencies
add_subdirectory(gameSrc)

if(WIN32)
	add_subdirectory(platform/win)
elseif(APPLE)
	add_subdirectory(platform/mac)
else()
	add_subdirectory(platform/android)
endif()
