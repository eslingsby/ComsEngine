#include "Engine.hpp"
#include "Engine.hpp"

#include <string>

Engine::Engine(){}

Engine::~Engine(){
	for (auto i : _systems)
		delete i.second;
}

int Engine::run(){
	assert(!_running);

	// Load in _config strings from argv and config file

	load();

	while (_running)
		update();

	// write _config changes

	return _exitCode;
}

std::string Engine::root(std::string exePath){
	exePath = exePath.substr(0, exePath.find_last_of("\\"));
	exePath = exePath.substr(0, exePath.find_last_of("\\") + 1);

	return exePath;
}
