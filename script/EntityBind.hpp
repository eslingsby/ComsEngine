#pragma once

#include "Binder.hpp"

#include "EntityRef.hpp"

namespace EntityBind{
	const char* name = "Entity";

	inline static void _create(lua_State* L);

	inline static int constructor(lua_State* L);

	inline static int _id(lua_State* L);
	inline static int _destroy(lua_State* L);
	inline static int _destroyed(lua_State* L);
	inline static int _invalidate(lua_State* L);
	inline static int _valid(lua_State* L);

	static const luaL_Reg meta[] = {
		{ "__gc", _invalidate },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "id", _id },
		{ "destroy", _destroy },
		{ "destroyed", _destroyed },
		{ "invalidate", _invalidate },
		{ "valid", _valid },
		{ 0, 0 }
	};
}

inline void EntityBind::_create(lua_State * L){
	// {} integer

	uint64_t id = luaL_checkinteger(L, 2);

	// {} integer {}
	void* location = lua_newuserdata(L, sizeof(EntityRef));
	new(location) EntityRef(Binder::getEngine(L).manager, id);
}

inline int EntityBind::constructor(lua_State * L){
	_create(L);

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);
	return 1;
}

inline int EntityBind::_id(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);
	lua_pushinteger(L, entity->id());
	return 1;
}

inline int EntityBind::_destroy(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);
	entity->destroy();
	return 0;
}

inline int EntityBind::_destroyed(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);

	if (entity->state() == EntityManager::Destroyed)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

inline int EntityBind::_invalidate(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);
	entity->invalidate();
	return 0;
}

inline int EntityBind::_valid(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);
	lua_pushboolean(L, (int)entity->valid());
	return 1;
}
