#include "System.hpp"

#include "EntityManager.hpp"

BaseSystem::BaseSystem(EntityManager* manager, uint32_t mask) : _manager(manager), mask(mask){
	assert(manager != nullptr);

	// Add self to manager

	//std::tuple<Ts...> ringo;
}

BaseSystem::~BaseSystem(){

	// Remove self from manager
}

unsigned int BaseSystem::_typeCounter = 0;