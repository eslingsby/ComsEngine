#include "Engine.hpp"

#include "Identification.hpp"
#include "Scripting.hpp"
#include "Renderer.hpp"
#include "Hierarchy.hpp"
#include "Input.hpp"

struct Game : public Engine{
	void start() override{
		getSystem<Scripting>()->callFile("Start.lua");
	}
};

int main(int argc, char* argv[]){
	Game game;

	game.addSystem<Hierarchy>(0, 0);
	game.addSystem<Identification>(1, 1);
	game.addSystem<Input>(2, 2);
	game.addSystem<Scripting>(3, 3);
	game.addSystem<Renderer>(4, 4);
	
	return game.run(argc, argv);
}