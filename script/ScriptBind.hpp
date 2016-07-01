#pragma once

#include "Binder.hpp"

#include "Script.hpp"

namespace ScriptBind{
	inline static int create(lua_State* L);
	inline static int destroy(lua_State* L);
	
	static const luaL_Reg methods[] = {
		{ "create", create },
		{ "destroy", destroy },
		{ 0, 0 }
	};
}

int ScriptBind::create(lua_State* L){
	// int string int

	// int
	uint64_t id = lua_tointeger(L, -3);

	std::string type = lua_tostring(L, -2);
	unsigned int number = (unsigned int)lua_tointeger(L, -1);
	lua_pop(L, 2);

	// -
	Binder::commonCreate(L);

	// {}
	Script* script = StaticEngine::get().manager.getComponent<Script>(id);
	assert(script);

	assert(script->references.find(type) != script->references.end());

	assert(script->references[type].size() > number);
	
	auto& reference = script->references[type][number];

	assert(reference.first);
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, reference.second);

	return 1;
}

int ScriptBind::destroy(lua_State* L){
	// {}

	// -
	Binder::commonDestroy(L);

	return 0;
}