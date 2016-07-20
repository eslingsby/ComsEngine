#include "Renderer.hpp"

#include "Engine.hpp"

Renderer::Renderer(Engine* engine) : System(engine){}

void Renderer::load(){
	// Initiate SDL
}

void Renderer::update(){
	_engine.manager.processEntities(this);
}

void Renderer::onProcess(uint64_t id, Transform& script){
	// Render squares for each Transform
}
