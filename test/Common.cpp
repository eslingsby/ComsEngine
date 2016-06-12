#include "Common.hpp"

#include "EntityManager.hpp"

Common::Common(EntityManager* manager) : System(manager){

}

void Common::onLoad(){

}

void Common::onUpdate(){
	_manager->processEntities(this);
}

void Common::onProcess(uint64_t id){

}
