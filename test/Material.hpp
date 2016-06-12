#pragma once

#include "Component.hpp"

#include <string>

struct Material : public Component<Material>{
	Material(const std::string& source) : source(source){}

	const std::string source;

	unsigned int materialId;

	unsigned int height;
	unsigned int width;

	struct Origin{
		unsigned int x = 0;
		unsigned int y = 0;
	} origin;

	bool loaded = false;
};