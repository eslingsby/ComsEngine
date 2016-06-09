#include "EntityManager.hpp"

EntityManager::EntityManager(){}

EntityManager::~EntityManager(){
	// Call all component destructers properly

	for (ObjectPool* pool : _pools){
		if (pool)
			delete pool;
	}
}
