#pragma once

#include "Binder.hpp"

#include "LuaTypes.hpp"

namespace QuatBind{
	const char* name = "Quat";

	inline static int constructor(lua_State* L);

	inline static int _eulerAngles(lua_State* L);
	
	inline static int _add(lua_State* L);
	inline static int _unm(lua_State* L);
	inline static int _mul(lua_State* L);

	inline static int _eq(lua_State* L);

	inline static int _print(lua_State* L);
	
	static const luaL_Reg global[] = {
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "eulerAngles", _eulerAngles },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__add", _add },
		{ "__unm", _unm },
		{ "__mul", _mul },
		{ "__eq", _eq },
		{ "__tostring", _print },
		{ 0, 0 }
	};

	static const Binder::MemberReg getters[] = {
		{ "x", Binder::getNumber, offsetof(LuaQuat, x) },
		{ "y", Binder::getNumber, offsetof(LuaQuat, y) },
		{ "z", Binder::getNumber, offsetof(LuaQuat, z) },
		{ "w", Binder::getNumber, offsetof(LuaQuat, w) },
		{ 0, 0 }
	};

	static const Binder::MemberReg setters[] = {
		{ "x", Binder::setNumber, offsetof(LuaQuat, x) },
		{ "y", Binder::setNumber, offsetof(LuaQuat, y) },
		{ "z", Binder::setNumber, offsetof(LuaQuat, z) },
		{ "w", Binder::getNumber, offsetof(LuaQuat, w) },
		{ 0, 0 }
	};
}

inline int QuatBind::constructor(lua_State * L){
	LuaQuat* quat = (LuaQuat*)lua_newuserdata(L, sizeof(LuaQuat));
	*quat = LuaQuat();

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	if (luaL_testudata(L, 2, name))
		*quat = *(LuaQuat*)luaL_checkudata(L, 2, name);
	else if (luaL_testudata(L, 2, "Vec3"))
		*quat = LuaQuat(glm::radians(*(LuaVec3*)luaL_checkudata(L, 2, "Vec3")));

	return 1;
}

int QuatBind::_eulerAngles(lua_State * L){
	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 1, name);

	LuaVec3* vec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
	*vec = glm::degrees(glm::eulerAngles(*quat));

	luaL_getmetatable(L, "Vec3");
	lua_setmetatable(L, -2);

	return 1;
}

inline int QuatBind::_add(lua_State * L){
	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 1, name);
	LuaQuat* other = (LuaQuat*)luaL_checkudata(L, 2, name);

	LuaQuat* newQuat = (LuaQuat*)lua_newuserdata(L, sizeof(LuaQuat));
	*newQuat = *quat + *other;

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int QuatBind::_unm(lua_State * L){
	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 1, name);

	LuaQuat* newQuat = (LuaQuat*)lua_newuserdata(L, sizeof(LuaQuat));
	*newQuat = -*quat;

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int QuatBind::_mul(lua_State * L){
	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 1, name);
	LuaQuat* other = (LuaQuat*)luaL_checkudata(L, 2, name);

	LuaQuat* newQuat = (LuaQuat*)lua_newuserdata(L, sizeof(LuaQuat));
	*newQuat = *quat * *other;

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int QuatBind::_eq(lua_State * L){
	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 1, name);
	LuaQuat* other = (LuaQuat*)luaL_checkudata(L, 2, name);

	lua_pushboolean(L, *quat == *other);

	return 1;
}

int QuatBind::_print(lua_State * L){
	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 1, name);
	std::string combined = std::to_string((*quat).x) + "," + std::to_string((*quat).y) + "," + std::to_string((*quat).z) + "," + std::to_string((*quat).w);

	// L{} string
	lua_pushstring(L, combined.c_str());

	return 1;
}
