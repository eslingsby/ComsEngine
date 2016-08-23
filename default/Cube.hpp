#pragma once

#include "Component.hpp"

#include <glm\vec3.hpp>

struct Cube : public Component<Cube>{
	double killTime;
	double seconds;
	double speed;
	glm::dvec3 movement;
};