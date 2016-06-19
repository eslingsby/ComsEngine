#pragma once

#include "Component.hpp"

template <size_t bytes = 1>
struct TestComponent : public Component<TestComponent<bytes>>{
	double begin;
	
	char memory[bytes];
	
	double end;
};