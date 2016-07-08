#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Identification.hpp"
#include "EntityRef.hpp"

namespace IdentificationBind{
	const char* name = "Identification";

	inline static int _getByName(lua_State* L);
	inline static int _getByLayer(lua_State* L);
	inline static int _hasName(lua_State* L);
	inline static int _hasLayer(lua_State* L);

	static const luaL_Reg global[] = {
		{ "getByName", _getByName },
		{ "getByLayer", _getByLayer },
		{ "hasName", _hasName },
		{ "hasLayer", _hasLayer },
		{ 0, 0 }
	};
}

int IdentificationBind::_getByName(lua_State* L){
	Identification* identification = Binder::getSystem<Identification>(L);

	uint64_t id = identification->getByName(luaL_checkstring(L, 1));

	void* location = lua_newuserdata(L, sizeof(EntityRef));
	new(location) EntityRef(Binder::getEngine(L).manager, id);

	luaL_getmetatable(L, "Entity");

	lua_setmetatable(L, -2);

	return 1;
}

int IdentificationBind::_getByLayer(lua_State * L){
	Identification* identification = Binder::getSystem<Identification>(L);

	uint64_t id = identification->getByLayer(luaL_checkstring(L, 1), luaL_checkinteger(L, 2));

	void* location = lua_newuserdata(L, sizeof(EntityRef));
	new(location) EntityRef(Binder::getEngine(L).manager, id);

	luaL_getmetatable(L, "Entity");

	lua_setmetatable(L, -2);

	return 1;
}

int IdentificationBind::_hasName(lua_State * L){
	Identification* identification = Binder::getSystem<Identification>(L);

	lua_pushboolean(L, (int)identification->getByName(luaL_checkstring(L, 1)));

	return 1;
}

int IdentificationBind::_hasLayer(lua_State * L){
	Identification* identification = Binder::getSystem<Identification>(L);

	lua_pushinteger(L, identification->hasLayer(luaL_checkstring(L, 1)));

	return 1;
}