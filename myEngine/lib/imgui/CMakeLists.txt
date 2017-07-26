cmake_minimum_required(VERSION 2.8)

message("--->Cmake imgui--->")

project(imgui)
include_directories(.)

FILE(GLOB SRC_IMGUI "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(imgui STATIC ${SRC_IMGUI})
