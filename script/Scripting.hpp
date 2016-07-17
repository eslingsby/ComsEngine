#pragma once

#include "System.hpp"

#include "Script.hpp"

#include <lua.hpp>
#include <string>
#include <set>

class Scripting : public System<Script>{
	lua_State* const _L;

	std::set<std::string> _loadedScripts;

	std::string _scriptPath;

	bool _ctrlDown = false;
	bool _reloaded = false;

public:
	Scripting(Engine* engine, const std::string& scriptPath);
	~Scripting();

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Script& script) override;

	void createInstance(uint64_t id, const std::string& type, unsigned int number = 0);
	void destroyInstance(uint64_t id, const std::string& type, unsigned int number = 0);

	void registerFile(const std::string& type, const std::string& file);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;
};