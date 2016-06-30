#pragma once

#include "Engine.hpp"

class StaticEngine : public Engine{
	StaticEngine();

public:
	inline static StaticEngine& get();
};

inline StaticEngine& StaticEngine::get(){
	static StaticEngine instance;
	return instance;
}
