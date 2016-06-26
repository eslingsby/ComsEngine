#include "Scripting.hpp"

#include "Engine.hpp"

#include <iostream>
#include <thread>

Scripting::Scripting(Engine* engine) : System(engine){}

void Scripting::load(){
	//for (unsigned int i = 0; i < 1000; i++){
	//	uint64_t id = _engine.manager.createEntity();
	//
	//	_engine.manager.addComponent<Script>(id, "");
	//}
}

void Scripting::update(){
	//std::cout << _engine.deltaTime() << "\n";
	//system("cls");

	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

	//_engine.manager.processEntities(this);

	//_engine.shutdown();
}

void Scripting::onProcess(uint64_t id, Script& script){
	//std::cout << script.source << "\n";
}

void Scripting::onCreate(uint64_t id){}

void Scripting::onDestroy(uint64_t id){}

void Scripting::onActivate(uint64_t id){}

void Scripting::onDeactivate(uint64_t id){}
