#include "Common.hpp"

#include "EntityManager.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

#include <iostream>

Common::Common(EntityManager* manager) : System(manager){}

void Common::load(){
	uint64_t id = _manager->createEntity();

	_manager->addComponent<Material>(id, "test");
	_manager->addComponent<Mesh>(id, "test");
	_manager->addComponent<Transform>(id);
}

void Common::update(){
	_manager->processEntities(this);
}

void Common::onProcess(uint64_t id){
	std::cout << "Common" << " onProcess " << BitHelper::front(id) << "\n";
}

void Common::onCreate(uint64_t id){
	std::cout << "Common" << " onCreate " << BitHelper::front(id) << "\n";
}

void Common::onDestroy(uint64_t id){
	std::cout << "Common" << " onDestroy " << BitHelper::front(id) << "\n";
}

void Common::onActivate(uint64_t id){
	std::cout << "Common" << " onActivate " << BitHelper::front(id) << "\n";
}

void Common::onDeactivate(uint64_t id){
	std::cout << "Common" << " onDeactivate " << BitHelper::front(id) << "\n";
}
