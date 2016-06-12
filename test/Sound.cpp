#include "Sound.hpp"

#include "EntityManager.hpp"

Sound::Sound(EntityManager* manager) : System(manager){

}

void Sound::onLoad(){

}

void Sound::onUpdate(){
	_manager->processEntities(this);
}

void Sound::onProcess(uint64_t id, Transform* transform, Audio* audio){

}
