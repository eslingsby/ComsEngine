#include "TestObjects.hpp"

#include "EntityManager.hpp"

#include <iostream>

Transform::Transform(float x, float y) : x(x), y(y){}

Transform::~Transform(){
	std::cout << "Transform dead...\n";
}

Velocity::Velocity(float gravity) : gravity(gravity){}

Velocity::~Velocity(){
	std::cout << "Velocity dead...\n";
}

Physics::Physics(EntityManager* manager) : System(manager){}

void Physics::update(){
	_manager->processEntities(this);
}

void Physics::operator()(uint64_t id, Transform* transform, Velocity* velocity){
	std::cout << BitHelper::front(id) << " - " << transform->x << ", " << transform->y << " - " << velocity->gravity << "\n";
}