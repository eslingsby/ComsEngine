#pragma once

#include <cstdint>

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
	return type;
}