#include "System.hpp"

#include "EntityManager.hpp"

BaseSystem::BaseSystem(EntityManager* manager, uint32_t mask) : _manager(manager), mask(mask){
	assert(manager != nullptr);
}

BaseSystem::~BaseSystem(){}

unsigned int BaseSystem::_typeCounter = 0;