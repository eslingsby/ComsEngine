#pragma once

#include "Binder.hpp"

#include "Script.hpp"

// Todo
// - Create valid Lua constructor and destructor

namespace ScriptBind{
	inline static int create(lua_State* L);

	inline static int destroy(lua_State* L);

	static const luaL_Reg meta[] = {
		{ "new", create },
		{ "__gc", destroy },
		{ 0, 0 }
	};
}

int ScriptBind::create(lua_State* L){
	// int

	// -
	//Binder::commonCreate(L);

	// {}
	//lua_newtable(L);

	// {} M{}
	//luaL_getmetatable(L, "Script");

	// {}
	//lua_setmetatable(L, -2);

	return 1;
}

int ScriptBind::destroy(lua_State* L){
	//Binder::commonDestroy(L);

	return 0;
}