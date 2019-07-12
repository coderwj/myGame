cmake_minimum_required(VERSION 2.8)

message("--->Cmake tinyxml2--->")

project(tinyxml2)

include_directories(.)

add_library(tinyxml2 STATIC tinyxml2.cpp)

target_include_directories(tinyxml2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
