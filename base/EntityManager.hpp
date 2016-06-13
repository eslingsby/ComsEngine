#pragma once

#include "ObjectPool.hpp"
#include "Component.hpp"
#include "System.hpp"

#include <queue>
#include <tuple>

/*
- Add onCreate and onDestroy System calls
- Change processEntities to allow custom filters and callback functions
*/

class EntityManager{
	std::vector<BasePool*> _pools; // Byte pools for each component type

	// Below vectors use the same key for each entity
	std::vector<uint8_t> _states; // Entity occupant slots

	std::vector<uint32_t> _masks; // Bit masks representing components of an entity
	std::vector<uint32_t> _enabled; // Bit masks representing enabled components
	std::vector<uint32_t> _versions; // Version of each entity

	std::queue<uint32_t> _free; // List of recently destroyed entities

	uint32_t _entities = 0;

	std::vector<BaseSystem*> _systems;

	enum EntityState : uint8_t{ Empty, Active, Inactive };

	EntityManager(const EntityManager& other) = delete;
	EntityManager& operator=(const EntityManager& other) = delete;

	template<typename T>
	void _registerSystem(T* system);

	template<typename ...Ts>
	inline void _fillTuple(uint32_t index, std::tuple<Ts*...>& t); // Entry point of recursive function

	template<uint32_t I, typename ...Ts>
	static inline void _fillTuple(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t); // Actual transformation function

	template<uint32_t I, typename ...Ts>
	struct FillTuple{
		inline void operator()(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){ // Body of compiled recursive function
			_fillTuple<I, Ts...>(pools, index, t);
			FillTuple<I - 1, Ts...>{}(pools, index, t);
		}
	};
	
	template<typename ...Ts>
	struct FillTuple<0, Ts...>{
		inline void operator()(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){ // Bottom level of compiled recursive function
			_fillTuple<0, Ts...>(pools, index, t);
		}
	};

	template<typename ...Ts>
	struct FillTuple<-1, Ts...>{
		inline void operator()(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){} // Bottom level for systems with no components
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

	friend class Engine;
};


template<typename T>
inline void EntityManager::_registerSystem(T* system){
	if (T::type() >= _systems.size())
		_systems.resize(T::type() + 1);

	assert(!_systems[T::type()]);

	_systems[T::type()] = system;
}

template<typename ...Ts>
inline void EntityManager::_fillTuple(uint32_t index, std::tuple<Ts*...>& t){
	const uint32_t size = std::tuple_size<std::tuple<Ts*...>>::value;

	FillTuple<size - 1, Ts...>{}(_pools, index, t);
}

template<uint32_t I, typename ...Ts>
inline void EntityManager::_fillTuple(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){
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

	// Call onCreate (for blank systems)
	uint64_t id = BitHelper::combine(index, _versions[index]);

	for (BaseSystem* system : _systems){
		if (!system->mask)
			system->onCreate(id);
	}

	return id;
}

inline void EntityManager::destroyEntity(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	// Call onDestroy
	for (BaseSystem* system : _systems){
		if ((system->mask & _masks[index]) == system->mask)
			system->onDestroy(id);
	}

	for (uint8_t i = 0; i < _pools.size(); i++){
		if (BitHelper::getBit(i, _masks[index]))
			_pools[i]->erase(index);
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

	if ((active && _states[index] == EntityState::Active) || (!active && _states[index] == EntityState::Inactive))
		return;

	if (active)
		_states[index] = EntityState::Active;
	else
		_states[index] = EntityState::Inactive;

	// Call onActivate / onDeactivate
	for (BaseSystem* system : _systems){
		if ((system->mask & _masks[index]) == system->mask){
			if (active)
				system->onActivate(id);
			else
				system->onDeactivate(id);
		}
	}
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

	if (T::type() >= _pools.size())
		_pools.resize(T::type() + 1);

	if (!_pools[T::type()])
		_pools[T::type()] = new ObjectPool<T>();

	uint32_t old = _masks[index];

	_masks[index] = BitHelper::setBit(T::type(), true, _masks[index]);
	_enabled[index] = BitHelper::setBit(T::type(), true, _enabled[index]);

	T* component = _pools[T::type()]->insert<T>(index, args...);

	// Call onCreate (for potential new systems)
	for (BaseSystem* system : _systems){
		if ((system->mask & _masks[index]) == system->mask && (system->mask & old) != system->mask){
			system->onCreate(id);
		}
	}

	return component;
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

	// Call onProcess
	for (uint32_t i = 0; i < _states.size(); i++){
		if (_states[i] != EntityState::Active)
			continue;

		if ((system->mask & _enabled[i]) == system->mask){
			if (system->mask)
				_fillTuple(i, components);

			system->onProcess(BitHelper::combine(i, _versions[i]), std::get<Ts*>(components)...);
		}
	}
}