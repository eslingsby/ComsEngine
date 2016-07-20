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

	glm::vec3 globalPosition(uint64_t id);
	glm::quat globalRotation(uint64_t id);
	glm::vec3 globalScale(uint64_t id);
};