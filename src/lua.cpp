#include <stdio.h>
#include "lua.hpp"

void printvalue(lua_State* luastate,int idx){
	if(lua_isnil(luastate,idx))
		printf("nil");
	else if(lua_isboolean(luastate,idx))
		printf("%s",lua_toboolean(luastate,idx)?"true":"false");
	/*
	else if(lua_isinteger(luastate,idx))
		printf("%d",(int)lua_tointeger(luastate,idx));
	*/
	else if(lua_isnumber(luastate,idx))
		printf("%f",lua_tonumber(luastate,idx));
	else if(lua_isstring(luastate,idx))
		printf("%s",lua_tostring(luastate,idx));
}

void printtable(lua_State* luastate,int depth=0){
	//den befehl hab ich nicht ganz geblickt, aber damit geht man auf dem stack in ne tabelle rein
	lua_pushnil(luastate);
	//solange es weitere werte in der tabelle gibt, wird sie durchlaufen
	while(lua_next(luastate,-2)!=0){
		//einrückung damits schön aussieht^^
		for(int i=0;i<depth;i++)printf("\t");
		//wenn tabelle in aktueller tabelle in diese absteigen
		if(lua_istable(luastate, -1)){
			//formatierung für schöne ausgabe
			//key ausgeben
			printvalue(luastate,-2);
			printf(":{\n");
			//in verschachtelte tabelle absteigen (und für die einrückung eins hochzählen)
			printtable(luastate,depth+1);
			for(int i=0;i<depth;i++)printf("\t");
			printf("},\n");
		//sonst key-value paare ausgeben
		}else{
			//key ausgeben
			printvalue(luastate,-2);
			printf("=");
			//value ausgeben
			printvalue(luastate,-1);
			printf(",\n");
		}
		//wenn wert bearbeitet von stack nehmen
		lua_pop(luastate,1);
	}
}

lua_State* load_lua(){
	//lua laden
	lua_State* luastate=luaL_newstate();
	//standartlibraries von lua laden falls nötig
	//z.B.: os.etc math.etc string.etc etc...
	//wenn man sie nicht braucht kann man das auch weglassen
	luaL_openlibs(luastate);
	//luafile laden
	int result=luaL_loadfile(luastate,"main.lua");
	if(result==LUA_OK){
		//falls ok geladen luafile ausführen
		lua_pcall(luastate,0,LUA_MULTRET,0);
	}else{
		//fehlermeldung, danach sollte man ne fehlerbehandlung machen
		fprintf(stderr,"lua_error: %s\n",lua_tostring(luastate,-1));
	}

	return luastate;
}

void close_lua(lua_State* luastate){
	lua_close(luastate);
}

void run_lua(lua_State* luastate){
	lua_getglobal(luastate,"main");
	//only call the function if nil is not returned
	if(lua_isnil(luastate,-1)==0){
		lua_call(luastate,0,0);
	}
}

void lua_key_pressed(lua_State* luastate,const char* key,int action){
	lua_getglobal(luastate,"key_pressed");
	//only call the function if nil is not returned
	if(lua_isnil(luastate,-1)==0){
		lua_pushstring(luastate,key);
		lua_pushnumber(luastate,action);
		lua_call(luastate,2,0);
	}
}
