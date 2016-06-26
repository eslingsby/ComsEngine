#include "Engine.hpp"

Engine::Engine(){}

Engine::~Engine(){
	for (BaseSystem* i : _systems)
		delete i;
}

int Engine::run(int argc, char* argv[]){
	assert(!_running);

	// Load in _config strings from argv and config file

	load();

	while (_running)
		update();

	// write _config changes

	return _exitCode;
}