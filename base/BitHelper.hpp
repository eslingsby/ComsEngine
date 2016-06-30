#pragma once

#include <cstdint>
#include <tuple>

struct BitHelper{
	// Set bit at location I in 32 bit value
	static inline uint32_t setBit(uint8_t i, bool value, uint32_t bits = 0);

	// Get bit at location I in 32 bit value
	static inline bool getBit(uint8_t i, uint32_t bits);
	
	// Get first 32 bits in 64 bit (represents entity index)
	static inline uint32_t front(uint64_t i);
	
	// back first 32 bits in 64 bit (represents entity version)
	static inline uint32_t back(uint64_t i);
	
	// Combine two 32 bit values to 64 bit (entity index + entity version)
	static inline uint64_t combine(uint32_t front, uint32_t back);

	// Create 32 bit mask representing components
	template<class ...Ts>
	static inline uint32_t createMask();

private:

	// Recursive compile time functions for iterating over components in createMask
	template<uint32_t I, typename ...Ts>
	static inline void _createMask(uint32_t& mask);

	// Body of recursive compile time function
	template<uint32_t I, typename ...Ts>
	struct CreateMask{
		inline void operator()(uint32_t& mask){
			_createMask<I, Ts...>(mask);
			CreateMask<I - 1, Ts...>{}(mask);
		}
	};

	// Bottom of recursive compile time function
	template<typename ...Ts>
	struct CreateMask<0, Ts...>{
		inline void operator()(uint32_t& mask){
			_createMask<0, Ts...>(mask);
		}
	};

	// Bottom of recursive compile time function if createMask left empty
	template<typename ...Ts>
	struct CreateMask<-1, Ts...>{
		inline void operator()(uint32_t& mask){}
	};
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

template<class ...Ts>
inline uint32_t BitHelper::createMask(){
	uint32_t mask = 0;

	const uint32_t size = std::tuple_size<std::tuple<Ts...>>::value;

	CreateMask<size - 1, Ts...>{}(mask);

	return mask;
}

template<uint32_t I, typename ...Ts>
inline void BitHelper::_createMask(uint32_t& mask){
	using T = typename std::tuple_element<I, std::tuple<Ts...>>::type;

	mask = BitHelper::setBit(T::type(), true, mask);
}