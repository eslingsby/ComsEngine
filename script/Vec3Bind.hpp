#pragma once

#include "Binder.hpp"

#include "LuaTypes.hpp"

namespace Vec3Bind{
	const char* name = "Vec3";

	inline static int constructor(lua_State* L);

	inline static int _length(lua_State* L, void* value);

	inline static int _add(lua_State* L);
	inline static int _unm(lua_State* L);
	inline static int _sub(lua_State* L);
	inline static int _mul(lua_State* L);
	inline static int _div(lua_State* L);

	inline static int _eq(lua_State* L);

	inline static int _print(lua_State* L);

	static const luaL_Reg global[] = {
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ 0, 0 }
	};

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
	// number number number   OR   L{}   or   {}

	// ... L{}
	LuaVec3* vec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
	*vec = LuaVec3();

	if (lua_isuserdata(L, 2) && lua_gettop(L) > 2){
		*vec = *(LuaVec3*)lua_touserdata(L, 1);
	}
	else if (lua_istable(L, 2)){
		lua_getfield(L, 2, "x");
		lua_getfield(L, 2, "y");
		lua_getfield(L, 2, "z");

		if (lua_isnil(L, -3) && lua_isnil(L, -2) && lua_isnil(L, -1)){
			lua_pop(L, 3);

			if (lua_geti(L, 2, 1))
				(*vec).x = lua_tonumber(L, -3);
			if (lua_geti(L, 2, 2))
				(*vec).y = lua_tonumber(L, -2);
			if (lua_geti(L, 2, 3))
				(*vec).z = lua_tonumber(L, -1);
		}
		else{
			if (!lua_isnil(L, -3))
				(*vec).x = lua_tonumber(L, -3);
			if (!lua_isnil(L, -2))
				(*vec).y = lua_tonumber(L, -2);
			if (!lua_isnil(L, -1))
				(*vec).z = lua_tonumber(L, -1);
		}

		lua_pop(L, 3);
	}
	else if (lua_gettop(L) > 4){
		*vec = LuaVec3(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	}

	// number number number L{} L{}
	lua_pushvalue(L, -1);

	// number number number L{} L{} M{}
	luaL_getmetatable(L, name);

	// number number number L{}
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_length(lua_State* L, void* value){
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);

	lua_pushnumber(L, vec->length());
	return 1;
}

inline int Vec3Bind::_add(lua_State* L){
	// L{} number   OR   L{} L{}
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		LuaVec3* other = (LuaVec3*)lua_touserdata(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) + (*other);
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) + number;
	}
	
	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_unm(lua_State * L){
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);

	LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
	*newVec = LuaVec3(-*vec);

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_sub(lua_State * L){
	// L{} number   OR   L{} L{}
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		LuaVec3* other = (LuaVec3*)lua_touserdata(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) - (*other);
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) - number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_mul(lua_State * L){
	// L{} number   OR   L{} L{}
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));

		if (luaL_testudata(L, 2, name)){
			LuaVec3* other = (LuaVec3*)lua_touserdata(L, 2);
			(*newVec) = (*vec) * (*other);
		}
		else if (luaL_testudata(L, 2, "Quat")){
			LuaQuat* quat = (LuaQuat*)lua_touserdata(L, 2);
			(*newVec) = (*vec) * (*quat);
		}
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) * number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_div(lua_State * L){
	// L{} number   OR   L{} L{}
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		LuaVec3* other = (LuaVec3*)lua_touserdata(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) / (*other);
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		LuaVec3* newVec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
		(*newVec) = (*vec) / number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::_eq(lua_State * L){
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);
	LuaVec3* other = (LuaVec3*)lua_touserdata(L, 2);

	lua_pushboolean(L, (int)(*vec == *other));

	return 1;
}

inline int Vec3Bind::_print(lua_State* L){
	// L{}
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);
	std::string combined = std::to_string((*vec).x) + "," + std::to_string((*vec).y) + "," + std::to_string((*vec).z);

	// L{} string
	lua_pushstring(L, combined.c_str());

	return 1;
}