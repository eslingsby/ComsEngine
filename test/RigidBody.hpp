#pragma once

#include "Component.hpp"

struct RigidBody : public Component<RigidBody>{
	RigidBody(float weight = 1.f, float friction = 1.f, float bounciness = 0.5f) : weight(weight), friction(friction), bounciness(bounciness){}

	float weight;
	float friction;
	float bounciness;

	float gravity = 1.f;

	struct Velocity{
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	} velocity;

	struct Mass{
		float x = 0.5f;
		float y = 0.5f;
		float z = 0.5f;
	} mass;
};