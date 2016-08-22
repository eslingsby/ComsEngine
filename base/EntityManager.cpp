#include "EntityManager.hpp"

EntityManager::EntityManager(){}

EntityManager::~EntityManager(){
	// Delete dynamic memory
	for (BasePool* pool : _pools){
		if (pool)
			delete pool;
	}
}