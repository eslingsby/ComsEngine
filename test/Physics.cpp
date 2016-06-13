#include "Physics.hpp"

#include "EntityManager.hpp"

#include <iostream>

Physics::Physics(EntityManager* manager) : System(manager){}

void Physics::load(){

}

void Physics::update(){
	_manager->processEntities(this);
}

void Physics::onProcess(uint64_t id, Transform* transform, RigidBody* rigidBody, Collider* collider){
	std::cout << "Physics" << " onProcess " << BitHelper::front(id) << "\n";
}

void Physics::onCreate(uint64_t id){
	std::cout << "Physics" << " onCreate " << BitHelper::front(id) << "\n";
}

void Physics::onDestroy(uint64_t id){
	std::cout << "Physics" << " onDestroy " << BitHelper::front(id) << "\n";
}

void Physics::onActivate(uint64_t id){
	std::cout << "Physics" << " onActivate " << BitHelper::front(id) << "\n";
}

void Physics::onDeactivate(uint64_t id){
	std::cout << "Physics" << " onDeactivate " << BitHelper::front(id) << "\n";
}
