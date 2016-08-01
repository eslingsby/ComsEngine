#pragma once

#include "Binder.hpp"

namespace VecBind{
	template <typename T>
	inline static int _length(lua_State* L, const char* name);

	template <typename T>
	inline static int _add(lua_State* L, const char* name);

	template <typename T>
	inline static int _unm(lua_State* L, const char* name);

	template <typename T>
	inline static int _sub(lua_State* L, const char* name);

	template <typename T>
	inline static int _mul(lua_State* L, const char* name);

	template <typename T>
	inline static int _mulAdv(lua_State* L, const char* name);

	template <typename T>
	inline static int _div(lua_State* L, const char* name);

	template <typename T>
	inline static int _eq(lua_State* L, const char* name);
}

template <typename T>
inline int VecBind::_length(lua_State* L, const char* name){
	T* vec = (T*)lua_touserdata(L, 1);

	lua_pushnumber(L, vec->length());
	return 1;
}

template <typename T>
inline int VecBind::_add(lua_State* L, const char* name){
	// L{} number   OR   L{} L{}
	T* vec = (T*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		T* other = (T*)lua_touserdata(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) + (*other);
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) + number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

template <typename T>
inline int VecBind::_unm(lua_State * L, const char* name){
	T* vec = (T*)lua_touserdata(L, 1);

	T* newVec = (T*)lua_newuserdata(L, sizeof(T));
	*newVec = T(-*vec);

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

template <typename T>
inline int VecBind::_sub(lua_State * L, const char* name){
	// L{} number   OR   L{} L{}
	T* vec = (T*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		T* other = (T*)lua_touserdata(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) - (*other);
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) - number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

template <typename T>
inline int VecBind::_mul(lua_State * L, const char* name){
	// L{} number   OR   L{} L{}
	T* vec = (T*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));

		if (luaL_testudata(L, 2, name)){
			T* other = (T*)lua_touserdata(L, 2);
			(*newVec) = (*vec) * (*other);
		}
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) * number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

template <typename T>
inline int VecBind::_mulAdv(lua_State * L, const char* name){
	// L{} number   OR   L{} L{}
	T* vec = (T*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));

		if (luaL_testudata(L, 2, name)){
			T* other = (T*)lua_touserdata(L, 2);
			(*newVec) = (*vec) * (*other);
		}
		else if (luaL_testudata(L, 2, "Quat")){
			LuaQuat* quat = (LuaQuat*)lua_touserdata(L, 2);
			(*newVec) = (*vec) * (*quat);
		}
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) * number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

template <typename T>
inline int VecBind::_div(lua_State * L, const char* name){
	// L{} number   OR   L{} L{}
	T* vec = (T*)lua_touserdata(L, 1);

	if (lua_isuserdata(L, 2)){
		T* other = (T*)lua_touserdata(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) / (*other);
	}
	else{
		lua_Number number = luaL_checknumber(L, 2);
		T* newVec = (T*)lua_newuserdata(L, sizeof(T));
		(*newVec) = (*vec) / number;
	}

	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);

	return 1;
}

template <typename T>
inline int VecBind::_eq(lua_State * L, const char* name){
	T* vec = (T*)lua_touserdata(L, 1);
	T* other = (T*)lua_touserdata(L, 2);

	lua_pushboolean(L, (int)(*vec == *other));

	return 1;
}