#include "EntityManager.hpp"

EntityManager::EntityManager(){}

EntityManager::~EntityManager(){
	for (BasePool* pool : _pools){
		if (pool)
			delete pool;
	}
}