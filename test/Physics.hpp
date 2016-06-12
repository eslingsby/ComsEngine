#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"

class Physics : public System<Transform, RigidBody, Collider>{

public:
	Physics(EntityManager* manager);

	void onLoad() override;
	void onUpdate() override;

	void onProcess(uint64_t id, Transform* transform, RigidBody* rigidBody, Collider* collider) override;
};