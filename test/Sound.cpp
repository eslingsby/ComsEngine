#include "Sound.hpp"

#include "EntityManager.hpp"

#include <iostream>

Sound::Sound(EntityManager* manager) : System(manager){}

void Sound::load(){

}

void Sound::update(){
	_manager->processEntities(this);
}

void Sound::onProcess(uint64_t id, Transform* transform, Audio* audio){
	std::cout << "Sound" << " onProcess " << BitHelper::front(id) << "\n";
}

void Sound::onCreate(uint64_t id){
	std::cout << "Sound" << " onCreate " << BitHelper::front(id) << "\n";
}

void Sound::onDestroy(uint64_t id){
	std::cout << "Sound" << " onDestroy " << BitHelper::front(id) << "\n";
}

void Sound::onActivate(uint64_t id){
	std::cout << "Sound" << " onActivate " << BitHelper::front(id) << "\n";
}

void Sound::onDeactivate(uint64_t id){
	std::cout << "Sound" << " onDeactivate " << BitHelper::front(id) << "\n";
}
