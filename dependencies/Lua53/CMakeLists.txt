cmake_minimum_required(VERSION 2.8)

message("--->Cmake Lua53--->")

# Set projectname
project(Lua53)

include_directories(./src)

file(GLOB HEADERS src/*.h)
set(SOURCES
	src/lapi.c
	src/lcode.c
	src/ldebug.c
	src/lgc.c
	src/lmathlib.c
	src/lopcodes.c
	src/lstring.c
	src/ltm.c
	src/lutf8lib.c
	src/lauxlib.c
	src/lcorolib.c
	src/ldo.c
	src/linit.c
	src/lmem.c
	src/loslib.c
	src/lstrlib.c
	src/lvm.c
	src/lbaselib.c
	src/lctype.c
	src/ldump.c
	src/liolib.c
	src/loadlib.c
	src/lparser.c
	src/ltable.c
	src/lzio.c
	src/lbitlib.c
	src/ldblib.c
	src/lfunc.c
	src/llex.c
	src/lobject.c
	src/lstate.c
	src/ltablib.c
	src/lundump.c
)
add_library(Lua53 ${HEADERS} ${SOURCES})
#set_target_properties(Lua53 PROPERTIES OUTPUT_NAME lua)