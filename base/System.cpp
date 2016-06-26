#include "System.hpp"

#include "EntityManager.hpp"

BaseSystem::BaseSystem(Engine* const engine, uint32_t mask) : _engine(*engine), mask(mask){
	assert(engine != nullptr);
}

BaseSystem::~BaseSystem(){}

unsigned int BaseSystem::_typeCounter = 0;