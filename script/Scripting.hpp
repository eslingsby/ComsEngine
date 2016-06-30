#pragma once

#include "System.hpp"
#include "Entity.hpp"

#include "Script.hpp"

#include <lua.hpp>
#include <string>

class Scripting : public System<Script>{
	lua_State* const L;

public:
	Scripting(Engine* engine);
	~Scripting();

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Script& script) override;

	void registerFile(const std::string& file);

	void createInstance(uint64_t id, Script& script, const std::string& meta);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};