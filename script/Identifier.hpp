#pragma once

#include "Component.hpp"

#include <string>

struct Identifier : public Component<Identifier>{
	Identifier(const std::string& name = "", const std::string& layer = "") : name(name), layer(layer){}

	const std::string name;
	const std::string layer;

	bool validName = false;
};