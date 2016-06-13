#include "EntityManager.hpp"

EntityManager::EntityManager(){}

EntityManager::~EntityManager(){
	// Call all component destructers properly

	for (BasePool* pool : _pools){
		if (pool)
			delete pool;
	}
}