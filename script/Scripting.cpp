#include "Scripting.hpp"

#include "Engine.hpp"
#include "Binder.hpp"

#include <iostream>
#include <thread>
#include <atomic>

#define SCRIPTING_CONSOLE

#ifdef SCRIPTING_CONSOLE
// Console thread
std::thread* _console = nullptr;

// Console thread shared global data
std::atomic<bool> _consoleDone = false;
std::string _consoleCommand = "";

static void consoleThread(){
	std::string message = "";

	// Wait for user input via console
	while (message == ""){
		printf(">>> ");
		std::getline(std::cin, message);
	}

	// If input, set as done, and finish
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

	// Add CTRL+R input for triggering reload
	_input->addInput("reload", SDL_SCANCODE_LCTRL, SDL_SCANCODE_R);

	// Load Lua libs
	luaL_openlibs(_L);

	// Set path to script folder
	if (_scriptPath == "")
		_scriptPath = _engine.getConfig("data");

	// Bind all C code to Lua
	Binder::bind(_L, _engine);

	// Call the load file
	callFile("Load.lua");

	// Set start script file if not set
	if (_engine.getConfig("script.start") == "")
		_engine.setConfig("script.start", "Start.lua");

	_startScript = _engine.getConfig("script.start");

#ifdef SCRIPTING_CONSOLE
	// Create console thread
	_console = new std::thread(consoleThread);
#endif
}

void Scripting::reset(){
	lua_close(_L);

	_L = luaL_newstate();

	luaL_openlibs(_L);

	_reset = true;
}

void Scripting::update(){
	// If first update call
	if (_started){
		// Call the start script
		callFile(_startScript);
		_started = false;
	}

#ifdef SCRIPTING_CONSOLE
	// Check for console input
	if (_consoleDone == true){
		// Execute console input
		if (luaL_dostring(_L, _consoleCommand.c_str()))
			Binder::printStackError(_L);
		
		// Reset console thread
		_consoleCommand = "";
		_consoleDone = false;

		_console->join();
		delete _console;

		_console = new std::thread(consoleThread);
	}
#endif

	// Check for CTRL+R input
	if (_input->wasDown("reload")){
		// Trigger a reload by recalling load.lua
		callFile("Load.lua");
		_reloaded = true;
	}

	// Process all script components
	_engine.manager.processEntities(this);

	// Check lua garbage collection
	lua_gc(_L, LUA_GCCOLLECT, 0);

	if (_reloaded)
		_reloaded = false;
}

void Scripting::onProcess(uint64_t id, Script& script){
	// For each Lua object in entity script component
	for (unsigned int i = 0; i < Script::maxReferences; i++){
		// If entity is no longer active (from previous loop) abort
		if (_engine.manager.getEntityState(id) != EntityManager::EntityState::Active)
			return;

		// If Lua object reference is valid
		if (script.references[i] != -1){
			int reference = script.references[i];
	
			// Push lua object onto lua stack
			// {}
			lua_rawgeti(_L, LUA_REGISTRYINDEX, reference);
	
			// If reload was called
			if (_reloaded){
				// Trigger reload function on lua object

				// {} function()
				lua_getfield(_L, -1, "reload");
	
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

			// If entity has changed state from active, abort
			if (_engine.manager.getEntityState(id) != EntityManager::EntityState::Active){
				lua_pop(_L, 1);
				return;
			}

			// Call update function on lua object
	
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
	// Run lua file
	if (luaL_dofile(_L, (_scriptPath + file).c_str()))
		Binder::printStackError(_L);
}

bool Scripting::createInstance(uint64_t id, const std::string& type){
	Script* script = _engine.manager.getComponent<Script>(id);
	
	if (!script)
		return false;

	// Create new lua object
	
	// {}
	lua_newtable(_L);

	// Assign it to component type

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

	// Create new entity reference to base lua object on

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
		if (script->references[i] == -1){
			reference = luaL_ref(_L, LUA_REGISTRYINDEX);
			strcat_s(script->identifiers[i], type.c_str());
			script->references[i] = reference;
			break;
		}
	
		if (i == Script::maxReferences - 1)
			return false;
	}

	// Call load function in lua object

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

	// For each lua object in entity script component
	for (unsigned int i = 0; i < Script::maxReferences; i++){
		if (script->references[i] != -1){
			const char* identifier = script->identifiers[i];
			int reference = script->references[i];
		
			// If lua instance equals desired number, unreference
			if (type == identifier && index == number){
				luaL_unref(_L, LUA_REGISTRYINDEX, reference);
				script->references[i] = -1;
				strcpy_s(script->identifiers[i], "");
				return true;
			}

			if (type == identifier)
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

	// For each lua object in entity script component
	for (unsigned int i = 0; i < Script::maxReferences; i++){
		if (script->references[i] != -1){
			const char* identifier = script->identifiers[i];

			// If lua instance equals desired number, return reference
			if (type == identifier && index == number)
				return script->references[i];

			if (type == identifier)
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

	// Call script lua script file

	// {}
	if (lua_pcall(_L, 0, 1, 0)){
		Binder::printStackError(_L);
		return false;
	}

	// Define (or get) new metatable of type name

	// {} M{}
	luaL_newmetatable(_L, type.c_str());

	// M{} {}
	lua_insert(_L, -2);

	// Set metatable of table returned from script to new type named metatable

	// M{}
	lua_setfield(_L, -2, "__index");

	// -
	lua_pop(_L, 1);

	// Old code
	std::string combined = type + ";" + file;

	if (_loadedScripts.find(combined) == _loadedScripts.end())
		_loadedScripts.insert(combined);

	return true;
}

void Scripting::onCreate(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	// Set component data blank
	memset(script->references, -1, sizeof(script->references));
	memset(script->identifiers, 0, sizeof(script->identifiers));
}

void Scripting::onDestroy(uint64_t id){
	Script* script = _engine.manager.getComponent<Script>(id);

	// Un-reference all lua object attached to component
	for (unsigned int i = 0; i < Script::maxReferences; i++){
		int reference = script->references[i];

		if (reference != -1)
			luaL_unref(_L, LUA_REGISTRYINDEX, reference);
	}
}