# We require CMake >= 2.8
cmake_minimum_required(VERSION 2.8)

message("--->Cmake myGame--->")

# Set projectname
project(myGame)

# if(NOT DEFINED ENV{ENGINE_PATH})
#     message(FATAL_ERROR "can't find ENGINE_PATH")
# endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release)

if(WIN32)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /MDd /Zi /EHsc /W3 /Od")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /MDd /Zi /EHsc /W3 /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /D NDEBUG /Zi /EHsc")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD /D NDEBUG /Zi /EHsc")
elseif(APPLE)
	OPTION(IOS "" TRUE)
endif()

# add dependencies
add_subdirectory(dependencies)

# add Engine sources
add_subdirectory(myEngine)

# add core sources
add_subdirectory(src)
