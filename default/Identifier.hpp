#pragma once

#include "Component.hpp"

#include <string>

struct Identifier : public Component<Identifier>{
	Identifier(const std::string& name = "", const std::string& layer = ""){
		strcpy_s(Identifier::name, name.c_str());
		strcpy_s(Identifier::layer, layer.c_str());
	}

	char name[32];
	char layer[32];

	bool validName = false;
};