#pragma once

class BaseComponent{
protected:
	static unsigned int _typeCounter;

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