/*
** Lua binding: 
** Generated automatically by tolua++-1.0.93-lua53 on Sat Jun 23 20:18:03 2018.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua__open (lua_State* tolua_S);

#include "toluaNeed.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Model");
 tolua_usertype(tolua_S,"GameScene");
 tolua_usertype(tolua_S,"Camera");
 tolua_usertype(tolua_S,"Shader");
 tolua_usertype(tolua_S,"Character");
}

/* method: getInstance of class  GameScene */
#ifndef TOLUA_DISABLE_tolua__GameScene_getInstance00
static int tolua__GameScene_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameScene",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameScene* tolua_ret = (GameScene*)  GameScene::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameScene");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroyInstance of class  GameScene */
#ifndef TOLUA_DISABLE_tolua__GameScene_destroyInstance00
static int tolua__GameScene_destroyInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScene",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScene* self = (GameScene*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyInstance'", NULL);
#endif
  {
   self->destroyInstance();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCamera of class  GameScene */
#ifndef TOLUA_DISABLE_tolua__GameScene_getCamera00
static int tolua__GameScene_getCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScene",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScene* self = (GameScene*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCamera'", NULL);
#endif
  {
   Camera* tolua_ret = (Camera*)  self->getCamera();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Camera");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadScene of class  GameScene */
#ifndef TOLUA_DISABLE_tolua__GameScene_loadScene00
static int tolua__GameScene_loadScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScene",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScene* self = (GameScene*)  tolua_tousertype(tolua_S,1,0);
  string scenename = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadScene'", NULL);
#endif
  {
   self->loadScene(scenename);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeScene of class  GameScene */
#ifndef TOLUA_DISABLE_tolua__GameScene_changeScene00
static int tolua__GameScene_changeScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScene",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScene* self = (GameScene*)  tolua_tousertype(tolua_S,1,0);
  string scenename = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'changeScene'", NULL);
#endif
  {
   self->changeScene(scenename);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'changeScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua__open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Camera","Camera","",NULL);
  tolua_beginmodule(tolua_S,"Camera");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Character","Character","",NULL);
  tolua_beginmodule(tolua_S,"Character");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Model","Model","",NULL);
  tolua_beginmodule(tolua_S,"Model");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Shader","Shader","",NULL);
  tolua_beginmodule(tolua_S,"Shader");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameScene","GameScene","",NULL);
  tolua_beginmodule(tolua_S,"GameScene");
   tolua_function(tolua_S,"getInstance",tolua__GameScene_getInstance00);
   tolua_function(tolua_S,"destroyInstance",tolua__GameScene_destroyInstance00);
   tolua_function(tolua_S,"getCamera",tolua__GameScene_getCamera00);
   tolua_function(tolua_S,"loadScene",tolua__GameScene_loadScene00);
   tolua_function(tolua_S,"changeScene",tolua__GameScene_changeScene00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ (lua_State* tolua_S) {
 return tolua__open(tolua_S);
};
#endif

