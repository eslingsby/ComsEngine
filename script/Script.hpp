#pragma once

#include "Component.hpp"

#include <string>
#include <set>
#include <unordered_map>

struct Script : public Component<Script>{
	std::unordered_map<std::string, std::set<int>> references;
};