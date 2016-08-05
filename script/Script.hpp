#pragma once

#include "Component.hpp"

//#include <string>
//#include <unordered_map>
//#include <vector>

struct Script : public Component<Script>{
	//typedef std::pair<bool, uint32_t> RefPair;
	//typedef std::vector<RefPair> RefVec;
	//typedef std::unordered_map<std::string, RefVec> RefMap;
	//
	//RefMap* references = nullptr;

	static const unsigned int maxReferences = 8;
	static const unsigned int maxIdentifier = 32;

	int testingReferences[maxReferences];
	char testingIdentifiers[maxReferences][maxIdentifier];
};