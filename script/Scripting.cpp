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
	luaL_openlibs(L);

	Binder::bind(L, _engine);
	
	if (luaL_dofile(L, (_engine.getConfig("data") + "Load.lua").c_str())){
		std::cout << lua_tostring(L, -1) << "\n";
		lua_pop(L, 1);
	}
	
	registerFile("Test.lua");
	registerFile("Other.lua");

	uint64_t id = _engine.manager.createEntity();

	_engine.manager.addComponent<Script>(id);

	createInstance(id, "Test");
	//createInstance(id, "Other");
}

void Scripting::update(){
	_engine.manager.processEntities(this);
}

void Scripting::onProcess(uint64_t id, Script& script){
	for (auto type : script.references){
		for (auto reference : type.second){

			if (!reference.first)
				continue;

			// {}
			lua_rawgeti(L, LUA_REGISTRYINDEX, reference.second);

			// {} function()
			lua_getfield(L, -1, "update");

			if (!lua_isnil(L, -1)){
				// {} function() {}
				lua_rawgeti(L, LUA_REGISTRYINDEX, reference.second);

				// {}
				if (lua_pcall(L, 1, 0, 0)){
					std::cout << lua_tostring(L, -1) << "\n";
					lua_pop(L, 1);
				}
			}

			// -
			lua_pop(L, 1);
		}
	}

	lua_gc(L, LUA_GCCOLLECT, 0);
}

void Scripting::registerFile(const std::string& file){
	// function()
	if (luaL_loadfile(L, (_engine.getConfig("data") + file).c_str())){
		std::cout << lua_tostring(L, -1) << "\n";
		lua_pop(L, 2);
		return;
	}
	
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
	if (lua_pcall(L, 1, 1, 0)){
		std::cout << lua_tostring(L, -1) << "\n";
		lua_pop(L, 2);
		return;
	}

	// {} string
	std::string type = lua_tostring(L, -1);

	// {}
	lua_setfield(L, -2, "type");
	
	// M{} {}
	if (!luaL_newmetatable(L, type.c_str()))
		luaL_getmetatable(L, type.c_str());

	lua_insert(L, -2);

	// M{}
	lua_setfield(L, -2, "__index");

	// -
	lua_pop(L, 1);
}

void Scripting::createInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);
	assert(script);

	// {}
	lua_newtable(L);

	// {} M{}
	luaL_getmetatable(L, type.c_str());

	if (lua_isnil(L, -1)){
		lua_pop(L, 2);
		return;
	}

	// {}
	lua_setmetatable(L, -2);
	
	// function() {}
	lua_getfield(L, -1, "load");
	lua_insert(L, -2);

	// function()
	int reference = luaL_ref(L, LUA_REGISTRYINDEX);
	
	if (script->references[type].size() <= number)
		script->references[type].resize(number + 1);

	auto& referance = script->references[type][number];

	assert(!referance.first);

	referance.first = true;
	referance.second = reference;

	// -
	if (!lua_isnil(L, -1)){
		lua_rawgeti(L, LUA_REGISTRYINDEX, referance.second);

		if (lua_pcall(L, 1, 0, 0)){
			std::cout << lua_tostring(L, -1) << "\n";
			lua_pop(L, 1);
		}
	}
	else{
		lua_pop(L, 1);
	}
}

void Scripting::destroyInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);
	assert(script);

	assert(script->references[type].size() > number);

	auto& i = script->references[type][number];

	assert(i.first);

	i.first = false;

	luaL_unref(L, LUA_REGISTRYINDEX, i.second);
}

void Scripting::onCreate(uint64_t id){
	// Run raw lua string here, passed via component constructor
}

void Scripting::onDestroy(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	// Important! Safely unreference scripts on destroy!

	for (auto type : script->references)
		for (auto ref : type.second)
			luaL_unref(L, LUA_REGISTRYINDEX, ref.second);
}