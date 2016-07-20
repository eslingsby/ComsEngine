#pragma once

#include "Engine.hpp"
#include "EntityRef.hpp"

#include <string>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

namespace Binder{
	typedef int(*member_func)(lua_State* L, void* value);

	typedef const struct{
		const char* name;
		member_func func;
		size_t offset;
		bool nil;
	} MemberReg;

	inline static int _indexHandler(lua_State* L);
	inline static int _newindexHandler(lua_State* L);
	
	void _setMembers(lua_State* L, const MemberReg* binder);

	void bind(lua_State* L, const std::string& type, lua_CFunction constructor = 0, const luaL_Reg* global = 0, const luaL_Reg* instance = 0, const luaL_Reg* meta = 0, const MemberReg* getters = 0, const MemberReg* setters = 0);
	void bind(lua_State* L, Engine& engine);

	inline Engine& getEngine(lua_State* L);

	template <typename T>
	inline static T* getSystem(lua_State* L);

	inline static void createEntityRef(lua_State* L, uint64_t id, const std::string& type);

	inline static void error(lua_State* L, const std::string& name, const std::string& error);

	inline static bool requireSelfData(lua_State* L, const std::string& name);

	inline static void printStack(lua_State* L);

	inline static int getInt(lua_State* L, void* value);
	inline static int setInt(lua_State* L, void* value);
	inline static int getNumber(lua_State* L, void* value);
	inline static int setNumber(lua_State* L, void* value);
	inline static int getString(lua_State* L, void* value);
	inline static int setString(lua_State* L, void* value);
	inline static int getBool(lua_State* L, void* value);
	inline static int setBool(lua_State* L, void* value);
}

inline int Binder::_indexHandler(lua_State* L){
	lua_pushvalue(L, 2);

	lua_rawget(L, lua_upvalueindex(1)); 

	if (!lua_islightuserdata(L, -1)){
		lua_pop(L, 1);  

		lua_pushvalue(L, 2);

		lua_gettable(L, lua_upvalueindex(2));
		
		return 1;
	}

	// U{} string
	MemberReg* binding = (MemberReg*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	// -
	if (binding->nil)
		return binding->func(L, nullptr);
	
	return binding->func(L, (void*)((uint8_t*)lua_touserdata(L, 1) + binding->offset));
}

inline int Binder::_newindexHandler(lua_State* L){
	lua_pushvalue(L, 2);

	lua_rawget(L, lua_upvalueindex(1));

	// U{} string value
	MemberReg* binding = (MemberReg*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	// -
	return binding->func(L, (void*)((uint8_t*)lua_touserdata(L, 1) + binding->offset));
}

inline Engine& Binder::getEngine(lua_State * L){
	lua_getglobal(L, "__engine");
	Engine* engine = (Engine*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	assert(engine);

	return *engine;
}

template<typename T>
inline T* Binder::getSystem(lua_State* L){
	T* system = getEngine(L).getSystem<T>();

	assert(system);

	return system;
}

inline void Binder::createEntityRef(lua_State* L, uint64_t id, const std::string& type){
	void* location = lua_newuserdata(L, sizeof(EntityRef));
	new(location) EntityRef(Binder::getEngine(L).manager, id);

	luaL_getmetatable(L, type.c_str());
	lua_setmetatable(L, -2);
}

inline void Binder::error(lua_State* L, const std::string& name, const std::string& error){
	lua_pushstring(L, (name + " Error! " + error).c_str());
	lua_error(L);
}

inline bool Binder::requireSelfData(lua_State* L, const std::string& name){
	if (lua_isuserdata(L, 1))
		return false;

	error(L, name, "Method requires self as argument, use ':' operator instead of '.'");
	return true;
}

inline void Binder::printStack(lua_State* L){
	for (int i = 1; i <= lua_gettop(L); i++){
		int t = lua_type(L, i);

		if (t == LUA_TSTRING)
			printf("`%s'", lua_tostring(L, i));
		else if (t == LUA_TBOOLEAN)
			printf(lua_toboolean(L, i) ? "true" : "false");
		else if (t == LUA_TNUMBER)
			printf("%g", lua_tonumber(L, i));
		else
			printf("%s", lua_typename(L, t));

		printf(", ");
	}
	printf("\n");
}

inline int Binder::getInt(lua_State* L, void* value){
	lua_pushinteger(L, *(lua_Integer*)value);
	return 1;
}

inline int Binder::setInt(lua_State* L, void* value){
	*(lua_Integer*)value = luaL_checkinteger(L, 3);
	return 0;
}

inline int Binder::getNumber(lua_State* L, void* value){
	lua_pushnumber(L, *(lua_Number*)value);
	return 1;
}

inline int Binder::setNumber(lua_State* L, void* value){
	*(lua_Number*)value = luaL_checknumber(L, 3);
	return 0;
}

inline int Binder::getString(lua_State* L, void* value){
	lua_pushstring(L, (char*)value);
	return 1;
}

inline int Binder::setString(lua_State* L, void* value){
	*(char*)value = *luaL_checkstring(L, 3);
	return 0;
}

inline int Binder::getBool(lua_State* L, void* value){
	lua_pushboolean(L, *(int*)value);
	return 1;
}

inline int Binder::setBool(lua_State* L, void* value){
	*(lua_Integer*)value = luaL_checkinteger(L, 3);
	return 0;
}