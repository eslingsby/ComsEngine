#pragma once

#include "StaticEngine.hpp"
#include "Scripting.hpp"

#include <string>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

// http://lua-users.org/wiki/BindingWithMembersAndMethods

namespace Binder{
	typedef int(*member_func)(lua_State* L, void* value);

	typedef const struct{
		const char* name;
		member_func func;
		size_t offset;
	} MemberReg;

	void _setMembers(lua_State* L, const MemberReg* binder);

	inline static int _indexHandler(lua_State* L);
	inline static int _newindexHandler(lua_State* L);

	void bind(lua_State* L, const std::string& type, const luaL_Reg* methods, const luaL_Reg* meta = 0, const MemberReg* getters = 0, const MemberReg* setters = 0);
	void bind(lua_State* L);

	template <typename T>
	inline static T* getComponent(lua_State* L);

	inline static void commonCreate(lua_State* L);
	inline static void commonDestroy(lua_State* L);

	inline static int getInt(lua_State* L, void* value);
	inline static int setInt(lua_State* L, void* value);
	inline static int getNumber(lua_State* L, void* value);
	inline static int setNumber(lua_State* L, void* value);
	inline static int getString(lua_State* L, void* value);
	inline static int setString(lua_State* L, void* value);
	inline static int getBool(lua_State* L, void* value);
	inline static int setBool(lua_State* L, void* value);
}

int Binder::_indexHandler(lua_State* L){
	// U{} string

	// U{} string string
	lua_pushvalue(L, 2);

	// U{} string L{}
	lua_rawget(L, lua_upvalueindex(1));

	// U{} string
	MemberReg* binding = (MemberReg*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	// -
	return binding->func(L, (void*)((uint8_t*)lua_touserdata(L, 1) + binding->offset));
}

int Binder::_newindexHandler(lua_State* L){
	// U{} string value

	// U{} string value string
	lua_pushvalue(L, 2);

	// U{} string value L{}
	lua_rawget(L, lua_upvalueindex(1));

	if (!lua_islightuserdata(L, -1)){
		lua_pop(L, 1);
		lua_pushvalue(L, 2);
		lua_gettable(L, lua_upvalueindex(2));
	}

	// U{} string value
	MemberReg* binding = (MemberReg*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	// -
	return binding->func(L, (void*)((uint8_t*)lua_touserdata(L, 1) + binding->offset));
}

template <typename T>
inline T* Binder::getComponent(lua_State* L){
	uint64_t id = 0;// get id from table

	T* component = StaticEngine::get().manager.getComponent<T>(id);
	assert(component);

	return component;
}

inline void Binder::commonCreate(lua_State* L){
	// int

	// -
	uint64_t id = lua_tointeger(L, -1);// get id from stack args
	lua_pop(L, 1);

	StaticEngine::get().manager.addReference(id);
}

inline void Binder::commonDestroy(lua_State* L){
	// {}

	// {} int
	lua_getfield(L, -1, "id");

	// -
	uint64_t id = lua_tointeger(L, -1);// get id from table
	lua_pop(L, 2);

	StaticEngine::get().manager.removeReference(id);
}

int Binder::getInt(lua_State* L, void* value){
	lua_pushinteger(L, *(lua_Integer*)value);
	return 1;
}

int Binder::setInt(lua_State* L, void* value){
	*(lua_Integer*)value = luaL_checkinteger(L, 3);
	return 0;
}

int Binder::getNumber(lua_State* L, void* value){
	lua_pushnumber(L, *(lua_Number*)value);
	return 1;
}

int Binder::setNumber(lua_State* L, void* value){
	*(lua_Number*)value = luaL_checknumber(L, 3);
	return 0;
}

int Binder::getString(lua_State* L, void* value){
	lua_pushstring(L, (char*)value);
	return 1;
}

int Binder::setString(lua_State* L, void* value){
	*(char*)value = *luaL_checkstring(L, 3);
	return 0;
}

int Binder::getBool(lua_State* L, void* value){
	lua_pushboolean(L, *(int*)value);
	return 1;
}

int Binder::setBool(lua_State* L, void* value){
	*(lua_Integer*)value = luaL_checkinteger(L, 3);
	return 0;
}