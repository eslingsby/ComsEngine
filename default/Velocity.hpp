#pragma once

#include "Component.hpp"

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>

struct Velocity : public Component<Velocity>{
	glm::vec3 position;
	glm::quat rotation;

	float friction;

	Velocity() :
		position({ 0, 0, 0 }),
		rotation(glm::quat({ 0, 0, 0 })),
		friction(0.5f){
	}
};