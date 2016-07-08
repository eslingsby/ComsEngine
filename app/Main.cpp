#include "Engine.hpp"

#include "Scripting.hpp"
#include "Identification.hpp"

int main(int argc, char* argv[]){
	Engine engine;

	engine.addSystem<Scripting>();
	engine.addSystem<Identification>();

	return engine.run(argc, argv);
}