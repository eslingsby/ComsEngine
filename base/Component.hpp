#pragma once

#include <cstdint>
#include <cassert>

class EntityManager;

class BaseComponent{
protected:
	static uint32_t _typeCounter;

public:
	virtual ~BaseComponent(){}
};

template <class T>
class Component : public BaseComponent{
public:
	static unsigned int type();
};

template<class T>
unsigned int Component<T>::type(){
	static unsigned int type = _typeCounter++;

	assert(type < 31); // 31 max components
	return type;
}

struct Transform : public Component<Transform>{
	Transform(float x = 0.f, float y = 0.f);
	~Transform();

	float x;
	float y;
};

struct Velocity : public Component<Velocity>{
	Velocity(float gravity = 1.f);
	~Velocity();

	float gravity;

	float dx = 0.f;
	float dy = 0.f;
};