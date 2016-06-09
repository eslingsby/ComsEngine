#pragma once

#include "BitHelper.hpp"

#include <cassert>
#include <tuple>

class EntityManager;

class BaseSystem{
protected:
	static uint32_t _typeCounter;
	EntityManager* const _manager;

public:
	const uint32_t mask;

	BaseSystem(EntityManager* manager, uint32_t mask);
	virtual ~BaseSystem();

	virtual void update() = 0;
};

template <class ...Ts>
class System : public BaseSystem{
	System() = delete;

	static inline uint32_t _createMask();

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

public:
	const uint32_t size;

	System(EntityManager* manager);

	static unsigned int type();

	virtual void operator()(uint64_t id, Ts*...) = 0;
};

template<class ...Ts>
inline uint32_t System<Ts...>::_createMask(){
	uint32_t mask = 0;

	const uint32_t size = std::tuple_size<std::tuple<Ts...>>::value;
	CreateMask<size - 1, Ts...>{}(mask);

	return mask;
}

template<class ...Ts>
template<uint32_t I>
inline void System<Ts...>::_createMask(uint32_t& mask){
	using T = typename std::tuple_element<I, std::tuple<Ts...>>::type;

	mask = BitHelper::setBit(T::type(), true, mask);
}

template<class ...Ts>
System<Ts...>::System(EntityManager* manager) : BaseSystem(manager, _createMask()), size(std::tuple_size<std::tuple<Ts...>>::value){}

template<class ...Ts>
unsigned int System<Ts...>::type(){
	static unsigned int type = _typeCounter++;
	
	assert(type < 32);
	return type;
}