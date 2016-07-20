#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Identifier.hpp"

namespace IdentifierBind{
	const char* name = "Identifier";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	inline int _name(lua_State* L, void* value);
	inline int _layer(lua_State* L, void* value);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ 0, 0 }
	};

	static const Binder::MemberReg getters[] = {
		{ "name", _name, 0, true },
		{ "layer", _layer, 0, true },
		{ 0, 0 }
	};
}

inline int IdentifierBind::constructor(lua_State* L){
	// {} integer

	Binder::createEntityRef(L, luaL_checkinteger(L, -1), name);

	return 1;
}

inline int IdentifierBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	if (lua_gettop(L) > 2)
		engine.manager.addComponent<Identifier>(id, luaL_checkstring(L, 2), luaL_checkstring(L, 3));
	else
		engine.manager.addComponent<Identifier>(id, luaL_checkstring(L, 2));

	return 0;
}

inline int IdentifierBind::_name(lua_State * L, void * value){
	if (Binder::requireSelfData(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);
	lua_pushstring(L, entity->getComponent<Identifier>()->name.c_str());

	return 1;
}

inline int IdentifierBind::_layer(lua_State * L, void * value){
	if (Binder::requireSelfData(L, "Entity"))
		return 0;

	EntityRef* entity = (EntityRef*)lua_touserdata(L, 1);
	lua_pushstring(L, entity->getComponent<Identifier>()->layer.c_str());

	return 1;
}