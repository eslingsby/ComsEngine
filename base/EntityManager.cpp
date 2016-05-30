#include "EntityManager.hpp"

EntityManager::EntityManager(){

}

EntityManager::~EntityManager(){
	for (ObjectPool* pool : _pools){
		if (pool)
			delete pool;
	}
}
