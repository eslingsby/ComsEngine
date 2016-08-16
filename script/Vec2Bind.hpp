#pragma once

#include "Binder.hpp"

#include "LuaTypes.hpp"
#include "VecBind.hpp"

namespace Vec2Bind{
	const char name[] = "Vec2";

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
		{ "x", Binder::getNumber, offsetof(LuaVec2, x) },
		{ "y", Binder::getNumber, offsetof(LuaVec2, y) },
		{ "length", _length, 0, true },
		{ 0, 0 }
	};

	static const Binder::MemberReg setters[] = {
		{ "x", Binder::setNumber, offsetof(LuaVec2, x) },
		{ "y", Binder::setNumber, offsetof(LuaVec2, y) },
		{ 0, 0 }
	};
}

inline int Vec2Bind::constructor(lua_State* L){
	LuaVec2* vec = (LuaVec2*)lua_newuserdata(L, sizeof(LuaVec2));

	if (lua_gettop(L) > 3)
		*vec = LuaVec2(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	else if (lua_gettop(L) > 2)
		*vec = LuaVec2(luaL_checknumber(L, 2), 0);
	else
		*vec = LuaVec2();

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec2Bind::_print(lua_State* L){
	// L{}
	LuaVec2* vec = (LuaVec2*)luaL_checkudata(L, 1, name);
	std::string combined = std::to_string((*vec).x) + "," + std::to_string((*vec).y);

	// L{} string
	lua_pushstring(L, combined.c_str());

	return 1;
}

inline int Vec2Bind::_add(lua_State* L){
	return VecBind::_add<LuaVec2>(L, name);
}

inline int Vec2Bind::_unm(lua_State* L){
	return VecBind::_unm<LuaVec2>(L, name);
}

inline int Vec2Bind::_sub(lua_State* L){
	return VecBind::_sub<LuaVec2>(L, name);
}

inline int Vec2Bind::_mul(lua_State* L){
	return VecBind::_mul<LuaVec2>(L, name);
}

inline int Vec2Bind::_div(lua_State* L){
	return VecBind::_div<LuaVec2>(L, name);
}

inline int Vec2Bind::_eq(lua_State* L){
	return VecBind::_eq<LuaVec2>(L, name);
}

inline int Vec2Bind::_length(lua_State* L, void* value){
	return VecBind::_length<LuaVec2>(L, name);
}
