#pragma once

#include "Binder.hpp"

#include "LuaTypes.hpp"
#include "VecBind.hpp"

namespace Vec3Bind{
	const char name[] = "Vec3";

	inline static int constructor(lua_State* L);

	inline static int _print(lua_State* L);

	inline int _add(lua_State* L);
	inline int _unm(lua_State* L);
	inline int _sub(lua_State* L);
	inline int _mul(lua_State* L);
	inline int _div(lua_State* L);
	inline int _eq(lua_State* L);

	inline int _length(lua_State* L, void* value);

	static const luaL_Reg meta[] = {
		{ "__add", _add },
		{ "__unm", _unm },
		{ "__sub", _sub },
		{ "__mul", _mul },
		{ "__div", _div },
		{ "__eq", _eq },
		{ "__tostring", _print },
		{ 0, 0 }
	};

	static const Binder::MemberReg getters[] = {
		{ "x", Binder::getNumber, offsetof(LuaVec3, x) },
		{ "y", Binder::getNumber, offsetof(LuaVec3, y) },
		{ "z", Binder::getNumber, offsetof(LuaVec3, z) },
		{ "length", _length, 0, true },
		{ 0, 0 }
	};

	static const Binder::MemberReg setters[] = {
		{ "x", Binder::setNumber, offsetof(LuaVec3, x) },
		{ "y", Binder::setNumber, offsetof(LuaVec3, y) },
		{ "z", Binder::setNumber, offsetof(LuaVec3, z) },
		{ 0, 0 }
	};
}

inline int Vec3Bind::constructor(lua_State* L){
	LuaVec3* vec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));

	Binder::printStack(L);

	if (lua_gettop(L) > 4)
		*vec = LuaVec3(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	else if (lua_gettop(L) > 3)
		*vec = LuaVec3(luaL_checknumber(L, 2), luaL_checknumber(L, 3), 0);
	else if (lua_gettop(L) > 2)
		*vec = LuaVec3(luaL_checknumber(L, 2), 0, 0);
	else
		*vec = LuaVec3();

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_print(lua_State* L){
	// L{}
	LuaVec3* vec = (LuaVec3*)luaL_checkudata(L, 1, name);
	std::string combined = std::to_string((*vec).x) + "," + std::to_string((*vec).y) + "," + std::to_string((*vec).z);

	// L{} string
	lua_pushstring(L, combined.c_str());

	return 1;
}

inline int Vec3Bind::_add(lua_State* L){
	return VecBind::_add<LuaVec3>(L, name);
}

inline int Vec3Bind::_unm(lua_State* L){
	return VecBind::_unm<LuaVec3>(L, name);
}

inline int Vec3Bind::_sub(lua_State* L){
	return VecBind::_sub<LuaVec3>(L, name);
}

inline int Vec3Bind::_mul(lua_State* L){
	return VecBind::_mulAdv<LuaVec3>(L, name);
}

inline int Vec3Bind::_div(lua_State* L){
	return VecBind::_div<LuaVec3>(L, name);
}

inline int Vec3Bind::_eq(lua_State* L){
	return VecBind::_eq<LuaVec3>(L, name);
}

inline int Vec3Bind::_length(lua_State* L, void* value){
	return VecBind::_length<LuaVec3>(L, name);
}
