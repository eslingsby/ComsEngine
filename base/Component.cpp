#include "Component.hpp"

#ifdef _DEBUG
#include <iostream>
#endif

unsigned int BaseComponent::_typeCounter = 0;

Transform::Transform(float x, float y) : x(x), y(y){}

Transform::~Transform(){
#ifdef _DEBUG
	std::cout << "Transform dead...\n";
#endif
}

Velocity::Velocity(float gravity) : gravity(gravity){}

Velocity::~Velocity(){
#ifdef _DEBUG
	std::cout << "Velocity dead...\n";
#endif
}