#pragma once

#include "System.hpp"

class Common : public System<>{

public:
	Common(EntityManager* manager);

	void onLoad() override;
	void onUpdate() override;

	void onProcess(uint64_t id) override;
};