#include "Engine.hpp"

#include "Scripting.hpp"

int main(int argc, char* argv[]){
	Engine engine;

	engine.addSystem<Scripting>();

	return engine.run(argc, argv);
}