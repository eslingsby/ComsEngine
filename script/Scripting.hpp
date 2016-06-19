#pragma once

#include "System.hpp"
#include "Entity.hpp"

#include "Script.hpp"

class Scripting : public System<Script>{
	Entity staticScript;

public:
	Scripting(EntityManager* manager);

	void load(Engine& engine) override;
	void update(Engine& engine) override;

	void onProcess(uint64_t id, Script& script) override;

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};