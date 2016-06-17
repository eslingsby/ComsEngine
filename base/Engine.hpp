#pragma once

#include "EntityManager.hpp"

#include <vector>
#include <chrono>

class Engine{
	EntityManager* const _manager;

	std::vector<BaseSystem*> _systems;
	std::vector<uint32_t> _updateOrder;

	uint32_t _systemCount = 0;

	bool _running = false;
	int _exit = 0;

	inline void _loadSystems();
	inline void _updateSystems();

public:
	Engine();
	virtual ~Engine();

	template <typename T, typename ...Ts>
	void addSystem(Ts... args);

	virtual int run();
};

inline void Engine::_loadSystems(){
	for (uint32_t i : _updateOrder){
		if (i)
			_systems[i - 1]->load();
	}
}

inline void Engine::_updateSystems(){
	for (uint32_t i : _updateOrder){
		if (i)
			_systems[i - 1]->update();
	}
}

template<typename T, typename ...Ts>
void Engine::addSystem(Ts ...args){
	assert(!_running);

	if (_systems.size() <= T::type()){
		_systems.resize(_systemCount + 1);
		_updateOrder.resize(_systemCount + 1);
	}
	else{
		assert(_systems[T::type()] == nullptr);
	}

	T* system = new T(_manager, args...);

	_systems[T::type()] = system;
	_updateOrder[_systemCount] = T::type() + 1;

	_systemCount++;

	_manager->_registerSystem(system);
}