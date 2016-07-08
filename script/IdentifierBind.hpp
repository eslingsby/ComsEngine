#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "ComponentRef.hpp"
#include "Identifier.hpp"
#include "EntityBind.hpp"

namespace IdentifierBind{
	const char* name = "Identifier";

	inline int constructor(lua_State* L);

	inline int _name(lua_State* L, void* value);
	inline int _layer(lua_State* L, void* value);

	static const Binder::MemberReg getters[] = {
		{ "name", _name, 0, true },
		{ "layer", _layer, 0, true },
		{ 0, 0 }
	};
}

int IdentifierBind::constructor(lua_State* L){
	// {} integer

	uint64_t id = luaL_checkinteger(L, 2);

	// {} integer {}
	void* location = lua_newuserdata(L, sizeof(ComponentRef<Identifier>));
	new(location) ComponentRef<Identifier>(Binder::getEngine(L).manager, id);

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

int IdentifierBind::_name(lua_State * L, void * value){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	ComponentRef<Identifier>* entity = (ComponentRef<Identifier>*)lua_touserdata(L, 1);
	lua_pushstring(L, entity->component().name.c_str());

	return 1;
}

int IdentifierBind::_layer(lua_State * L, void * value){
	if (Binder::requireUserdata(L, "Entity"))
		return 0;

	ComponentRef<Identifier>* entity = (ComponentRef<Identifier>*)lua_touserdata(L, 1);
	lua_pushstring(L, entity->component().layer.c_str());

	return 1;
}