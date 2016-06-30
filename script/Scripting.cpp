#include "Scripting.hpp"

#include "Engine.hpp"
#include "StaticEngine.hpp"

#include "Binder.hpp"

#include <iostream>
#include <thread>

Scripting::Scripting(Engine* engine) : System(engine), L(luaL_newstate()){}

Scripting::~Scripting(){
	lua_close(L);
}

void Scripting::load(){
	Binder::bind(L);
	
	if (luaL_dofile(L, (_engine.getConfig("data") + "Load.lua").c_str()))
		std::cout << lua_tostring(L, -1) << "\n";
	
	registerFile("Test.lua");

	uint64_t id = _engine.manager.createEntity();

	Script* script = _engine.manager.addComponent<Script>(id);

	createInstance(id, *script, "Test");
	createInstance(id, *script, "Test");
	createInstance(id, *script, "Test");
	createInstance(id, *script, "Test");
}

void Scripting::update(){
	_engine.manager.processEntities(this);
}

void Scripting::onProcess(uint64_t id, Script& script){
	for (auto type : script.references){
		for (int ref : type.second){
			// {}
			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

			// {} function()
			lua_getfield(L, -1, "update");

			if (!lua_isnil(L, -1)){
				if (lua_pcall(L, 0, 0, 0)){
					std::cout << lua_tostring(L, -1) << "\n";
					_engine.manager.setComponentEnabled<Script>(id, false);

					return;
				}
			}

			// -
			lua_pop(L, 1);
		}
	}
}

void Scripting::registerFile(const std::string& file){
	// function()
	assert(!luaL_loadfile(L, (_engine.getConfig("data") + file).c_str()));
	
	// function() {}
	lua_newtable(L);
	
	// function() {} M{}
	luaL_getmetatable(L, "Script");
	
	// function() {}
	lua_setmetatable(L, -2);
	
	// function() {} {}
	lua_pushvalue(L, -1);
	
	// {} function() {} 
	lua_insert(L, -3);
	
	// {}
	assert(!lua_pcall(L, 1, 1, 0));

	// {} string
	std::string type = lua_tostring(L, -1);

	// {}
	lua_setfield(L, -2, "type");
	
	// M{} {}
	luaL_newmetatable(L, type.c_str());
	lua_insert(L, -2);

	// M{}
	lua_setfield(L, -2, "__index");

	// -
	lua_pop(L, 1);
}

void Scripting::createInstance(uint64_t id, Script& script, const std::string& meta){
	// {}
	lua_newtable(L);

	// {} M{}
	luaL_getmetatable(L, meta.c_str());

	// {}
	lua_setmetatable(L, -2);
	
	// {} int
	lua_pushinteger(L, id);

	// {}
	lua_setfield(L, -2, "id");

	// {} string
	lua_getfield(L, -1, "type");

	// {}
	std::string type = lua_tostring(L, -1);
	lua_pop(L, 1);

	// function() {}
	lua_getfield(L, -1, "load");
	lua_insert(L, -2);

	// function()
	int reference = luaL_ref(L, LUA_REGISTRYINDEX);
	assert(script.references[type].find(reference) == script.references[type].end());

	script.references[type].insert(reference);

	// -
	if (!lua_isnil(L, -1))
		assert(!lua_pcall(L, 0, 0, 0));
	else
		lua_pop(L, 1);
}

void Scripting::onCreate(uint64_t id){
	// Run raw lua string here, passed via component constructor
}

void Scripting::onDestroy(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	for (auto type : script->references)
		for (int ref : type.second)
			luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

void Scripting::onActivate(uint64_t id){}

void Scripting::onDeactivate(uint64_t id){}
