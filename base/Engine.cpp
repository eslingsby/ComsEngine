#include "Engine.hpp"
#include "Engine.hpp"

#include <string>

Engine::Engine(){}

Engine::~Engine(){
	for (auto i : _systems)
		delete i.second;
}

int Engine::run(int argc, char* argv[]){
	assert(!_running);

	init(argc, argv);

	load();

	while (_running)
		update();

	return exit();
}