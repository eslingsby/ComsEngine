#include "EntityManager.hpp"
#include "Test.hpp"

#include <iostream>

int main(int argc, char* argv[]){
	EntityManager manager;
	
	uint32_t id = manager.createEntity();
	
	manager.addComponent<Transform>(id, 123.f, 456.f);
	manager.addComponent<Velocity>(id, 100.f);

	//manager.getComponent<Transform>(id)->x = 0.f;
	//manager.getComponent<Transform>(id)->y = 0.f;

	//std::cout << "\nDeleting entity " << id << "!\n\n";

	//manager.destroyEntity(id);

	
	Physics system(&manager);

	system.update();
	

	std::cin.get();

	return 0;
}