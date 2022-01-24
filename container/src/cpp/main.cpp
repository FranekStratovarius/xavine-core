#include <SDL.h>

#include "lua.hpp"
#include "lua/luainit.h"
#include "graphics/lua_primitives.h"

const int SCREEN_HEIGHT=500;
const int SCREEN_WIDTH=500;

int main(int argc,char* args[]){

	/******************/
	/* LUA EINSTELLEN */
	/******************/

	//lua laden
	lua_State* L=luaL_newstate();
	l_init(L);
	set_lua_primitive_functions(L);

	int result=lua_getglobal(L,"xavine");
	if(result!=0){
		result=lua_getfield(L,1,"load");
		if(result!=0){
			lua_call(L,0,0);
		}
	}
	
	/**************/
	/* LUA FERTIG */
	/**************/

	SDL_Window* window=nullptr;

	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		SDL_Log("Could not initialize SDL. Error: %s",SDL_GetError());
	}else{
		SDL_Log("video initialized");
	}

	window=SDL_CreateWindow(
		"SDL_Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if(!window){
		SDL_Log("Could not create window. Error: %s",SDL_GetError());
	}else{
		SDL_Log("window created");
	}

	SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	if(!window){
		SDL_Log("Could not create renderer. Error: %s",SDL_GetError());
	}else{
		SDL_Log("renderer created");
	}
	SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);

	//pass renderer to luastate for easy access (later: metamethods?)
	SDL_Renderer** lua_renderer=(SDL_Renderer**)lua_newuserdatauv(L,sizeof(SDL_Renderer*),1);
	*lua_renderer=renderer;
	lua_setfield(L,LUA_REGISTRYINDEX,"xavine_renderer");

	Uint64 NOW=SDL_GetPerformanceCounter();
	Uint64 LAST=0;
	double deltaTime=0;
	double elapsed=0;
	bool running=true;
	SDL_Event sdl_event;
	while(running){
		while(SDL_PollEvent(&sdl_event)!=0){
			if(sdl_event.type==SDL_QUIT){
				running=false;
			}else if(sdl_event.type==SDL_KEYDOWN){
				int result=lua_getglobal(L,"xavine");
				if(result!=0){
					result=lua_getfield(L,1,"keypressed");
					if(result!=0){
						lua_pushstring(L,SDL_GetScancodeName(sdl_event.key.keysym.scancode));
						lua_call(L,1,0);
					}
				}
			}
		}
		LAST=NOW;
		NOW=SDL_GetPerformanceCounter();
		deltaTime=(double)((NOW-LAST)*1000/(double)SDL_GetPerformanceFrequency());
		//SDL_Log("delta time: %f\nelapsed: %f",deltaTime,elapsed);
		elapsed+=deltaTime;
		//check if results are nil
		int result=lua_getglobal(L,"xavine");
		if(result!=0){
			result=lua_getfield(L,1,"update");
			if(result!=0){
				lua_pushnumber(L,deltaTime);
				lua_call(L,1,0);
			}
		}

		//reset renderer color
		SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
		SDL_RenderClear(renderer);

		result=lua_getglobal(L,"xavine");
		if(result!=0){
			result=lua_getfield(L,1,"draw");
			if(result!=0){
				lua_call(L,0,0);
			}
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	window=nullptr;
	renderer=nullptr;

	SDL_Quit();

	return 0;
}
