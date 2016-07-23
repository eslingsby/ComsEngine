#include "Engine.hpp"

#include "Identification.hpp"
#include "Scripting.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Hierarchy.hpp"
#include "Input.hpp"

int main(int argc, char* argv[]){
	Engine engine;

	engine.addSystem<Hierarchy>(0, 0);
	engine.addSystem<Identification>(1, 1);
	engine.addSystem<Input>(2, 2);
	engine.addSystem<Scripting>(3, 3);
	engine.addSystem<Physics>(4, 4);
	engine.addSystem<Renderer>(5, 5);
	
	return engine.run(argc, argv);
}