#pragma once

#include "Component.hpp"

#include <string>

struct Script : public Component<Script>{
	Script(std::string source) : source(source){}

	const std::string source;
	
	bool loaded = false;
};