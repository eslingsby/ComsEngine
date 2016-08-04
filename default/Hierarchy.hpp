#pragma once

#include "System.hpp"

#include "Transform.hpp"

#include <set>
#include <unordered_map>

class Hierarchy : public System<Transform>{
	std::unordered_map<uint64_t, std::set<uint64_t>> _children;

public:
	Hierarchy(Engine* engine);

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void translate(uint64_t id, glm::vec3 position);
	void localTranslate(uint64_t id, glm::vec3 position);
	void rotate(uint64_t id, glm::quat rotation);
	void localRotate(uint64_t id, glm::quat rotation);

	void lookAt(uint64_t id, glm::vec3 target, glm::vec3 up);

	// Global transform data recursively calculated from transfrom parents
	glm::vec3 globalPosition(uint64_t id);
	glm::quat globalRotation(uint64_t id);
	glm::vec3 globalScale(uint64_t id);
};