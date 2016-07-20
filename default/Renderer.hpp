#pragma once

#include "System.hpp"

#include "Transform.hpp"

class Renderer : public System<Transform>{

public:
	Renderer(Engine* engine);

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Transform& script) override;
};