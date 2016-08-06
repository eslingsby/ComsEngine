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
		printf(">>> ");
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
		if (luaL_dostring(_L, _consoleCommand.c_str()))
			Binder::printStackError(_L);
		
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
	for (unsigned int i = 0; i < Script::maxReferences; i++){
		if (_engine.manager.getEntityState(id) != EntityManager::EntityState::Active)
			return;

		if (script.testingReferences[i] != -1){
			int reference = script.testingReferences[i];
	
			// {}
			lua_rawgeti(_L, LUA_REGISTRYINDEX, reference);
	
			if (_reloaded){
				// {} function()
				lua_getfield(_L, -1, "reset");
	
				if (!lua_isnil(_L, -1)){
					// {} function() {}
					lua_rawgeti(_L, LUA_REGISTRYINDEX, reference);
	
					// {}
					if (lua_pcall(_L, 1, 0, 0))
						Binder::printStackError(_L);
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
				lua_rawgeti(_L, LUA_REGISTRYINDEX, reference);
	
				// {}
				if (lua_pcall(_L, 1, 0, 0))
					Binder::printStackError(_L);
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
	if (luaL_dofile(_L, (_scriptPath + file).c_str()))
		Binder::printStackError(_L);
}

bool Scripting::createInstance(uint64_t id, const std::string& type){
	Script* script = _engine.manager.getComponent<Script>(id);
	
	if (!script)
		return false;
	
	// {}
	lua_newtable(_L);

	// {} M{}
	luaL_getmetatable(_L, type.c_str());

	if (lua_isnil(_L, -1)){
		lua_pop(_L, 2);
		return false;
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
	int reference;
	
	for (unsigned int i = 0; i < Script::maxReferences; i++){
		if (script->testingReferences[i] == -1){
			reference = luaL_ref(_L, LUA_REGISTRYINDEX);
			strcat_s(script->testingIdentifiers[i], type.c_str());
			script->testingReferences[i] = reference;
			break;
		}
	
		if (i == Script::maxReferences - 1)
			return false;
	}

	// -
	if (!lua_isnil(_L, -1)){
		lua_rawgeti(_L, LUA_REGISTRYINDEX, reference);

		if (lua_pcall(_L, 1, 0, 0))
			Binder::printStackError(_L);
	}
	else{
		lua_pop(_L, 1);
	}

	return true;
}

bool Scripting::destroyInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);

	if (!script)
		return false;

	unsigned int index = 0;

	for (unsigned int i = 0; i < Script::maxReferences; i++){
		if (script->testingReferences[i] != -1){
			const char* identifier = script->testingIdentifiers[i];
			int reference = script->testingReferences[i];
		
			if (identifier == type.c_str() && index == number){
				luaL_unref(_L, LUA_REGISTRYINDEX, reference);
				script->testingReferences[i] = -1;
				strcpy_s(script->testingIdentifiers[i], "");
				return true;
			}

			if (identifier == type.c_str())
				index++;
		}
	}

	return false;
}

int Scripting::getInstance(uint64_t id, const std::string& type, unsigned int number){
	Script* script = _engine.manager.getComponent<Script>(id);

	if (!script)
		return -1;

	unsigned int index = 0;

	for (unsigned int i = 0; i < Script::maxReferences; i++){
		if (script->testingReferences[i] != -1){
			const char* identifier = script->testingIdentifiers[i];

			if (identifier == type.c_str() && index == number)
				return script->testingReferences[i];

			if (identifier == type.c_str())
				index++;
		}
	}

	return -1;
}

bool Scripting::registerFile(const std::string& type, const std::string& file){
	std::string name = _scriptPath + file.c_str();

	// function()
	if (luaL_loadfile(_L, name.c_str())){
		Binder::printStackError(_L);
		return false;
	}

	// {}
	if (lua_pcall(_L, 0, 1, 0)){
		Binder::printStackError(_L);
		return false;
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

	return true;
}

void Scripting::onCreate(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	memset(script->testingReferences, -1, sizeof(script->testingReferences));
	memset(script->testingIdentifiers, 0, sizeof(script->testingIdentifiers));
}

void Scripting::onDestroy(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	for (unsigned int i = 0; i < Script::maxReferences; i++){
		int reference = script->testingReferences[i];

		if (reference != -1)
			luaL_unref(_L, LUA_REGISTRYINDEX, reference);
	}
}