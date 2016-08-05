#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Script.hpp"
#include "Scripting.hpp"

namespace ScriptBind{
	const char* name = "Script";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	inline static int _invalidate(lua_State* L);

	inline int _create(lua_State* L);
	inline int _remove(lua_State* L);
	inline int _get(lua_State* L);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", _invalidate },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "create", _create },
		{ "remove", _remove },
		{ "get", _get },
		{ 0, 0 }
	};
}

inline int ScriptBind::constructor(lua_State* L){
	Binder::createEntityRef(L, luaL_checkinteger(L, -1), name);

	return 1;
}

inline int ScriptBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	engine.manager.addComponent<Script>(id);

	return 0;
}

int ScriptBind::_invalidate(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();
	return 0;
}

inline int ScriptBind::_create(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	assert(entity->valid());

	if (lua_gettop(L) >= 3)
		Binder::getSystem<Scripting>(L)->createInstance(entity->id(), luaL_checkstring(L, 2), (unsigned int)luaL_checkinteger(L, 3));
	else
		Binder::getSystem<Scripting>(L)->createInstance(entity->id(), luaL_checkstring(L, 2));

	return 0;
}

inline int ScriptBind::_remove(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	assert(entity->valid());

	if (lua_gettop(L) >= 3)
		Binder::getSystem<Scripting>(L)->destroyInstance(entity->id(), luaL_checkstring(L, 2), (unsigned int)luaL_checkinteger(L, 3));
	else
		Binder::getSystem<Scripting>(L)->destroyInstance(entity->id(), luaL_checkstring(L, 2));

	return 0;
}

inline int ScriptBind::_get(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	
	assert(entity->valid());

	int reference = -1;

	if (lua_gettop(L) >= 3)
		reference = Binder::getSystem<Scripting>(L)->getInstance(entity->id(), luaL_checkstring(L, 2), (unsigned int)luaL_checkinteger(L, 3));
	else
		reference = Binder::getSystem<Scripting>(L)->getInstance(entity->id(), luaL_checkstring(L, 2));

	assert(reference >= 0);

	lua_rawgeti(L, LUA_REGISTRYINDEX, reference);

	return 1;
}