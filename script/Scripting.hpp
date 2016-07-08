#pragma once

#include "System.hpp"

#include "Script.hpp"

#include <lua.hpp>
#include <string>

class Scripting : public System<Script>{
	lua_State* const _L;

	void _addRequirePath(const std::string& path);

public:
	Scripting(Engine* engine);
	~Scripting();

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Script& script) override;

	void createInstance(uint64_t id, const std::string& type, unsigned int number = 0);
	void destroyInstance(uint64_t id, const std::string& type, unsigned int number = 0);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;
};