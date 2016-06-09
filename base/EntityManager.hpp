#pragma once

#include "ObjectPool.hpp"
#include "Component.hpp"
#include "System.hpp"

#include <queue>

class EntityManager{
	std::vector<ObjectPool*> _pools; // Byte pools for each component type

	// Below vectors use the same key for each entity
	std::vector<uint8_t> _states; // Entity occupant slots

	std::vector<uint32_t> _masks; // Bit masks representing components of an entity
	std::vector<uint32_t> _enabled; // Bit masks representing enabled components
	std::vector<uint32_t> _versions; // Version of each entity

	std::queue<uint32_t> _free; // List of recently destroyed entities

	uint32_t _entities = 0;

	EntityManager(const EntityManager& other) = delete;
	EntityManager& operator=(const EntityManager& other) = delete;

	template<typename ...Ts>
	inline void _fillTuple(uint32_t index, std::tuple<Ts*...>& t); // Entry point of recursive function

	template<uint32_t I, typename ...Ts>
	static inline void _fillTuple(std::vector<ObjectPool*>& pools, uint32_t index, std::tuple<Ts*...>& t); // Actual transformation function

	enum EntityState : uint8_t{
		Empty,
		Active,
		Inactive
	};

	template<uint32_t I, typename ...Ts>
	struct FillTuple{
		inline void operator()(std::vector<ObjectPool*>& pools, uint32_t index, std::tuple<Ts*...>& t){ // Body of compiled recursive function
			_fillTuple<I, Ts...>(pools, index, t);
			FillTuple<I - 1, Ts...>{}(pools, index, t);
		}
	};
	
	template<typename ...Ts>
	struct FillTuple<0, Ts...>{
		inline void operator()(std::vector<ObjectPool*>& pools, uint32_t index, std::tuple<Ts*...>& t){ // Bottom level of compiled recursive function
			_fillTuple<0, Ts...>(pools, index, t);
		}
	};

public:
	EntityManager();
	~EntityManager();

	inline uint64_t createEntity();
	inline void destroyEntity(uint64_t id);

	inline void setEntityActive(uint64_t id, bool active);

	template <typename T>
	inline void setComponentEnabled(uint64_t id, bool enabled);

	template <typename T, typename ...Ts>
	inline T* addComponent(uint64_t id, Ts... args);

	template <typename T>
	inline T* getComponent(uint64_t id);

	template <typename ...Ts>
	inline void processEntities(System<Ts...>* system);
};


template<typename ...Ts>
inline void EntityManager::_fillTuple(uint32_t index, std::tuple<Ts*...>& t){
	const uint32_t size = std::tuple_size<std::tuple<Ts*...>>::value;
	FillTuple<size - 1, Ts...>{}(_pools, index, t);
}

template<uint32_t I, typename ...Ts>
inline void EntityManager::_fillTuple(std::vector<ObjectPool*>& pools, uint32_t index, std::tuple<Ts*...>& t){
	using T = typename std::tuple_element<I, std::tuple<Ts...>>::type;

	assert(pools.size() > T::type());

	if (pools[T::type()]){
		T* component = pools[T::type()]->get<T>(index);

		assert(component != nullptr);

		std::get<I>(t) = pools[T::type()]->get<T>(index);
	}
}

inline uint64_t EntityManager::createEntity(){
	uint32_t index;

	if (_free.size()){
		index = _free.front();
		_free.pop();
	}
	else{
		_states.resize(_entities + 1);
		_masks.resize(_entities + 1);
		_versions.resize(_entities + 1);
		_enabled.resize(_entities + 1);

		index = _entities;
	}

	assert(!_states[index]);

	_states[index] = EntityState::Active;

	_masks[index] = 0;
	_enabled[index] = 0;

	if (!_versions[index])
		_versions[index] = 1;

	_entities++;

	return BitHelper::combine(index, _versions[index]);
}

inline void EntityManager::destroyEntity(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	for (uint8_t i = 0; i < _pools.size(); i++){
		if (BitHelper::getBit(i, _masks[index]))
			_pools[i]->get<BaseComponent>(index)->~BaseComponent();
	}

	_free.push(index);

	_states[index] = EntityState::Empty;

	_versions[index]++;

	_entities--;
}

inline void EntityManager::setEntityActive(uint64_t id, bool active){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	if (active)
		_states[index] = EntityState::Active;
	else
		_states[index] = EntityState::Inactive;
}

template <typename T>
inline void EntityManager::setComponentEnabled(uint64_t id, bool enabled){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version && BitHelper::getBit(T::type(), _masks[index]));

	_enabled[index] = BitHelper::setBit(T::type(), enabled, _enabled[index]);
}

template <typename T, typename ...Ts>
inline T* EntityManager::addComponent(uint64_t id, Ts... args){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version && !BitHelper::getBit(T::type(), _masks[index]));

	if (T::type() >= _pools.size()){
		_pools.resize(T::type() + 1);
		_pools[T::type()] = new ObjectPool(sizeof(T));
	}

	_masks[index] = BitHelper::setBit(T::type(), true, _masks[index]);
	_enabled[index] = BitHelper::setBit(T::type(), true, _enabled[index]);

	return _pools[T::type()]->insert(index, T(args...));
}

template<typename T>
inline T* EntityManager::getComponent(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	if (T::type() >= _pools.size() || !_pools[T::type()] || index > _states.size() || !_states[index] || _versions[index] != version)
		return nullptr;

	return _pools[T::type()]->get<T>(index);
}

template <typename ...Ts>
inline void EntityManager::processEntities(System<Ts...>* system){
	std::tuple<Ts*...> components;

	for (uint32_t i = 0; i < _states.size(); i++){
		if (_states[i] != EntityState::Active)
			continue;

		if ((system->mask & _enabled[i]) == system->mask){
			_fillTuple(i, components);
			(*system)(BitHelper::combine(i, _versions[i]), std::get<Ts*>(components)...);
		}
	}
}