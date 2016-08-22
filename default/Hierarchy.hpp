#pragma once

#include "System.hpp"

#include "Transform.hpp"

#include <set>
#include <unordered_map>

// Renderer system. Subsribes to entities with Transform components.
// Used to perform positional transformations.

// Will eventually be used to parent entities to each other.

class Hierarchy : public System<Transform>{
	std::unordered_map<uint64_t, std::set<uint64_t>> _children;

public:
	Hierarchy(Engine* engine);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	// Translate transform on global axes
	void translate(uint64_t id, glm::vec3 position);
	// Translate transform on transform's rotation axes
	void localTranslate(uint64_t id, glm::vec3 position);
	// Rotate transform on global axes
	void rotate(uint64_t id, glm::quat rotation);
	// Rotate  transform on transform's rotation axes
	void localRotate(uint64_t id, glm::quat rotation);

	// Rotate transform to look at other position
	void lookAt(uint64_t id, glm::vec3 target, glm::vec3 up);

	// Global transform data recursively calculated from transfrom parents (not working)
	glm::vec3 globalPosition(uint64_t id);
	glm::quat globalRotation(uint64_t id);
	glm::vec3 globalScale(uint64_t id);
};