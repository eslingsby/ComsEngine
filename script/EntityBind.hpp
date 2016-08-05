#pragma once

#include "Binder.hpp"

#include "EntityRef.hpp"

namespace EntityBind{
	const char* name = "Entity";

	inline static int constructor(lua_State* L);

	inline static int _create(lua_State* L);

	inline static int _add(lua_State* L);
	inline static int _get(lua_State* L);

	inline static int _id(lua_State* L);
	inline static int _destroy(lua_State* L);
	inline static int _destroyed(lua_State* L);
	inline static int _invalidate(lua_State* L);
	inline static int _valid(lua_State* L);

	static const luaL_Reg meta[] = {
		{ "__gc", _invalidate },
		{ 0, 0 }
	};

	static const luaL_Reg global[] = {
		{ "create", _create },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "add", _add },
		{ "get", _get },
		{ "id", _id },
		{ "destroy", _destroy },
		{ "destroyed", _destroyed },
		{ "invalidate", _invalidate },
		{ "valid", _valid },
		{ 0, 0 }
	};
}

inline int EntityBind::constructor(lua_State * L){
	// {} integer

	Binder::createEntityRef(L, luaL_checkinteger(L, -1), name);

	return 1;
}

inline int EntityBind::_create(lua_State* L){
	Engine& engine = Binder::getEngine(L);
	
	uint64_t id = engine.manager.createEntity();

	Binder::createEntityRef(L, id, name);

	return 1;
}

inline int EntityBind::_add(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	if (!lua_istable(L, 2))
		luaL_argerror(L, 2, "not engine component");

	lua_getfield(L, 2, Binder::typeName);
	
	if (lua_isnil(L, -1) || luaL_checknumber(L, -1) != Binder::EngineType::Component)
		luaL_argerror(L, 2, "not engine component");

	lua_pop(L, 1);

	// U{} G{} ... function()
	lua_getfield(L, 2, "add");

	// U{} G{} function() ... 
	lua_insert(L, 3);

	// U{} G{} function() ... int
	lua_pushinteger(L, entity->id());

	// U{} G{} function() int ... 
	lua_insert(L, 4);
	
	// U{} G{}
	if (lua_pcall(L, lua_gettop(L) - 3, 0, 0))
		Binder::printStackError(L);

	return 0;
}

inline int EntityBind::_get(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	if (!lua_istable(L, 2))
		luaL_argerror(L, 2, "not engine component");

	lua_getfield(L, 2, Binder::typeName);

	if (lua_isnil(L, -1) || luaL_checknumber(L, -1) != Binder::EngineType::Component)
		luaL_argerror(L, 2, "not engine component");

	lua_pop(L, 1);

	// U{} G{} function()
	luaL_getmetafield(L, 2, "__call");

	// U{} G{} function() integer
	lua_pushinteger(L, entity->id());

	// U{} G{} U{}
	if (lua_pcall(L, 1, 1, 0)){
		Binder::printStackError(L);
		return 0;
	}

	return 1;
}

inline int EntityBind::_id(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	lua_pushinteger(L, entity->id());
	return 1;
}

inline int EntityBind::_destroy(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->destroy();
	return 0;
}

inline int EntityBind::_destroyed(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	if (entity->state() == EntityManager::Destroyed)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

inline int EntityBind::_invalidate(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();
	return 0;
}

inline int EntityBind::_valid(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	lua_pushboolean(L, (int)entity->valid());
	return 1;
}
