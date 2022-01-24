#include <SDL.h>

#include "lua.hpp"

static int l_draw_rect(lua_State* L){
	int x=luaL_checknumber(L,1);
	int y=luaL_checknumber(L,2);
	int width=luaL_checknumber(L,3);
	int height=luaL_checknumber(L,4);

	lua_getfield(L,LUA_REGISTRYINDEX,"xavine_renderer");
	SDL_Renderer** renderer=(SDL_Renderer**)lua_touserdata(L,-1);
	lua_pop(L,1);
	if(!renderer){
		return luaL_error(L,"INVALID RENDERER");
	}

	SDL_Rect fillRect={x,y,width,height};
	SDL_RenderFillRect(*renderer,&fillRect);
	return 0;
}

static int l_set_color(lua_State* L){
	int r=luaL_checknumber(L,1)*0xFF;
	int g=luaL_checknumber(L,2)*0xFF;
	int b=luaL_checknumber(L,3)*0xFF;
	int a=luaL_checknumber(L,4)*0xFF;

	lua_getfield(L,LUA_REGISTRYINDEX,"xavine_renderer");
	SDL_Renderer** renderer=(SDL_Renderer**)lua_touserdata(L,-1);
	lua_pop(L,1);
	if(!renderer){
		return luaL_error(L,"INVALID RENDERER");
	}

	SDL_SetRenderDrawColor(*renderer,r,g,b,a);
	return 0;
}

void set_lua_primitive_functions(lua_State* L){
	//set drawfunctions in luatable
	lua_getglobal(L,"xavine");
	lua_pushcfunction(L,l_draw_rect);
	lua_setfield(L,1,"draw_rect");
	lua_getglobal(L,"xavine");
	lua_pushcfunction(L,l_set_color);
	lua_setfield(L,1,"set_color");
}
