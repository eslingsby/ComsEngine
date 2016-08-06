#pragma once

#include "Component.hpp"

struct Script : public Component<Script>{
	static const unsigned int maxReferences = 8;
	static const unsigned int maxIdentifier = 32;

	int testingReferences[maxReferences];
	char testingIdentifiers[maxReferences][maxIdentifier];
};