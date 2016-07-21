#include "Engine.hpp"

#include "Identification.hpp"
#include "Scripting.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Hierarchy.hpp"

int main(int argc, char* argv[]){
	Engine engine;

	engine.addSystem<Hierarchy>(0, 0);
	engine.addSystem<Identification>(1, 1);
	engine.addSystem<Scripting>(2, 2, Engine::root(argv[0]) + "data");
	engine.addSystem<Physics>(3, 3);
	engine.addSystem<Renderer>(4, 4);
	
	return engine.run();
}