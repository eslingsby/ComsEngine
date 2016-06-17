#include "Engine.hpp"

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
