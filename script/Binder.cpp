#include "Binder.hpp"

#include "ScriptBind.hpp"

void Binder::bind(lua_State* L, const std::string& type, const luaL_Reg* methods, const luaL_Reg* meta){
	// M{}
	luaL_newmetatable(L, type.c_str());

	// M{} {}
	lua_newtable(L);

	// M{} {} {}
	lua_pushvalue(L, -1);

	// M{} {}
	lua_setfield(L, -3, "__index");

	// M{} {} {}
	lua_pushvalue(L, -1);

	// M{} {}
	lua_setglobal(L, type.c_str());

	// M{}
	luaL_setfuncs(L, methods, 0);

	// -
	if (meta)
		luaL_setfuncs(L, meta, 0);
	else
		lua_pop(L, 1);
}

void Binder::bind(lua_State* L){
	bind(L, "Script", ScriptBind::methods);
}