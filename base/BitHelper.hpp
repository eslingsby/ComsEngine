#pragma once

#include <cstdint>

struct BitHelper{
	static inline uint32_t setBit(uint8_t i, bool value, uint32_t bits = 0);

	static inline bool getBit(uint8_t i, uint32_t bits);
};

inline uint32_t BitHelper::setBit(uint8_t i, bool value, uint32_t bits){
	if (value)
		return bits | (1 << i);
	else
		return bits | (0 << i);
}

inline bool BitHelper::getBit(uint8_t i, uint32_t bits){
	if ((1 << i) & bits)
		return true;

	return false;
}