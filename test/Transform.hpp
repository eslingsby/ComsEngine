#pragma once

#include "Component.hpp"

struct Transform : public Component<Transform>{
	Transform(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z){}

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	struct Rotation{
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	} rotation;

	struct Scale{
		float x = 1.f;
		float y = 1.f;
		float z = 1.f;
	} scale;
};