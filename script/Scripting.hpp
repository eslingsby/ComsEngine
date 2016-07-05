#pragma once

#include "System.hpp"
#include "Entity.hpp"

#include "Script.hpp"

#include <lua.hpp>
#include <string>

class Scripting : public System<Script>{
	void _unreference(uint32_t reference);

public:
	lua_State* const L;

	Scripting(Engine* engine);
	~Scripting();

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Script& script) override;

	void registerFile(const std::string& file);

	void createInstance(uint64_t id, const std::string& type, unsigned int number = 0);
	void destroyInstance(uint64_t id, const std::string& type, unsigned int number = 0);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;
};