#include <SDL.h>

#include "lua.hpp"
#include "lua/luainit.h"

/*
static int l_testadd(lua_State* L){
	double a=lua_tonumber(L,1);
	double b=lua_tonumber(L,2);
	lua_pushnumber(L,a+b);
	return 1;
}
*/

void l_init(lua_State* L){
	SDL_Log("lua loading...");
	//standartlibraries von lua laden falls nötig
	luaL_openlibs(L);
	//lua_pushcfunction(L,l_testadd);
	//lua_setglobal(L,"testadd");
	int result=luaL_loadfile(L,"assets/test.lua");
	if(result==LUA_OK){
		//falls ok geladen luafile ausführen
		SDL_Log("lua loaded");
		lua_newtable(L);
		lua_setglobal(L,"xavine");
		lua_pcall(L,0,LUA_MULTRET,0);
	}else{
		//fehlermeldung, danach sollte man ne fehlerbehandlung machen
		SDL_Log("Konnte Luascript nicht laden: %i",result);
	}
}
