#pragma once

#include "Component.hpp"

struct Script : public Component<Script>{
	static const unsigned int maxReferences = 8;
	static const unsigned int maxIdentifier = 32;

	int references[maxReferences];
	char identifiers[maxReferences][maxIdentifier];
};