#include "Engine.hpp"

#include "Identification.hpp"
#include "Scripting.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Hierarchy.hpp"
#include "Input.hpp"

struct Game : public Engine{
	void start() override{
		uint64_t id = manager.createEntity();
		manager.addComponent<Script>(id);
		getSystem<Scripting>()->createInstance(id, "Camera");

		getSystem<Renderer>()->setCamera(id);

		EntityRef entity(manager);

		entity.create();

		entity.addComponent<Script>();
		getSystem<Scripting>()->createInstance(entity.id(), "Single");

		for (unsigned int i = 0; i < 1024 * 4; i++){
			id = manager.createEntity();

			manager.addComponent<Script>(id);
			getSystem<Scripting>()->createInstance(id, "Many");
		}
	}
};

int main(int argc, char* argv[]){
	Game game;

	game.addSystem<Hierarchy>(0, 0);
	game.addSystem<Identification>(1, 1);
	game.addSystem<Input>(2, 2);
	game.addSystem<Scripting>(3, 3);
	game.addSystem<Physics>(4, 4);
	game.addSystem<Renderer>(5, 5);
	
	return game.run(argc, argv);
}