#include "Engine.hpp"

Engine::Engine() : _manager(new EntityManager()){}

Engine::~Engine(){
	for (BaseSystem* i : _systems)
		delete i;
}

int Engine::run(int argc, char* argv[]){
	assert(!_running && _systemCount > 0);

	// Load in _config strings from argv and config file

	_loadSystems();

	_running = true;

	while (_running){
		_updateSystems();
		
		// check for shutdown or restart
	}

	// write _config changes

	return _exitCode;
}
