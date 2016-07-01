#pragma once

#include "Component.hpp"

#include <string>
#include <unordered_map>
#include <vector>

struct Script : public Component<Script>{
	std::unordered_map<std::string, std::vector<std::pair<bool, uint32_t>>> references;
};