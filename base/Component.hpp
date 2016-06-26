#pragma once

#include <cstdint>
#include <cassert>

class BaseComponent{
protected:
	static uint32_t _typeCounter;

public:
	virtual ~BaseComponent(){}
};

template <class T, uint32_t chunkSize = 8>
class Component : public BaseComponent{
public:
	static inline uint32_t type();
	static inline uint32_t chunk();
};

template <class T, uint32_t chunkSize>
inline unsigned int Component<T, chunkSize>::type(){
	static unsigned int type = _typeCounter++;

	assert(type < 32);
	return type;
}

template<class T, uint32_t chunkSize>
inline uint32_t Component<T, chunkSize>::chunk(){
	return chunkSize;
}