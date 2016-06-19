#include "Scripting.hpp"

#include "Engine.hpp"

#include <iostream>

Scripting::Scripting(EntityManager* manager) : System(manager){}

void Scripting::load(Engine& engine){
	staticScript = Entity(_manager);

	staticScript.create();

	staticScript.addComponent<Script>("load.lua");
}

void Scripting::update(Engine& engine){
	_manager->processEntities(this);
}

void Scripting::onProcess(uint64_t id, Script& script){
	
}

void Scripting::onCreate(uint64_t id){}

void Scripting::onDestroy(uint64_t id){}

void Scripting::onActivate(uint64_t id){}

void Scripting::onDeactivate(uint64_t id){}
