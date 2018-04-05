# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

# add dependencies
add_subdirectory(gameSrc)

if(WIN32)
	add_subdirectory(platform/win)
elseif(APPLE)
	if(IOS)
		message("---------------choose ios -------")
		add_subdirectory(platform/ios)
	else()
		message("---------------choose mac -------")
		add_subdirectory(platform/mac)
	endif()
else()
	add_subdirectory(platform/android)
endif()
