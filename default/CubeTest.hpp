#pragma once

#include "System.hpp"

#include "Cube.hpp"

class CubeTest : public System<Cube>{
	
public:
	CubeTest(Engine* engine);

	void load() override;
	void update() override;

	void onCreate(uint64_t id) override;

	void onProcess(uint64_t id, Cube& cube) override;
};