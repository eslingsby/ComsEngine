#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Script.hpp"
#include "Scripting.hpp"

namespace ScriptBind{
	const char* name = "Script";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);
	inline int _has(lua_State* L);

	inline static int _gc(lua_State* L);

	inline static int _register(lua_State* L);

	inline int _create(lua_State* L);
	inline int _remove(lua_State* L);
	inline int _instance(lua_State* L);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ "has", _has },
		{ "register", _register },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", _gc },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "create", _create },
		{ "remove", _remove },
		{ "instance", _instance },
		{ 0, 0 }
	};
}

inline int ScriptBind::constructor(lua_State* L){
	Engine& engine = Binder::getEngine(L);
	uint64_t id = luaL_checkinteger(L, -1);

	engine.manager.getComponent<Script>(id);

	Binder::checkEngineError(L);

	Binder::createEntityRef(L, id, name);

	return 1;
}

inline int ScriptBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	engine.manager.addComponent<Script>(id);
	
	Binder::checkEngineError(L);

	return 0;
}

int ScriptBind::_has(lua_State * L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	bool has = engine.manager.hasComponents<Script>(id);

	Binder::checkEngineError(L);

	lua_pushboolean(L, has);

	return 1;
}

int ScriptBind::_gc(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();

	return 0;
}

int ScriptBind::_register(lua_State * L){
	Binder::getSystem<Scripting>(L)->registerFile(luaL_checkstring(L, 1), luaL_checkstring(L, 2));

	return 0;
}

inline int ScriptBind::_create(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	Binder::checkEntity(L, entity);

	bool okay = Binder::getSystem<Scripting>(L)->createInstance(entity->id(), luaL_checkstring(L, 2));

	if (!okay){
		Binder::checkEngineError(L);
		luaL_error(L, "reached max amount of script components OR script type is non-existent");
	}

	return 0;
}

inline int ScriptBind::_remove(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	Binder::checkEntity(L, entity);

	bool okay = true;

	if (lua_gettop(L) >= 3)
		okay = Binder::getSystem<Scripting>(L)->destroyInstance(entity->id(), luaL_checkstring(L, 2), (unsigned int)luaL_checkinteger(L, 3));
	else
		okay = Binder::getSystem<Scripting>(L)->destroyInstance(entity->id(), luaL_checkstring(L, 2));

	if (!okay){
		Binder::checkEngineError(L);
		luaL_error(L, "script type is non-existent OR script version is non-existent");
	}

	return 0;
}

inline int ScriptBind::_instance(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	
	Binder::checkEntity(L, entity);

	int reference = -1;

	if (lua_gettop(L) >= 3)
		reference = Binder::getSystem<Scripting>(L)->getInstance(entity->id(), luaL_checkstring(L, 2), (unsigned int)luaL_checkinteger(L, 3));
	else
		reference = Binder::getSystem<Scripting>(L)->getInstance(entity->id(), luaL_checkstring(L, 2));

	if (reference == -1){
		Binder::checkEngineError(L);
		luaL_error(L, "script type is non-existent OR script version is non-existent");
	}

	lua_rawgeti(L, LUA_REGISTRYINDEX, reference);

	return 1;
}