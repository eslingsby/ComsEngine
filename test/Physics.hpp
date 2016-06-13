#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"

class Physics : public System<Transform, RigidBody, Collider>{

public:
	Physics(EntityManager* manager);

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Transform* transform, RigidBody* rigidBody, Collider* collider) override;

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};