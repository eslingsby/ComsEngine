#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Mesh.hpp"

namespace MeshBind{
	const char* name = "Mesh";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	inline static int _invalidate(lua_State* L);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", _invalidate },
		{ 0, 0 }
	};
}

inline int MeshBind::constructor(lua_State* L){
	Engine& engine = Binder::getEngine(L);
	uint64_t id = luaL_checkinteger(L, -1);

	if (!engine.manager.getComponent<Mesh>(id))
		luaL_error(L, engine.manager.getErrorString().c_str());

	Binder::createEntityRef(L, id, name);

	return 1;
}

inline int MeshBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);

	Engine& engine = Binder::getEngine(L);

	engine.manager.addComponent<Mesh>(id, luaL_checkstring(L, 2));

	uint8_t error = engine.manager.getError();
	if (error)
		luaL_error(L, engine.manager.errorString(error).c_str());

	return 0;
}

int MeshBind::_invalidate(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();
	return 0;
}
