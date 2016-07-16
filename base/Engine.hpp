#pragma once

#include "EntityManager.hpp"

#include <vector>
#include <chrono>
#include <stack>
#include <unordered_map>
#include <string>

class Engine{
	std::vector<BaseSystem*> _systems;

	std::vector<uint32_t> _loadOrder;
	std::vector<uint32_t> _updateOrder;

	uint32_t _systemCount = 0;

	bool _running = false;
	int _exitCode = 0;

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

	// Call system update methods in order of added
	inline void _updateSystems();

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

	// Manually load and tick the engine (mainly for testing speed)
	inline void load();
	inline void update();

	// Automatically load and tick the engine
	int run();

	// Converts exe path to root folder
	static std::string root(std::string exePath);
};

inline void Engine::_loadSystems(){
	for (uint32_t i : _loadOrder){
		if (i)
			_systems[i - 1]->load();
	}

	manager.eraseDestroyed();
}

inline void Engine::_updateSystems(){
	for (uint32_t i : _updateOrder){
		if (i)
			_systems[i - 1]->update();
	}

	manager.eraseDestroyed();
}

template<typename T, typename ...Ts>
inline void Engine::addSystem(uint32_t loadIndex, uint32_t updateIndex, Ts ...args){
	assert(!_running);
	
	T* system = new T(this, args...);

	if (_systems.size() <= T::type())
		_systems.resize(T::type() + 1);

	assert(_systems[T::type()] == nullptr);

	_systems[T::type()] = system;

	manager.registerSystem(system);

	if (loadIndex >= _loadOrder.size())
		_loadOrder.resize(loadIndex + 1);

	if (updateIndex >= _updateOrder.size())
		_updateOrder.resize(updateIndex + 1);
	
	assert(_loadOrder[loadIndex] == 0 && _updateOrder[updateIndex] == 0);

	_loadOrder[loadIndex] = T::type() + 1;
	_updateOrder[updateIndex] = T::type() + 1;

	_systemCount++;
}

template<typename T>
inline T* const Engine::getSystem(){
	assert(_systems.size() > T::type() && _systems[T::type()]);

	return static_cast<T*>(_systems[T::type()]);
}

template <typename T>
inline T Engine::deltaTime(){
	return std::chrono::duration_cast<std::chrono::duration<T>>(_dt).count();
}

inline std::string Engine::getConfig(const std::string& key){
	auto config = _config.find(key);

	if (config != _config.end())
		return config->second;

	return "";
}

inline void Engine::setConfig(const std::string& key, const std::string& value){
	_config[key] = value;
}

inline void Engine::shutdown(bool abort){
	assert(_running);

	if (abort)
		_exitCode = 1;

	_running = false;
}

inline void Engine::load(){
	assert(!_running && _systemCount > 0);

	_start = Clock::now();
	_end = _start;

	_dt = _end - _start;

	_running = true;

	_loadSystems();
}

inline void Engine::update(){
	assert(_running && _systemCount > 0);

	_dt = _end - _start;
	_start = Clock::now();

	_updateSystems();

	_end = Clock::now();

	if (!_running){
		// check for shutdown or restart
	}
}