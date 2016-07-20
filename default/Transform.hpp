#pragma once

#include "Component.hpp"

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>

struct Transform : public Component<Transform>{
	const uint64_t parent;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1, 1, 1);

	Transform(uint64_t parent = 0) : parent(parent){}

	Transform(uint64_t parent, const Transform* other) : parent(parent){
		position = other->position;
		rotation = other->rotation;
		scale = other->scale;
	}
};