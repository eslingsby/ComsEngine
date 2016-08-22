#pragma once

#include <cstdint>
#include <cassert>
#include <string>

// Use be deriving from a template component.
// Second template argument defines chunk size, IE amount of components that will be made when increasing memory.
// Use a larger chunk size for entities that are created very often, default is 8.
//
//struct MyComponent : public Component<MyComponent, 8>{
//	int memberInt1;
//	int memberInt2;
//	int memberInt3;
//};

class BaseComponent{
protected:
	static uint32_t _typeCounter;

public:
	virtual ~BaseComponent(){}
};

template <class T, uint32_t chunkSize = 8>
class Component : public BaseComponent{
public:
	// Integer representing component type, increments and is unique for each derived class
	static inline uint32_t type();

	// Desired chunk size to increase object pool when expanding, defined by derived class
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