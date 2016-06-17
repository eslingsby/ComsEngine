#pragma once

#include "System.hpp"

#include "Script.hpp"

class Scripting : public System<Script>{

public:
	Scripting(EntityManager* manager);

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Script& script) override;

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};