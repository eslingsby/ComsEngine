#pragma once

#include "System.hpp"

#include "Script.hpp"

#include <lua.hpp>
#include <string>

class Scripting : public System<Script>{
	lua_State* const _L;

	std::vector<std::string> _componentNames;

	std::string _scriptPath;

public:
	Scripting(Engine* engine, const std::string& scriptPath);
	~Scripting();

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Script& script) override;

	void createInstance(uint64_t id, const std::string& type, unsigned int number = 0);
	void destroyInstance(uint64_t id, const std::string& type, unsigned int number = 0);

	void registerFile(const std::string& type, const std::string& file);

	template <typename T>
	void registerComponent(const std::string& type);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;
};

template <typename T>
inline void Scripting::registerComponent(const std::string& type){
	if (T::type() >= _componentNames.size())
		_componentNames.resize(T::type() + 1);

	_componentNames[T::type()] = type;
}
