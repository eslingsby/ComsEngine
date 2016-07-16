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

	Binder::getSystem<Scripting>(L)->registerFile(luaL_checkstring(L, 1), luaL_checkstring(L, 2));

	return 0;
}
