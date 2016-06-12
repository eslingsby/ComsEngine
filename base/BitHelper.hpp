#pragma once

#include <cstdint>

struct BitHelper{
	static inline uint32_t setBit(uint8_t i, bool value, uint32_t bits = 0);

	static inline bool getBit(uint8_t i, uint32_t bits);
	
	static inline uint32_t front(uint64_t i);
	
	static inline uint32_t back(uint64_t i);
	
	static inline uint64_t combine(uint32_t front, uint32_t back);

	template<class ...Ts>
	static inline uint32_t createMask();

private:

	template<uint32_t I>
	static inline void _createMask(uint32_t& mask);

	template<uint32_t I, typename ...Ts>
	struct CreateMask{
		inline void operator()(uint32_t& mask){
			_createMask<I>(mask);
			CreateMask<I - 1, Ts...>{}(mask);
		}
	};

	template<typename ...Ts>
	struct CreateMask<0, Ts...>{
		inline void operator()(uint32_t& mask){
			_createMask<0>(mask);
		}
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

	//if (size)
	//	CreateMask<size - 1, Ts...>{}(mask);

	return mask;
}

template<uint32_t I>
inline void BitHelper::_createMask(uint32_t& mask){
	using T = typename std::tuple_element<I, std::tuple<Ts...>>::type;

	mask = BitHelper::setBit(T::type(), true, mask);
}