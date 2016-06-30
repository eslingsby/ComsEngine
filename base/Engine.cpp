#include "Engine.hpp"

#include <string>

Engine::Engine(){}

Engine::~Engine(){
	for (BaseSystem* i : _systems)
		delete i;
}

int Engine::run(int argc, char* argv[]){
	assert(!_running);

	// Load in _config strings from argv and config file

	if (argc){
		std::string data = argv[0];

		data = data.substr(0, data.find_last_of("/\\"));
		data = data.substr(0, data.find_last_of("/\\") + 1) + "data/\\";

		_config["data"] = data;
	}

	load();

	while (_running)
		update();

	// write _config changes

	return _exitCode;
}