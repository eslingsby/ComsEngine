#pragma once

#include "Binder.hpp"

#include "Entity.hpp"

namespace EntityBind{
	const char* name = "Entity";

	inline static int constructor(lua_State* L);

	inline static int id(lua_State* L);
	inline static int destroy(lua_State* L);
	inline static int destroyed(lua_State* L);
	inline static int invalidate(lua_State* L);
	inline static int valid(lua_State* L);

	static const luaL_Reg meta[] = {
		{ "__gc", invalidate },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "id", id },
		{ "destroy", destroy },
		{ "destroyed", destroyed },
		{ "invalidate", invalidate },
		{ "valid", valid },
		{ 0, 0 }
	};
}

inline int EntityBind::constructor(lua_State * L){
	uint64_t id = luaL_checkinteger(L, 2);

	void* location = lua_newuserdata(L, sizeof(Entity));
	new(location) Entity(Binder::getEngine(L).manager, id);

	luaL_getmetatable(L, name);
	
	lua_setmetatable(L, 3);

	return 1;
}

inline int EntityBind::id(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	Entity* entity = (Entity*)lua_touserdata(L, 1);
	lua_pushinteger(L, entity->id());
	return 1;
}

inline int EntityBind::destroy(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	Entity* entity = (Entity*)lua_touserdata(L, 1);
	entity->destroy();
	return 0;
}

inline int EntityBind::destroyed(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	Entity* entity = (Entity*)lua_touserdata(L, 1);

	if (entity->state() == EntityManager::Destroyed)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

inline int EntityBind::invalidate(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	Entity* entity = (Entity*)lua_touserdata(L, 1);
	entity->invalidate();
	return 0;
}

inline int EntityBind::valid(lua_State * L){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	Entity* entity = (Entity*)lua_touserdata(L, 1);
	lua_pushboolean(L, (int)entity->valid());
	return 1;
}
