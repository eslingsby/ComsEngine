#pragma once

#include "Component.hpp"

#include <cstdint>

template <size_t bytes = 1>
struct TestComponent : public Component<TestComponent<bytes>>{
	uint32_t begin = 0;

	uint8_t memory[bytes];

	uint32_t end = 0;
};

typedef TestComponent<> TinyComponent;
typedef TestComponent<128 - 64> SmallComponent;
typedef TestComponent<512 - 64> MediumComponent;
typedef TestComponent<1024 - 64> LargeComponent;
typedef TestComponent<4096 - 64> HugeComponent;