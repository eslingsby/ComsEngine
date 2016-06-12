#include "Physics.hpp"

#include "EntityManager.hpp"

Physics::Physics(EntityManager* manager) : System(manager){

}

void Physics::onLoad(){

}

void Physics::onUpdate(){
	_manager->processEntities(this);
}

void Physics::onProcess(uint64_t id, Transform* transform, RigidBody* rigidBody, Collider* collider){

}
