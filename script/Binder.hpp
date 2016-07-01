#pragma once

#include "StaticEngine.hpp"
#include "Scripting.hpp"

#include <string>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

namespace Binder{
	void bind(lua_State* L, const std::string& type, const luaL_Reg* methods, const luaL_Reg* meta = 0);
	void bind(lua_State* L);
	
	template <typename T>
	inline static T* getComponent(lua_State* L);

	inline static void commonCreate(lua_State* L);
	inline static void commonDestroy(lua_State* L);
}

template <typename T>
inline T* Binder::getComponent(lua_State* L){
	uint64_t id = 0;// get id from table

	T* component = StaticEngine::get().manager.getComponent<T>(id);

	assert(component);

	return component;
}

inline void Binder::commonCreate(lua_State* L){
	// int

	// -
	uint64_t id = lua_tointeger(L, -1);// get id from stack args
	lua_pop(L, 1);

	StaticEngine::get().manager.addReference(id);
}

inline void Binder::commonDestroy(lua_State* L){
	// {}

	// {} int
	lua_getfield(L, -1, "id");

	// -
	uint64_t id = lua_tointeger(L, -1);// get id from table
	lua_pop(L, 2);

	StaticEngine::get().manager.removeReference(id);
}