#include "Renderer.hpp"

#include "EntityManager.hpp"

Renderer::Renderer(EntityManager* manager) : System(manager){

}

void Renderer::onLoad(){

}

void Renderer::onUpdate(){
	_manager->processEntities(this);
}

void Renderer::onProcess(uint64_t id, Transform* transform, Mesh* mesh, Material* material){

}
