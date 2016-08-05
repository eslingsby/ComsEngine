#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Identifier.hpp"

namespace IdentifierBind{
	const char* name = "Identifier";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	inline static int _invalidate(lua_State* L);

	inline int _name(lua_State* L, void* value);
	inline int _layer(lua_State* L, void* value);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", _invalidate },
		{ 0, 0 }
	};

	static const Binder::MemberReg getters[] = {
		{ "name", _name, 0, true },
		{ "layer", _layer, 0, true },
		{ 0, 0 }
	};
}

inline int IdentifierBind::constructor(lua_State* L){
	Engine& engine = Binder::getEngine(L);
	uint64_t id = luaL_checkinteger(L, -1);
	
	if (!engine.manager.getComponent<Identifier>(id))
		luaL_error(L, engine.manager.getErrorString().c_str());

	Binder::createEntityRef(L, id, name);

	return 1;
}

inline int IdentifierBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	if (lua_gettop(L) > 2)
		engine.manager.addComponent<Identifier>(id, luaL_checkstring(L, 2), luaL_checkstring(L, 3));
	else
		engine.manager.addComponent<Identifier>(id, luaL_checkstring(L, 2));

	uint8_t error = engine.manager.getError();
	if (error)
		luaL_error(L, engine.manager.errorString(error).c_str());

	return 0;
}

int IdentifierBind::_invalidate(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();
	return 0;
}

inline int IdentifierBind::_name(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	lua_pushstring(L, entity->getComponent<Identifier>()->name.c_str());

	return 1;
}

inline int IdentifierBind::_layer(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	lua_pushstring(L, entity->getComponent<Identifier>()->layer.c_str());

	return 1;
}