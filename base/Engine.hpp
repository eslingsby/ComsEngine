#pragma once

#include "EntityManager.hpp"

#include <vector>

class EntityManager;

class Engine{
	EntityManager* const _manager;

	std::vector<BaseSystem*> _systems;
	std::vector<uint32_t> _updateOrder;

	uint32_t _systemCount = 0;

	bool _running = false;
	int _exit = 0;

	void _loadSystems();
	void _updateSystems();

public:
	Engine();
	virtual ~Engine();

	template <typename T, typename ...Ts>
	void addSystem(Ts... args);

	virtual int run();
};

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