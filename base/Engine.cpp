#include "Engine.hpp"

void Engine::_updateSystems(){
	for (uint32_t i : _updateOrder){
		if (i)
			_systems[i - 1]->update();
	}
}

void Engine::_loadSystems(){
	for (uint32_t i : _updateOrder){
		if (i)
			_systems[i - 1]->load();
	}
}

Engine::Engine() : _manager(new EntityManager()){}

Engine::~Engine(){
	for (BaseSystem* i : _systems)
		delete i;
}

int Engine::run(){
	assert(_systemCount > 0);

	_loadSystems();

	_running = true;

	while (_running)
		_updateSystems();

	return _exit;
}
