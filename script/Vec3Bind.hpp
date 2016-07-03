#pragma once

#include "Binder.hpp"

#include <glm\vec3.hpp>

namespace Vec3Bind{
	typedef glm::tvec3<lua_Number, glm::highp> LuaVec3;

	inline static int create(lua_State* L);

	inline static int add(lua_State* L);

	inline static int print(lua_State* L);

	static const luaL_Reg methods[] = {
		{ "create", create },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__add", add },
		{ "__tostring", print },
		{ 0, 0 }
	};

	static const Binder::MemberReg getters[] = {
		{ "x",    Binder::getNumber, offsetof(LuaVec3, x) },
		{ "y",    Binder::getNumber, offsetof(LuaVec3, y) },
		{ "z",    Binder::getNumber, offsetof(LuaVec3, z) },
		{ 0, 0 }
	};

	static const Binder::MemberReg setters[] = {
		{ "x",    Binder::setNumber, offsetof(LuaVec3, x) },
		{ "y",    Binder::setNumber, offsetof(LuaVec3, y) },
		{ "z",    Binder::setNumber, offsetof(LuaVec3, z) },
		{ 0, 0 }
	};
}

int Vec3Bind::create(lua_State* L){
	// number number number   OR   L{}   or   {}

	// ... L{}
	LuaVec3* vec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
	*vec = LuaVec3();

	if (lua_isuserdata(L, 1)){
		*vec = *(LuaVec3*)lua_touserdata(L, 1);
	}
	else if (lua_istable(L, 1)){
		lua_getfield(L, 1, "x");
		lua_getfield(L, 1, "y");
		lua_getfield(L, 1, "z");

		if (lua_isnil(L, -3) && lua_isnil(L, -2) && lua_isnil(L, -1)){
			lua_pop(L, 3);

			if (lua_geti(L, 1, 1))
				(*vec).x = lua_tonumber(L, -3);
			if (lua_geti(L, 1, 2))
				(*vec).y = lua_tonumber(L, -2);
			if (lua_geti(L, 1, 3))
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
	else{
		*vec = LuaVec3(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	}

	// number number number L{} L{}
	lua_pushvalue(L, -1);

	// number number number L{} L{} M{}
	luaL_getmetatable(L, "Vec3");

	// number number number L{}
	lua_setmetatable(L, -2);

	return 1;
}

int Vec3Bind::add(lua_State* L){
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
	
	// L{} number L{} L{}
	lua_pushvalue(L, -1);

	// L{} number L{} L{} M{}
	luaL_getmetatable(L, "Vec3");

	// L{} number L{}
	lua_setmetatable(L, -2);

	return 1;
}

inline int Vec3Bind::print(lua_State* L){
	// L{}
	LuaVec3* vec = (LuaVec3*)lua_touserdata(L, 1);
	std::string combined = std::to_string((*vec).x) + "," + std::to_string((*vec).y) + "," + std::to_string((*vec).z);

	// L{} string
	lua_pushstring(L, combined.c_str());

	return 1;
}