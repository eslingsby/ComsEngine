#pragma once

#include "Binder.hpp"

#include "LuaTypes.hpp"
#include "VecBind.hpp"

namespace Vec4Bind{
	const char name[] = "Vec4";

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
		{ "x", Binder::getNumber, offsetof(LuaVec4, x) },
		{ "y", Binder::getNumber, offsetof(LuaVec4, y) },
		{ "z", Binder::getNumber, offsetof(LuaVec4, z) },
		{ "w", Binder::getNumber, offsetof(LuaVec4, w) },
		{ "length", _length, 0, true },
		{ 0, 0 }
	};

	static const Binder::MemberReg setters[] = {
		{ "x", Binder::setNumber, offsetof(LuaVec4, x) },
		{ "y", Binder::setNumber, offsetof(LuaVec4, y) },
		{ "z", Binder::setNumber, offsetof(LuaVec4, z) },
		{ "w", Binder::setNumber, offsetof(LuaVec4, w) },
		{ 0, 0 }
	};
}

inline int Vec4Bind::constructor(lua_State* L){
	LuaVec4* vec = (LuaVec4*)lua_newuserdata(L, sizeof(LuaVec4));

	if (lua_gettop(L) > 5)
		*vec = LuaVec4(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	else if (lua_gettop(L) > 4)
		*vec = LuaVec4(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), 0);
	else if (lua_gettop(L) > 3)
		*vec = LuaVec4(luaL_checknumber(L, 2), luaL_checknumber(L, 3), 0, 0);
	else if (lua_gettop(L) > 2)
		*vec = LuaVec4(luaL_checknumber(L, 2), 0, 0, 0);
	else
		*vec = LuaVec4();

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec4Bind::_print(lua_State* L){
	// L{}
	LuaVec4* vec = (LuaVec4*)luaL_checkudata(L, 1, name);
	std::string combined = std::to_string((*vec).x) + "," + std::to_string((*vec).y) + "," + std::to_string((*vec).z) + "," + std::to_string((*vec).w);

	// L{} string
	lua_pushstring(L, combined.c_str());

	return 1;
}

inline int Vec4Bind::_add(lua_State* L){
	return VecBind::_add<LuaVec4>(L, name);
}

inline int Vec4Bind::_unm(lua_State* L){
	return VecBind::_unm<LuaVec4>(L, name);
}

inline int Vec4Bind::_sub(lua_State* L){
	return VecBind::_sub<LuaVec4>(L, name);
}

inline int Vec4Bind::_mul(lua_State* L){
	return VecBind::_mulAdv<LuaVec4>(L, name);
}

inline int Vec4Bind::_div(lua_State* L){
	return VecBind::_div<LuaVec4>(L, name);
}

inline int Vec4Bind::_eq(lua_State* L){
	return VecBind::_eq<LuaVec4>(L, name);
}

inline int Vec4Bind::_length(lua_State* L, void* value){
	return VecBind::_length<LuaVec4>(L, name);
}
