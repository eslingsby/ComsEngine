#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Mesh.hpp"

namespace MeshBind{
	const char* name = "Mesh";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ 0, 0 }
	};
}

inline int MeshBind::constructor(lua_State* L){
	// {} integer

	Binder::createEntityRef(L, luaL_checkinteger(L, -1), name);

	return 1;
}

inline int MeshBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	engine.manager.addComponent<Mesh>(id, luaL_checkstring(L, 2));

	return 0;
}