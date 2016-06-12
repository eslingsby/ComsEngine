#include "Engine.hpp"

#include "Physics.hpp"
#include "Sound.hpp"
#include "Renderer.hpp"

int main(int argc, char* argv[]){
	Engine engine;

	engine.addSystem<Physics>();
	engine.addSystem<Renderer>();
	engine.addSystem<Sound>();

	return engine.run();
}