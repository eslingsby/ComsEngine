#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Velocity.hpp"

class Physics : public System<Transform, Velocity>{

public:
	Physics(Engine* engine);

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Transform& transform, Velocity& velocity) override;


	// Apply force to entity with velocity component
	void impulse(uint64_t id, glm::vec3 angle, float force = 1.f);
};