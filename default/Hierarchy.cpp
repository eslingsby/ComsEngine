#include "Hierarchy.hpp"

#include "Engine.hpp"

Hierarchy::Hierarchy(Engine* engine) : System(engine){}

void Hierarchy::onCreate(uint64_t id){
	Transform* transform = _engine.manager.getComponent<Transform>(id);

	if (!transform->parent)
		return;

	_children[transform->parent].insert(id);

	_engine.manager.addReference(id);
}

void Hierarchy::onDestroy(uint64_t id){
	Transform* transform = _engine.manager.getComponent<Transform>(id);

	if (!transform->parent)
		return;

	_children[transform->parent].erase(id);

	if (_children.find(id) == _children.end())
		return;

	std::queue<uint64_t> toErase;

	for (uint64_t child : _children[id]){
		transform = _engine.manager.getComponent<Transform>(child);

		transform->position = globalPosition(child);
		transform->rotation = globalRotation(child);
		transform->scale = globalScale(child);

		_engine.manager.addComponent<Transform>(child, 0, _engine.manager.getComponent<Transform>(child));
		_engine.manager.removeReference(child);

		toErase.push(child);
	}

	while (toErase.size()){
		_children[id].erase(toErase.front());
		toErase.pop();
	}

	if (!_children[id].size())
		_children.erase(id);
}

void Hierarchy::translate(uint64_t id, glm::vec3 position){
	Transform* transfrom = _engine.manager.getComponent<Transform>(id);
	assert(transfrom);

	transfrom->position += position;
}

void Hierarchy::localTranslate(uint64_t id, glm::vec3 position){
	Transform* transfrom = _engine.manager.getComponent<Transform>(id);
	assert(transfrom);

	transfrom->position += position * transfrom->rotation;
}

void Hierarchy::rotate(uint64_t id, glm::quat rotation){
	Transform* transfrom = _engine.manager.getComponent<Transform>(id);
	assert(transfrom);

	transfrom->rotation = rotation * transfrom->rotation;
}

void Hierarchy::localRotate(uint64_t id, glm::quat rotation){
	Transform* transfrom = _engine.manager.getComponent<Transform>(id);
	assert(transfrom);

	transfrom->rotation *= rotation;
}

glm::vec3 Hierarchy::globalPosition(uint64_t id){
	Transform* transform = _engine.manager.getComponent<Transform>(id);

	glm::vec3 position = transform->position;
	
	//while (transform->parent){
	//	transform = _engine.manager.getComponent<Transform>(transform->parent);
	//	position += transform->position;
	//}

	return position;
}

glm::quat Hierarchy::globalRotation(uint64_t id){
	Transform* transform = _engine.manager.getComponent<Transform>(id);

	glm::quat rotation = transform->rotation;

	//while (transform->parent){
	//	transform = _engine.manager.getComponent<Transform>(transform->parent);
	//	rotation *= transform->rotation;
	//}

	return rotation;
}

glm::vec3 Hierarchy::globalScale(uint64_t id){
	Transform* transform = _engine.manager.getComponent<Transform>(id);

	glm::vec3 scale = transform->scale;

	//while (transform->parent){
	//	transform = _engine.manager.getComponent<Transform>(transform->parent);
	//	scale *= transform->scale;
	//}

	return scale;
}
