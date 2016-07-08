#pragma once

#include "Binder.hpp"

#include "Engine.hpp"

namespace EngineBind{
	const char* name = "Engine";

	inline static int _dt(lua_State* L);
	inline static int _shutdown(lua_State* L);
	inline static int _references(lua_State* L);
	inline static int _define(lua_State* L);
	inline static int _register(lua_State* L);

	static const luaL_Reg global[] = {
		{ "dt", _dt },
		{ "shutdown", _shutdown },
		{ "references", _references },
		{ "define", _define },
		{ "register", _register },
		{ 0, 0 }
	};
}

inline int EngineBind::_dt(lua_State* L){
	lua_pushnumber(L, Binder::getEngine(L).deltaTime());
	return 1;
}

inline int EngineBind::_shutdown(lua_State* L){
	Binder::getEngine(L).shutdown();
	return 0;
}

inline int EngineBind::_references(lua_State* L){
	lua_pushinteger(L, Binder::getEngine(L).manager.totalReferences());
	return 1;
}

int EngineBind::_define(lua_State * L){
	// string

	// string {}
	lua_newtable(L);

	// string {} M{}
	luaL_newmetatable(L, luaL_checkstring(L, 1));

	// string {} M{} {}
	lua_pushvalue(L, -2);

	// string {} M{}
	lua_setfield(L, -2, "__index");

	// string {}
	lua_pop(L, 1);

	return 1;
}

int EngineBind::_register(lua_State* L){
	// function()

	std::string name = Binder::getEngine(L).getConfig("data") + luaL_checkstring(L, 2);

	if (luaL_loadfile(L, name.c_str())){
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return 0;
	}

	// {}
	if (lua_pcall(L, 0, 1, 0)){
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return 0;
	}

	// {} M{}
	luaL_newmetatable(L, luaL_checkstring(L, 1));

	// M{} {}
	lua_insert(L, -2);

	// M{}
	lua_setfield(L, -2, "__index");

	// -
	lua_pop(L, 1);

	return 0;
}
