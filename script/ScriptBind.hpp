#pragma once

#include "Binder.hpp"

#include "Script.hpp"

namespace ScriptBind{
	const char* name = "Script";

	inline static int create(lua_State* L);

	inline static int destroy(lua_State* L);

	inline static int id(lua_State* L);
	
	static const luaL_Reg methods[] = {
		{ "create", create },
		{ "destroy", destroy },
		{ "id", id },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", destroy },
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

	// {}
	Script* script = StaticEngine::get().manager.getComponent<Script>(id);

	if (!script){
		std::string error = "Entity doesn't have script component " + std::to_string(id) + "\n";
		Binder::error(L, "Script", error);
		return 0;
	}

	if (script->references.find(type) == script->references.end()){
		std::string error = "Entity " + std::to_string(id) + " doesn't have any script components of type " + type + "\n";
		Binder::error(L, "Script", error);
		return 0;
	}

	if (script->references[type].size() > number){
		auto& reference = script->references[type][number];

		if (reference.first){
			// -
			Binder::commonCreate(L);
			
			// L{}
			lua_rawgeti(L, LUA_REGISTRYINDEX, reference.second);

			return 1;
		}
	}

	std::string error = "Entity " + std::to_string(id) + " doesn't have a script component of type " + type + " at index " + std::to_string(number) + "\n";
	Binder::error(L, "Script", error);
	return 0;
}

int ScriptBind::destroy(lua_State* L){
	// {}

	// -
	Binder::commonDestroy(L);

	return 0;
}

int ScriptBind::id(lua_State * L){
	return 0;
}
