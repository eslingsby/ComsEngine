#include "Binder.hpp"

#include "Scripting.hpp"

#include "EngineBind.hpp"
#include "EntityBind.hpp"
#include "IdentificationBind.hpp"
#include "IdentifierBind.hpp"
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

void Binder::bind(lua_State* L, const std::string& type, lua_CFunction constructor, const luaL_Reg* global, const luaL_Reg* instance, const luaL_Reg* meta, const MemberReg* getters, const MemberReg* setters){
	// Register methods
	// {}
	lua_newtable(L);
	int methodsTable = lua_gettop(L);

	if (instance)
		luaL_setfuncs(L, instance, 0);
	
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

		_setMembers(L, getters);

		// {} M{} literal M{} {}
		lua_pushvalue(L, methodsTable);

		// {} M{} literal function()
		lua_pushcclosure(L, _indexHandler, 2);

		// {} M{}
		lua_rawset(L, metaTable);
	}
	else if (instance){
		lua_pushliteral(L, "__index");

		lua_pushvalue(L, methodsTable);

		lua_rawset(L, metaTable);
	}
	
	// Register member setters
	if (setters){
		// {} M{} literal
		lua_pushliteral(L, "__newindex");

		// {} M{} literal {}
		lua_newtable(L);

		_setMembers(L, setters);

		// {} M{} literal function()
		lua_pushcclosure(L, _newindexHandler, 1);

		// {} M{}
		lua_rawset(L, metaTable);
	}

	// {} M{} M{}
	lua_newtable(L);
	int globalMeta = lua_gettop(L);

	if (constructor){
		const luaL_Reg constructorReg[] = {
			{ "__call", constructor },
			{ 0, 0 }
		};

		luaL_setfuncs(L, constructorReg, 0);
	}

	// {} M{} M{} {}
	lua_newtable(L);
	int globalTable = lua_gettop(L);

	if (global)
		luaL_setfuncs(L, global, 0);

	// {} M{} M{} {} M{}
	lua_pushvalue(L, globalMeta);

	// {} M{} M{} {}
	lua_setmetatable(L, globalTable);

	// {} M{} M{}
	lua_setglobal(L, type.c_str());

	// -
	lua_pop(L, 3);
}

void Binder::bind(lua_State* L, Engine& engine){
	lua_pushlightuserdata(L, &engine);
	lua_setglobal(L, "__engine");

	Scripting* scripting = engine.getSystem<Scripting>();

	assert(scripting);

	bind(L, EngineBind::name, 0, EngineBind::global);
	bind(L, EntityBind::name, EntityBind::constructor, 0, EntityBind::methods, EntityBind::meta);
	bind(L, IdentificationBind::name, 0, IdentificationBind::global);bind(L, IdentificationBind::name, 0, IdentificationBind::global);
	bind(L, IdentifierBind::name, IdentifierBind::constructor, IdentifierBind::global, EntityBind::methods, EntityBind::meta, IdentifierBind::getters);
	bind(L, Vec3Bind::name, Vec3Bind::constructor, Vec3Bind::global, Vec3Bind::methods, Vec3Bind::meta, Vec3Bind::getters, Vec3Bind::setters);
}