#include "Scripting.hpp"

#include "Engine.hpp"

#include "EntityRef.hpp"
#include "Binder.hpp"
#include "Identification.hpp"

#include <iostream>

Scripting::Scripting(Engine* engine) : System(engine), _L(luaL_newstate()){}

Scripting::~Scripting(){
	lua_close(_L);
}

void Scripting::load(){
	luaL_openlibs(_L);

	Binder::bind(_L, _engine);
	
	if (luaL_dofile(_L, (_engine.getConfig("data") + "Load.lua").c_str())){
		std::cout << lua_tostring(_L, -1) << "\n";
		lua_pop(_L, 1);
	}

	// Testing
	registerFile("Test.lua");
	registerFile("Other.lua");
	
	uint64_t id = _engine.manager.createEntity();
	_engine.manager.addComponent<Identifier>(id, "camera");
	_engine.manager.addComponent<Script>(id);
	createInstance(id, "Other");
	
	for (unsigned int i = 0; i < 1024 * 4; i++){
		id = _engine.manager.createEntity();
		_engine.manager.addComponent<Script>(id);
		createInstance(id, "Test");
		//createInstance(id, "Test", 1);
	}
}

void Scripting::update(){
	_engine.manager.processEntities(this);

	lua_gc(_L, LUA_GCCOLLECT, 0);
}

void Scripting::onProcess(uint64_t id, Script& script){
	for (Script::RefMap::iterator i = script.references->begin(); i != script.references->end(); i++){
		for (auto reference : i->second){
			if (!reference.first)
				continue;
			
			// {}
			lua_rawgeti(_L, LUA_REGISTRYINDEX, reference.second);
			
			// {} function()
			lua_getfield(_L, -1, "update");
			
			if (!lua_isnil(_L, -1)){
				// {} function() {}
				lua_rawgeti(_L, LUA_REGISTRYINDEX, reference.second);
			
				// {}
				if (lua_pcall(_L, 1, 0, 0)){
					std::cout << lua_tostring(_L, -1) << "\n";
					lua_pop(_L, 1);
				}
			}
			
			// -
			lua_pop(_L, 1);
		}
	}
}

void Scripting::registerFile(const std::string& file){
	// function()
	if (luaL_loadfile(_L, (_engine.getConfig("data") + file).c_str())){
		std::cout << lua_tostring(_L, -1) << "\n";
		lua_pop(_L, 2);
		return;
	}
	
	// function() {}
	lua_newtable(_L);
	
	// function() {} {}
	lua_pushvalue(_L, -1);
	
	// {} function() {} 
	lua_insert(_L, -3);
	
	// {} string
	if (lua_pcall(_L, 1, 1, 0)){
		std::cout << lua_tostring(_L, -1) << "\n";
		lua_pop(_L, 2);
		return;
	}

	// {}
	std::string type = lua_tostring(_L, -1);
	lua_pop(_L, 1);
	
	// {} M{}
	if (!luaL_newmetatable(_L, type.c_str()))
		luaL_getmetatable(_L, type.c_str());

	// M{} {}
	lua_insert(_L, -2);

	// M{}
	lua_setfield(_L, -2, "__index");

	// -
	lua_pop(_L, 1);
}

void Scripting::createInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);
	assert(script);
	
	// {}
	lua_newtable(_L);

	// {} M{}
	luaL_getmetatable(_L, type.c_str());

	if (lua_isnil(_L, -1)){
		lua_pop(_L, 2);
		return;
	}

	// {}
	lua_setmetatable(_L, -2);
	
	// function() {}
	lua_getfield(_L, -1, "load");
	lua_insert(_L, -2);

	// function() {} _L{}
	void* location = lua_newuserdata(_L, sizeof(EntityRef));
	new(location) EntityRef(Binder::getEngine(_L).manager, id);

	// function() {} _L{} M{}
	luaL_getmetatable(_L, "Entity");

	// function() {} _L{}
	lua_setmetatable(_L, -2);

	// function() {}
	lua_setfield(_L, -2, "entity");

	// function()
	int reference = luaL_ref(_L, LUA_REGISTRYINDEX);
	
	if (!script->references)
		script->references = new Script::RefMap();

	if (script->references->find(type) == script->references->end())
		script->references->insert({ type, Script::RefVec() });

	if (script->references->at(type).size() <= number)
		script->references->at(type).resize(number + 1);

	auto& referance = script->references->at(type)[number];

	assert(!referance.first);

	referance.first = true;
	referance.second = reference;

	// -
	if (!lua_isnil(_L, -1)){
		lua_rawgeti(_L, LUA_REGISTRYINDEX, referance.second);

		if (lua_pcall(_L, 1, 0, 0)){
			std::cout << lua_tostring(_L, -1) << "\n";
			lua_pop(_L, 1);
		}
	}
	else{
		lua_pop(_L, 1);
	}
}

void Scripting::destroyInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);
	assert(script);

	assert(script->references);

	assert(script->references->at(type).size() > number);

	auto& referance = script->references->at(type)[number];

	assert(referance.first);
	referance.first = false;

	luaL_unref(_L, LUA_REGISTRYINDEX, referance.second);
}

void Scripting::onCreate(uint64_t id){
	// Run raw lua string here, passed via component constructor
}

void Scripting::onDestroy(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	if (!script->references)
		return;

	for (Script::RefMap::iterator i = script->references->begin(); i != script->references->end(); i++)
		for (auto referance : i->second)
			if (referance.first)
				luaL_unref(_L, LUA_REGISTRYINDEX, referance.second);

	delete script->references;
}