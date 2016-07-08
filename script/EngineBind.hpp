#pragma once

#include "Binder.hpp"

#include "Engine.hpp"

namespace EngineBind{
	const char* name = "Engine";

	inline static int _dt(lua_State* L);
	inline static int _shutdown(lua_State* L);

	static const luaL_Reg global[] = {
		{ "dt", _dt },
		{ "shutdown", _shutdown },
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