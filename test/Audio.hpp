#pragma once

#include "Component.hpp"

#include <string>

struct Audio : public Component<Audio>{
	Audio(const std::string& source) : source(source){}

	const std::string source;

	float volume = 1.f;

	bool loaded = false;
};