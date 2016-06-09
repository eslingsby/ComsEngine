#pragma once

#include <cstdint>

struct BitHelper{
	static inline uint32_t setBit(uint8_t i, bool value, uint32_t bits = 0);

	static inline bool getBit(uint8_t i, uint32_t bits);
	
	static inline uint32_t front(uint64_t i);
	
	static inline uint32_t back(uint64_t i);
	
	static inline uint64_t combine(uint32_t front, uint32_t back);
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

inline uint32_t BitHelper::back(uint64_t i){
	return (uint32_t)i;
}

inline uint32_t BitHelper::front(uint64_t i){
	return (uint32_t)(i >> 32);
}

inline uint64_t BitHelper::combine(uint32_t front, uint32_t back){
	return back + ((uint64_t)front << 32);
}