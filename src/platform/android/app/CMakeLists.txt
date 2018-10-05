# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

message("--->Cmake Main--->")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_subdirectory(../../../../dependencies androidDepend.out)
add_subdirectory(../../../gameSrc androidGameSrc.out)
add_subdirectory(../../../../myEngine androidmyEngine.out)

include_directories(../../../gameSrc)
include_directories(../../../../myEngine/src)

include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
add_library(app-glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c )


add_library( Main SHARED
             src/main/cpp/entry.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( log-lib log )
find_library( OPENGLES3_LIBRARY GLESv3 "OpenGL ES v3.0 library")
find_library( EGL_LIBRARY EGL "EGL 1.4 library" )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(Main
        GameSrc
        myEngine
        app-glue
        ${OPENGLES3_LIBRARY}
        ${EGL_LIBRARY}
        ${log-lib}
        android.so)

add_dependencies(Main GameSrc myEngine app-glue)