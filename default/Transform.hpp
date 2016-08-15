#pragma once

#include "Component.hpp"

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>

struct Transform : public Component<Transform>{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1, 1, 1);

	Transform(glm::vec3 position = glm::vec3(0, 0, 0), glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1, 1, 1)) :
		position(position),
		rotation(rotation),
		scale(scale){
	}
};