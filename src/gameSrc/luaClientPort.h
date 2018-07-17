#ifndef __LUA_CLIENT_PORT_H__
#define __LUA_CLIENT_PORT_H__

#ifdef WIN32
#pragma warning (disable:4800)
#endif

#include "tolua++.h"

TOLUA_API int  tolua__open(lua_State* tolua_S);

#endif //__LUA_CLIENT_PORT_H__