#include "Renderer.hpp"

#include "EntityManager.hpp"

#include <iostream>

Renderer::Renderer(EntityManager* manager) : System(manager){}

void Renderer::load(){
	uint64_t id = _manager->createEntity();

	_manager->addComponent<Material>(id, "test");
	_manager->addComponent<Mesh>(id, "test");
	_manager->addComponent<Transform>(id);

	_manager->destroyEntity(id);
}

void Renderer::update(){
	_manager->processEntities(this);
}

void Renderer::onProcess(uint64_t id, Transform* transform, Mesh* mesh, Material* material){
	std::cout << "Renderer" << " onProcess " << BitHelper::front(id) << "\n";
}

void Renderer::onCreate(uint64_t id){
	std::cout << "Renderer" << " onCreate " << BitHelper::front(id) << "\n";
}

void Renderer::onDestroy(uint64_t id){
	std::cout << "Renderer" << " onDestroy " << BitHelper::front(id) << "\n";
}

void Renderer::onActivate(uint64_t id){
	std::cout << "Renderer" << " onActivate " << BitHelper::front(id) << "\n";
}

void Renderer::onDeactivate(uint64_t id){
	std::cout << "Renderer" << " onDeactivate " << BitHelper::front(id) << "\n";
}
