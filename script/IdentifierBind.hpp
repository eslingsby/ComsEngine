#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Identifier.hpp"
#include "Identification.hpp"

namespace IdentifierBind{
	const char* name = "Identifier";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	inline static int _gc(lua_State* L);

	inline int _name(lua_State* L, void* value);
	inline int _layer(lua_State* L, void* value);

	inline int _rename(lua_State* L);

	inline static int _getByName(lua_State* L);
	inline static int _getByLayer(lua_State* L);
	inline static int _hasName(lua_State* L);
	inline static int _hasLayer(lua_State* L);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ "getByName", _getByName },
		{ "getByLayer", _getByLayer },
		{ "hasName", _hasName },
		{ "hasLayer", _hasLayer },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", _gc },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "rename", _rename },
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
	
	engine.manager.getComponent<Identifier>(id);

	Binder::checkEngineError(L);

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

	Binder::checkEngineError(L);

	return 0;
}

int IdentifierBind::_gc(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();

	return 0;
}

inline int IdentifierBind::_name(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	Binder::checkEntity(L, entity);

	lua_pushstring(L, entity->getComponent<Identifier>()->name);

	return 1;
}

inline int IdentifierBind::_layer(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	Binder::checkEntity(L, entity);

	lua_pushstring(L, entity->getComponent<Identifier>()->layer);

	return 1;
}

int IdentifierBind::_rename(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	Binder::checkEntity(L, entity);

	Binder::getSystem<Identification>(L)->rename(entity->id(), luaL_checkstring(L, 2), luaL_checkstring(L, 3));

	return 0;
}

inline int IdentifierBind::_getByName(lua_State* L){
	Identification* identification = Binder::getSystem<Identification>(L);

	std::string name = luaL_checkstring(L, 1);

	if (!identification->hasName(name)){
		Binder::checkEngineError(L);
		luaL_error(L, "no entity is bound to name");
	}

	uint64_t id = identification->getByName(name.c_str());

	Binder::createEntityRef(L, id, "Entity");

	return 1;
}

inline int IdentifierBind::_getByLayer(lua_State * L){
	Identification* identification = Binder::getSystem<Identification>(L);

	std::string name = luaL_checkstring(L, 1);

	if (!identification->hasLayer(name)){
		Binder::checkEngineError(L);
		luaL_error(L, "no entity is bound to layer");
	}

	uint64_t id = identification->getByLayer(name.c_str(), luaL_checkinteger(L, 2));

	Binder::createEntityRef(L, id, "Entity");

	return 1;
}

inline int IdentifierBind::_hasName(lua_State * L){
	Identification* identification = Binder::getSystem<Identification>(L);

	lua_pushboolean(L, (int)identification->getByName(luaL_checkstring(L, 1)));

	return 1;
}

inline int IdentifierBind::_hasLayer(lua_State * L){
	Identification* identification = Binder::getSystem<Identification>(L);

	lua_pushinteger(L, identification->hasLayer(luaL_checkstring(L, 1)));

	return 1;
}