#include "CubeTest.hpp"

#include "Engine.hpp"

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Identifier.hpp"

#include "Hierarchy.hpp"

#include <random>

#define OBJECTS 32768

template <typename T>
T randomRange(T min, T max){
	static std::random_device rd;
	static std::mt19937 gen(rd());
	
	std::uniform_int_distribution<> dis(min, max);
	
	return dis(gen);
}

CubeTest::CubeTest(Engine * engine) : System(engine){}

void CubeTest::load(){
	for (unsigned int i = 0; i < OBJECTS; i++){
		uint64_t id = _engine.manager.createEntity();

		_engine.manager.addComponent<Cube>(id);
	}
}

void CubeTest::update(){
	_engine.manager.processEntities(this);
}

void CubeTest::onCreate(uint64_t id){
	_engine.manager.addComponent<Transform>(id);
	_engine.manager.addComponent<Mesh>(id, "mesh/cube.obj");
	_engine.manager.addComponent<Identifier>(id, "", "cube_layer");

	Transform* transform = _engine.manager.getComponent<Transform>(id);

	int distance = 512;
	transform->position = glm::dvec3(randomRange(-distance, distance), randomRange(-distance, distance), -distance);

	Cube* cube = _engine.manager.getComponent<Cube>(id);

	cube->killTime = (randomRange(0.0, 600.0) / 100.0);
	cube->seconds = 0;
	cube->speed = randomRange(1, 1024);

	int spin = 256;
	cube->movement = glm::dvec3(randomRange(-spin, spin), randomRange(-spin, spin), randomRange(-spin, spin));
}

void CubeTest::onProcess(uint64_t id, Cube& cube){
	cube.seconds += _engine.deltaTime<double>();

	_engine.getSystem<Hierarchy>()->localRotate(id, glm::dquat(cube.movement * _engine.deltaTime<double>()));
	_engine.getSystem<Hierarchy>()->translate(id, glm::dvec3(0, 0, cube.speed * _engine.deltaTime<double>()));

	if (cube.seconds > cube.killTime){
		uint64_t entity = _engine.manager.createEntity();

		_engine.manager.addComponent<Cube>(entity);

		_engine.manager.destroyEntity(id);
	}
}
