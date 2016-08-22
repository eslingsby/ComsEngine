#include "Engine.hpp"
#include "Engine.hpp"

#include <string>

Engine::Engine(){}

Engine::~Engine(){
	// Delete dynamic memory
	for (auto i : _systems)
		delete i.second;
}

int Engine::run(int argc, char* argv[]){
	// Each step of the startup and running process

	assert(!_running);

	init(argc, argv);

	load();

	start();

	while (_running)
		update();

	return exit();
}