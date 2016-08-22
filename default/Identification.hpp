#pragma once

#include "System.hpp"

#include "Identifier.hpp"

#include <set>
#include <unordered_map>

// Renderer system. Subsribes to entities with Identifier components.
// Used to name components by uniqe names, or layer names.
// Entities can be obtained through this system via their unique name, or their layer name.

class Identification : public System<Identifier>{
	std::unordered_map<std::string, uint64_t> _names;
	std::unordered_map<std::string, std::set<uint64_t>> _layers;
	
public:
	Identification(Engine* engine);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;


	// Get entity ID by name or layer string
	uint64_t getByName(const std::string& name);
	uint64_t getByLayer(const std::string& layer, size_t number = 0);

	// Returns true if name is currently bound
	bool hasName(const std::string& name);

	// Returns how many entities are bound to layer
	size_t hasLayer(const std::string& layer);

	// Destroy single entity by name
	void destroyByName(const std::string& name);

	// Destroy all entities by layer
	void destroyByLayer(const std::string& layer);

	// Rename an entity
	void rename(uint64_t id, const std::string& name, const std::string& layer);
};