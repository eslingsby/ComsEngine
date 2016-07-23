#pragma once

#include "System.hpp"

#include "Identifier.hpp"

#include <set>
#include <unordered_map>

class Identification : public System<Identifier>{
	std::unordered_map<std::string, uint64_t> _names;
	std::unordered_map<std::string, std::set<uint64_t>> _layers;
	
public:
	Identification(Engine* engine);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	uint64_t getByName(const std::string& name);
	uint64_t getByLayer(const std::string& layer, size_t number = 0);

	bool hasName(const std::string& name);
	size_t hasLayer(const std::string& layer);

	void destroyByName(const std::string& name);
	void destroyByLayer(const std::string& layer);
};