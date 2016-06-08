#pragma once

#include "ObjectPool.hpp"
#include "Component.hpp"
#include "System.hpp"

#include <queue>
#include <functional>
#include <type_traits>
#include <utility>

/*
- Create entity version array and occupied array
- Maybe entity component disabled array
- Remove last bit flag from masks
*/

class EntityManager{
	std::vector<ObjectPool*> _pools; // Byte pools for each component type

	// Below vectors use the same key for each entity
	std::vector<uint32_t> _masks; // Bit mask representing components of an entity
	std::vector<uint8_t> _active; // Entity acvtive flags
	std::vector<uint32_t> _version; // Version of each entity

	std::queue<uint32_t> _free; // List of recently destroyed entities

	uint32_t _entities = 0;

	EntityManager(const EntityManager& other) = delete;
	EntityManager& operator=(const EntityManager& other) = delete;
	
	template<typename ...Ts>
	inline void _fillTuple(uint32_t id, std::tuple<Ts*...>& t);

	template<uint32_t I, typename ...Ts>
	static inline void _fillTuple(std::vector<ObjectPool*>& pools, uint32_t id, std::tuple<Ts*...>& t);

	template<uint32_t I, typename ...Ts>
	struct FillTuple{
		inline void operator()(std::vector<ObjectPool*>& pools, uint32_t id, std::tuple<Ts*...>& t){
			_fillTuple<I, Ts...>(pools, id, t);	
			FillTuple<I - 1, Ts...>{}(pools, id, t);
		}
	};
	
	template<typename ...Ts>
	struct FillTuple<0, Ts...>{
		inline void operator()(std::vector<ObjectPool*>& pools, uint32_t id, std::tuple<Ts*...>& t){
			_fillTuple<0, Ts...>(pools, id, t);
		}
	};

	static inline uint32_t _setBit(uint8_t i, bool value, uint32_t bits = 0);
	static inline bool _getBit(uint8_t i, uint32_t bits);

public:
	EntityManager();
	~EntityManager();

	inline uint32_t createEntity();
	inline void destroyEntity(uint32_t id);

	inline void setEntityActive(uint32_t id, bool active);

	template <typename T>
	inline void setComponentEnabled(uint32_t id, bool enabled);

	template <typename T, typename ...Ts>
	inline void addComponent(uint32_t id, Ts... args);

	template <typename T>
	inline T* getComponent(uint32_t id);

	template <typename ...Ts>
	inline void processEntities(System<Ts...>* system);
};

template<typename ...Ts>
inline void EntityManager::_fillTuple(uint32_t id, std::tuple<Ts*...>& t){
	const uint32_t size = std::tuple_size<std::tuple<Ts*...>>::value;
	FillTuple<size - 1, Ts...>{}(_pools, id, t);
}

template<uint32_t I, typename ...Ts>
inline void EntityManager::_fillTuple(std::vector<ObjectPool*>& pools, uint32_t id, std::tuple<Ts*...>& t){
	using T = typename std::tuple_element<I, std::tuple<Ts...>>::type;

	assert(pools.size() > T::type());

	if (pools[T::type()]){
		T* component = pools[T::type()]->get<T>(id);

		assert(component != nullptr);

		std::get<I>(t) = pools[T::type()]->get<T>(id);
	}
}

inline uint32_t EntityManager::_setBit(uint8_t i, bool value, uint32_t bits){
	if (value)
		return bits | (1 << i);
	else
		return bits | (0 << i);
}

inline bool EntityManager::_getBit(uint8_t i, uint32_t bits){
	if ((1 << i) & bits)
		return true;

	return false;
}

inline uint32_t EntityManager::createEntity(){
	uint32_t id;

	if (_free.size()){
		id = _free.front();
		_free.pop();
	}
	else{
		_masks.resize(_entities + 1);
		id = _entities;
	}

	_masks[id] = _setBit(31, true);
	_entities++;

	return id;
}

inline void EntityManager::destroyEntity(uint32_t id){
	assert(id < _masks.size() && _masks[id]);

	for (uint8_t i = 0; i < _pools.size(); i++){
		if (_getBit(i, _masks[id]))
			_pools[i]->get<BaseComponent>(id)->~BaseComponent();
	}

	_free.push(id);

	_masks[id] = 0;
	_entities--;
}

inline void EntityManager::setEntityActive(uint32_t id, bool active){

}

template <typename T>
inline void EntityManager::setComponentEnabled(uint32_t id, bool enabled){

}

template <typename T, typename ...Ts>
inline void EntityManager::addComponent(uint32_t id, Ts... args){
	assert(id < _masks.size() && _masks[id]);

	if (T::type() >= _pools.size()){
		_pools.resize(T::type() + 1);
		_pools[T::type()] = new ObjectPool(sizeof(T));
	}

	_masks[id] = _setBit(T::type(), true, _masks[id]);

	_pools[T::type()]->insert(id, T(args...));
}

template<typename T>
inline T* EntityManager::getComponent(uint32_t id){
	if (T::type() >= _pools.size() || !_pools[T::type()] || id > _masks.size() || _masks[id] == 0)
		return nullptr;

	return _pools[T::type()]->get<T>(id);
}

template <typename ...Ts>
inline void EntityManager::processEntities(System<Ts...>* system){
	std::tuple<Ts*...> components;

	_fillTuple(0, components);

	(*system)(0, std::get<Ts*>(components)...);
}