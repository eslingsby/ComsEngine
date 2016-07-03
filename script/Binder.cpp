#include "Binder.hpp"

#include "ScriptBind.hpp"
#include "Vec3Bind.hpp"

void Binder::_setMembers(lua_State* L, MemberReg* binder){
	// {}

	for (; binder->name; binder++){
		// {} string
		lua_pushstring(L, binder->name);

		// {} string L{}
		lua_pushlightuserdata(L, (void*)binder);

		// {}
		lua_settable(L, -3);
	}
}

void Binder::bind(lua_State* L, const std::string& type, const luaL_Reg* methods, const luaL_Reg* meta, const MemberReg* getters, const MemberReg* setters){
	// Register methods
	// {}
	lua_newtable(L);
	int methodsTable = lua_gettop(L);

	if (methods)
		luaL_setfuncs(L, methods, 0);
	
	// Register meta methods
	// {} M{}
	luaL_newmetatable(L, type.c_str());
	int metaTable = lua_gettop(L);

	if (meta)
		luaL_setfuncs(L, meta, 0);
	
	// Set metaTable.__metatable = methodsTable
	// {} M{} literal {}
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methodsTable);

	// {} M{}
	lua_rawset(L, metaTable);

	// Register member getters
	if (getters){
	// {} M{} literal
	lua_pushliteral(L, "__index");

	// {} M{} literal M{}
	lua_pushvalue(L, metaTable);

	//if (getters)
		_setMembers(L, getters);

	// {} M{} literal M{} {}
	lua_pushvalue(L, methodsTable);

	// {} M{} literal function()
	lua_pushcclosure(L, _indexHandler, 2);

	// {} M{}
	lua_rawset(L, metaTable);
	}
	
	// Register member setters
	if (setters){
	// {} M{} literal
	lua_pushliteral(L, "__newindex");

	// {} M{} literal {}
	lua_newtable(L);

	//if (setters)
		_setMembers(L, setters);

	// {} M{} literal function()
	lua_pushcclosure(L, _newindexHandler, 1);

	// {} M{}
	lua_rawset(L, metaTable);
	}
	
	// Set main table as global typename
	// {}
	lua_pop(L, 1);

	// -
	lua_setglobal(L, type.c_str());
}

void Binder::bind(lua_State* L){
	bind(L, "Script", ScriptBind::methods);
	bind(L, "Vec3", Vec3Bind::methods, Vec3Bind::meta, Vec3Bind::getters, Vec3Bind::setters);
}