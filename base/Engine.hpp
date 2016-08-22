#pragma once

#include "EntityManager.hpp"

#include <vector>
#include <chrono>
#include <stack>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

class Engine{
	std::unordered_map<std::type_index, BaseSystem*> _systems;
	
	std::vector<std::type_index> _loadOrder;
	std::vector<std::type_index> _updateOrder;

	uint32_t _systemCount = 0;

	bool _running = false;
	int _exitCode = 0;

	bool _reset = false;

	using Clock = std::chrono::high_resolution_clock;

	std::unordered_map<std::string, std::string> _config;
	
	Clock::time_point _start;
	Clock::time_point _end;

	Clock::duration _dt;

	// Non-copyable overloads
	Engine(const Engine& other) = delete;
	Engine& operator=(const Engine& other) = delete;

	// Call system load methods in order of added
	inline void _loadSystems();

	inline void _resetSystems();

	// Call system update methods in order of added
	inline void _updateSystems();

	inline void _exitSystems();

public:
	EntityManager manager;

	Engine();
	virtual ~Engine();

	// Create and add unique system to engine
	template <typename T, typename ...Ts>
	void addSystem(uint32_t loadIndex, uint32_t updateIndex, Ts... args);

	// Get unique system
	template <typename T>
	inline T* const getSystem();

	// Get length of previous update tick in floating point seconds
	template <typename T = long double>
	inline T deltaTime();

	// Get string in config map (eventually will read config from file at startup)
	inline std::string getConfig(const std::string& key);

	// Set string in config map (eventually will save config to file at reload or shutdown)
	inline void setConfig(const std::string& key, const std::string& value);
	
	// Stop the engine from running if using run method
	inline void shutdown(bool abort = false);

	inline void reset(bool data = false);

	// Each step of the run function (exposoed publicly for testing)
	virtual inline void init(int argc, char* argv[]);
	virtual inline void load();
	virtual inline void start(){}
	virtual inline void update();
	virtual inline int exit();

	// Automatically load and tick the engine
	int run(int argc, char* argv[]);
};

inline void Engine::_loadSystems(){
	// Call load on all systems in load order
	for (std::type_index i : _loadOrder)
		_systems[i]->load();

	manager.eraseDestroyed();
}

inline void Engine::_resetSystems(){
	// Call reset on all systems in load order
	for (std::type_index i : _loadOrder)
		_systems[i]->reset();

	manager.eraseDestroyed();
}

inline void Engine::_updateSystems(){
	// Call update on all systems in update order
	for (std::type_index i : _updateOrder)
		_systems[i]->update();

	manager.eraseDestroyed();
}

inline void Engine::_exitSystems(){
	// Call exit on all systems in load order
	for (std::type_index i : _loadOrder)
		_systems[i]->exit();

	manager.eraseDestroyed();
}

template<typename T, typename ...Ts>
inline void Engine::addSystem(uint32_t loadIndex, uint32_t updateIndex, Ts ...args){
	assert(!_running);
	
	// Create system in dynamic memory
	T* system = new T(this, args...);

	assert(_systems.find(typeid(T)) == _systems.end());

	// Add system to system map
	_systems[typeid(T)] = system;

	// Register system with entity manager
	manager.registerSystem(system);

	// Resize ordering vectors
	if (loadIndex >= _loadOrder.size())
		_loadOrder.resize(loadIndex + 1, typeid(nullptr));

	if (updateIndex >= _updateOrder.size())
		_updateOrder.resize(updateIndex + 1, typeid(nullptr));
	
	// Add system call order
	_loadOrder[loadIndex] = typeid(T);
	_updateOrder[updateIndex] = typeid(T);

	_systemCount++;
}

template<typename T>
inline T* const Engine::getSystem(){
	assert(_systems.find(typeid(T)) != _systems.end());

	return static_cast<T*>(_systems[typeid(T)]);
}

template <typename T>
inline T Engine::deltaTime(){
	return std::chrono::duration_cast<std::chrono::duration<T>>(_dt).count();
}

inline std::string Engine::getConfig(const std::string& key){
	auto config = _config.find(key);

	if (config != _config.end())
		return config->second;

	// If key doesn't exist return ""
	return "";
}

inline void Engine::setConfig(const std::string& key, const std::string& value){
	// Set key (or overwrite)
	_config[key] = value;
}

inline void Engine::shutdown(bool abort){
	assert(_running);

	if (abort)
		_exitCode = 1;

	_running = false;
}

inline void Engine::reset(bool data){
	_reset = true;
}

inline void Engine::init(int argc, char * argv[]){
	// Shorten exe path to root directory
	std::string root = argv[0];

	root = root.substr(0, root.find_last_of("\\"));
	root = root.substr(0, root.find_last_of("\\") + 1);

	// Add root directory as config key
	setConfig("root", root);
	// Add data folder as config key
	setConfig("data", root + "data\\");
	setConfig("name", "ComsEngine");
}

inline void Engine::load(){
	assert(!_running && _systemCount > 0);

	// Initiate clocks
	_start = Clock::now();
	_end = _start;

	_dt = _end - _start;

	_running = true;

	// Load all systems
	_loadSystems();
}

inline void Engine::update(){
	assert(_running && _systemCount > 0);

	// Calculate delta time
	_dt = _end - _start;
	// Start timer
	_start = Clock::now();

	// Update all systems
	_updateSystems();

	// End timer
	_end = Clock::now();

	// If reset was triggered (experimental)
	if (_reset){
		_resetSystems();
		manager.purge();
		manager.eraseDestroyed();
		_reset = false;
	}
}

inline int Engine::exit(){
	assert(_systemCount > 0);

	_exitSystems();

	return _exitCode;
}