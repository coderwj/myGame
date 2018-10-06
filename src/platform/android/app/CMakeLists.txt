# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

message("--->Cmake native-activity--->")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

find_library(log-lib log)
find_library(OPENGLES3_LIBRARY GLESv3 "OpenGL ES v3.0 library")
find_library(EGL_LIBRARY EGL "EGL 1.4 library")

# set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c )

add_subdirectory(../../../../dependencies androidDepend.out)
add_subdirectory(../../../gameSrc androidGameSrc.out)
add_subdirectory(../../../../myEngine androidmyEngine.out)

# now build app's shared lib
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")

include_directories(../../../gameSrc)
include_directories(../../../../myEngine/src)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)


add_library(native-activity SHARED src/main/cpp/entry.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# target_include_directories(native-activity PRIVATE
#         ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(native-activity
        android
        native_app_glue
        GameSrc
        myEngine
        ${OPENGLES3_LIBRARY}
        ${EGL_LIBRARY}
        ${log-lib})

add_dependencies(native-activity GameSrc myEngine)