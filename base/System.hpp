#pragma once

#include "BitHelper.hpp"

#include <cassert>

/*
- Change System from being fixed to one Component filter
- Remove templatized Component mask
*/

class EntityManager;

class BaseSystem{
protected:
	static uint32_t _typeCounter;
	EntityManager* const _manager;

public:
	const uint32_t mask;

	BaseSystem(EntityManager* manager, uint32_t mask);
	virtual ~BaseSystem() = 0;

	virtual void load(){};
	virtual void update(){};

	virtual void onCreate(uint64_t id){};
	virtual void onDestroy(uint64_t id){};

	virtual void onActivate(uint64_t id){};
	virtual void onDeactivate(uint64_t id){};
};

template <class ...Ts>
class System : public BaseSystem{
	System() = delete;

public:
	const uint32_t size;

	System(EntityManager* manager);

	static unsigned int type();

	virtual void onProcess(uint64_t id, Ts*...){};
};

template<class ...Ts>
System<Ts...>::System(EntityManager* manager) : BaseSystem(manager, BitHelper::createMask<Ts...>()), size(std::tuple_size<std::tuple<Ts...>>::value){}

template<class ...Ts>
unsigned int System<Ts...>::type(){
	static unsigned int type = _typeCounter++;
	
	assert(type < 32);
	return type;
}