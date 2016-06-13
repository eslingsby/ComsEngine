#pragma once

#include "System.hpp"

#include "Transform.hpp"

class Common : public System<>{

public:
	Common(EntityManager* manager);

	void load() override;
	void update() override;

	void onProcess(uint64_t id) override;

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};