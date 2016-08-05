#include "Scripting.hpp"

#include "Engine.hpp"
#include "Binder.hpp"

#include <iostream>
#include <thread>
#include <atomic>

#define SCRIPTING_CONSOLE

#ifdef SCRIPTING_CONSOLE
std::thread* _console = nullptr;

std::atomic<bool> _consoleDone = false;
std::string _consoleCommand = "";

static void consoleThread(){
	std::string message = "";

	while (message == ""){
		std::cout << ">>> ";
		std::getline(std::cin, message);
	}

	_consoleCommand = message;
	_consoleDone = true;
}
#endif

Scripting::Scripting(Engine* engine, const std::string& scriptPath) : System(engine), _L(luaL_newstate()){
	if (scriptPath != "")
		_scriptPath = scriptPath + "\\";
	else
		_scriptPath = "";
}

Scripting::~Scripting(){
	lua_close(_L);
}

void Scripting::load(){
	_input = _engine.getSystem<Input>();

	_input->addInput("reload", SDL_SCANCODE_LCTRL, SDL_SCANCODE_R);

	luaL_openlibs(_L);

	if (_scriptPath == "")
		_scriptPath = _engine.getConfig("data");

	Binder::bind(_L, _engine);
	
	callFile("Load.lua");

#ifdef SCRIPTING_CONSOLE
	_console = new std::thread(consoleThread);
#endif
}

void Scripting::update(){
#ifdef SCRIPTING_CONSOLE
	if (_consoleDone == true){
		// process command

		if (luaL_dostring(_L, _consoleCommand.c_str())){
			std::cout << "\n" << lua_tostring(_L, -1) << "\n\n";
			lua_pop(_L, 1);
		}

		_consoleCommand = "";
		_consoleDone = false;

		_console->join();
		delete _console;

		_console = new std::thread(consoleThread);
	}
#endif

	if (_input->wasDown("reload")){
		callFile("Load.lua");
		_engine.reset();

		_reloaded = true;
	}

	_engine.manager.processEntities(this);
	lua_gc(_L, LUA_GCCOLLECT, 0);

	if (_reloaded)
		_reloaded = false;
}

void Scripting::onProcess(uint64_t id, Script& script){
	if (!script.references || !script.references->size())
		return;

	for (Script::RefMap::iterator i = script.references->begin(); i != script.references->end(); i++){
		for (auto reference : i->second){
			if (!reference.first)
				continue;
			
			// {}
			lua_rawgeti(_L, LUA_REGISTRYINDEX, reference.second);
			
			if (_reloaded){
				// {} function()
				lua_getfield(_L, -1, "reset");

				if (!lua_isnil(_L, -1)){
					// {} function() {}
					lua_rawgeti(_L, LUA_REGISTRYINDEX, reference.second);

					// {}
					if (lua_pcall(_L, 1, 0, 0)){
						std::cout << lua_tostring(_L, -1) << "\n";
						lua_pop(_L, 1);
					}
				}
				else{
					lua_pop(_L, 1);
				}
			}

			// {}

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
			else{
				lua_pop(_L, 1);
			}
			
			// -
			lua_pop(_L, 1);
		}
	}
}

void Scripting::callFile(const std::string& file){
	if (luaL_dofile(_L, (_scriptPath + file).c_str())){
		std::cout << lua_tostring(_L, -1) << "\n";
		lua_pop(_L, 1);
	}
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

	if (_engine.manager.getEntityState(id) != EntityManager::EntityState::Destroyed)
		_engine.manager.setComponentEnabled<Script>(id, true);
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

	bool empty = true;

	for (auto& referances : script->references->at(type)){
		if (referances.first){
			empty = false;
			break;
		}
	}

	if (empty)
		script->references->erase(type);

	if (_engine.manager.getEntityState(id) != EntityManager::EntityState::Destroyed)
		_engine.manager.setComponentEnabled<Script>(id, false);
}

int Scripting::getInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);

	assert(script);

	assert(script->references);

	assert(script->references->at(type).size() > number);

	auto& referance = script->references->at(type)[number];

	return referance.second;
}

void Scripting::registerFile(const std::string& type, const std::string& file){
	std::string name = _scriptPath + file.c_str();

	// function()
	if (luaL_loadfile(_L, name.c_str())){
		printf("%s\n", lua_tostring(_L, -1));
		lua_pop(_L, 1);
		return;
	}

	// {}
	if (lua_pcall(_L, 0, 1, 0)){
		printf("%s\n", lua_tostring(_L, -1));
		lua_pop(_L, 1);
		return;
	}

	// {} M{}
	luaL_newmetatable(_L, type.c_str());

	// M{} {}
	lua_insert(_L, -2);

	// M{}
	lua_setfield(_L, -2, "__index");

	// -
	lua_pop(_L, 1);

	std::string combined = type + ";" + file;

	if (_loadedScripts.find(combined) == _loadedScripts.end())
		_loadedScripts.insert(combined);
}

void Scripting::onCreate(uint64_t id){
	_engine.manager.setComponentEnabled<Script>(id, false);
}

void Scripting::onDestroy(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	if (!script->references)
		return;

	for (Script::RefMap::iterator i = script->references->begin(); i != script->references->end(); i++){
		for (auto referance : i->second)
			if (referance.first)
				luaL_unref(_L, LUA_REGISTRYINDEX, referance.second);
	}

	delete script->references;
}