#pragma once

#include "Component.hpp"
#include "System.hpp"

struct Transform : public Component<Transform>{
	Transform(float x = 0.f, float y = 0.f);
	~Transform();

	float x;
	float y;
};

struct Velocity : public Component<Velocity>{
	Velocity(float gravity = 1.f);
	~Velocity();

	float gravity;

	float dx = 0.f;
	float dy = 0.f;
};

class Physics : System<Transform, Velocity>{
	uint32_t _counter;

public:
	Physics(EntityManager* manager);

	void update() override;
	void operator()(uint64_t id, Transform* transform, Velocity* velocity) override;
};