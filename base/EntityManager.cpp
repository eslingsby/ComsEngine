#include "EntityManager.hpp"

EntityManager::EntityManager(){}

EntityManager::~EntityManager(){
	// Systems should already be dead by this point

	for (uint32_t i = 0; i < _entities; i++){
		if (_states[i]){
			for (BasePool* pool : _pools){
				pool->get<BaseComponent>(i)->~BaseComponent();
			}
		}
	}

	for (BasePool* pool : _pools){
		if (pool)
			delete pool;
	}
}