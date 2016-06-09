#include "EntityManager.hpp"
#include "TestObjects.hpp"

#include <iostream>

int main(int argc, char* argv[]){
	EntityManager manager;
	
	uint64_t id = manager.createEntity();
	
	manager.addComponent<Transform>(id, 123.f, 456.f);
	manager.addComponent<Velocity>(id, 100.f);
	
	//manager.setEntityActive(id, false);

	id = manager.createEntity();

	manager.addComponent<Transform>(id, 789.f, 101.f);
	manager.addComponent<Velocity>(id, 140.f);

	//manager.destroyEntity(id);

	Physics system(&manager);

	system.update();	

	std::cin.get();

	return 0;
}