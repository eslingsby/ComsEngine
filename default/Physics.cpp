#include "Physics.hpp"

#include "Engine.hpp"

Physics::Physics(Engine* engine) : System(engine){}

void Physics::load(){}

void Physics::update(){
	_engine.manager.processEntities(this);
}

void Physics::onProcess(uint64_t id, Transform& transform, Velocity& velocity){
	//transform.position = glm::mix(transform.position, velocity.position, _engine.deltaTime());
	//transform.rotation = glm::mix(transform.rotation, velocity.rotation, _engine.deltaTime());

	//velocity.position /= 2 * _engine.deltaTime();
	//velocity.rotation /= 2 * _engine.deltaTime();
}

void Physics::impulse(uint64_t id, glm::vec3 angle, float force){

}
