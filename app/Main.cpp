#include "Engine.hpp"

#include "Scripting.hpp"
#include "Identification.hpp"

int main(int argc, char* argv[]){
	Engine engine;

	engine.addSystem<Scripting>(0, 0, Engine::root(argv[0]) + "data");
	engine.addSystem<Identification>(1, 1);

	return engine.run();
}