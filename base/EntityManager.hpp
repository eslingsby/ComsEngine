#pragma once

#include "ObjectPool.hpp"
#include "Component.hpp"
#include "System.hpp"

#include <queue>
#include <tuple>

// To-do
// - Make reload function

class EntityManager{
	// Byte pools for each component type
	std::vector<BasePool*> _pools;

	// Entity occupant slots
	std::vector<uint8_t> _states;
	// Bit masks representing components of an entity
	std::vector<uint32_t> _masks; 
	// Bit masks representing enabled components
	std::vector<uint32_t> _enabled;
	// Version of each entity
	std::vector<uint32_t> _versions;
	// Reference counters
	std::vector<uint16_t> _references;

	// List of recently destroyed entities
	std::queue<uint32_t> _free; 

	// Entity count
	uint32_t _entities = 0;

	// System register for calling BaseSystem events
	std::vector<BaseSystem*> _systems;

	// Non-copyable overloads
	EntityManager(const EntityManager& other) = delete;
	EntityManager& operator=(const EntityManager& other) = delete;

	// Erase entity component's from object pools
	inline void _eraseEntity(uint32_t index);

	// Fill component type tuple with relevent components at existing entity index
	// Entry point of compile time recursive function
	template <typename ...Ts>
	inline void _fillTuple(uint32_t index, std::tuple<Ts*...>& t); 

	// Actual transformation function
	template <uint32_t I, typename ...Ts>
	static inline void _fillTuple(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t);

	// Body of compiled recursive function
	template <uint32_t I, typename ...Ts>
	struct FillTuple{
		inline void operator()(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){ 
			_fillTuple<I, Ts...>(pools, index, t);
			FillTuple<I - 1, Ts...>{}(pools, index, t);
		}
	};

	// Bottom level of compile time recursive function
	template <typename ...Ts>
	struct FillTuple<0, Ts...>{
		inline void operator()(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){ 
			_fillTuple<0, Ts...>(pools, index, t);
		}
	};

	// Bottom level for systems with no components
	template <typename ...Ts>
	struct FillTuple<-1, Ts...>{
		inline void operator()(std::vector<BasePool*>& pools, uint32_t index, std::tuple<Ts*...>& t){} 
	};

public:
	// Entity states for entity _states vector
	enum EntityState : uint8_t{ Empty, Active, Inactive, Destroyed };

	EntityManager();
	~EntityManager();

	// Create new empty entity. Manager will re-use previously destroyed entity location if available,
	// and will increment the entity version. A 64 bit entity ID is made up the entitie's
	// index in memory, and its version number, ensuring all IDs refer to specific versions.
	inline uint64_t createEntity();

	// Destroy entity and components, and erase if entity has no references.
	// If entity has references, entity will be erased when all references are gone, meanwhile the
	// onDestroy function will be called as usual, and entity will stop being processed by systems.
	inline void destroyEntity(uint64_t id);

	// Set if entity gets processed by systems
	inline void setEntityActive(uint64_t id, bool active);

	// Set if component gets processed by systems
	template <typename T>
	inline void setComponentEnabled(uint64_t id, bool enabled);

	// Create and attach new unique component to existing entity
	template <typename T, typename ...Ts>
	inline T* const addComponent(uint64_t id, Ts... args);

	// Get unique component pointer from existing entity
	template <typename T>
	inline T* const getComponent(uint64_t id);

	// Pass all valid entities to a system via system's onProcess(Ts... args) function
	template <typename ...Ts>
	inline void processEntities(System<Ts...>* system);
	
	// Get the state of existing entity, states being defined in EntityState enum
	inline uint8_t getEntityState(uint64_t id);

	// Get the 32 bit mask of representing components owned by existing entity
	//
	//		Each component class has a type integer, in the below example assume that
	//		Transform has a type number of 0, and Velocity has a type number of 4
	//
	//		uint64_t id = manager.createEntity();
	//		manager.addComponent<Transform>(id, 0.f, 0.f, 0.f);
	//		manager.addComponent<Velocity>(id, 10.f);
	//
	//		manager.getEntityMask(id);		// = 0xb10001000000000000000000000000000
	//
	inline uint32_t getEntityMask(uint64_t id);

	// Returns true if entity has components
	//
	//		bool result = manager.hasComponents<Transform, Velocity>(id);
	//
	template <typename ...Ts>
	inline bool hasComponents(uint64_t id);

	// Returns if entity ID is valid
	inline bool entityExists(uint64_t id);

	// Increments an entitie's reference count, causing it not to be fully erased on destroy
	inline void addReference(uint64_t id);

	// Decrements an entitie's reference count
	inline void removeReference(uint64_t id);

	// Subscribes system to entity events via onCreate, onDestroy, onActivate, onDeactivate functions
	template<typename T>
	void registerSystem(T* system);
};

template<typename T>
inline void EntityManager::registerSystem(T* system){
	if (T::type() >= _systems.size())
		_systems.resize(T::type() + 1);

	assert(!_systems[T::type()]);

	_systems[T::type()] = system;
}

inline void EntityManager::addReference(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	_references[index]++;
}

inline void EntityManager::removeReference(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version && _references[index] != 0);

	_references[index]--;

	if (!_references[index] && _states[index] == EntityState::Destroyed)
		_eraseEntity(index);
}

inline void EntityManager::_eraseEntity(uint32_t index){
	for (uint8_t i = 0; i < _pools.size(); i++){
		if (BitHelper::getBit(i, _masks[index]))
			_pools[i]->erase(index);
	}

	_free.push(index);

	_states[index] = EntityState::Empty;

	_versions[index]++;

	_entities--;
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
		if (_states.size() <= _entities){
			_states.resize(_entities + 1);
			_masks.resize(_entities + 1);
			_versions.resize(_entities + 1);
			_enabled.resize(_entities + 1);
			_references.resize(_entities + 1);
		}

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

	if (_states[index] == EntityState::Destroyed)
		return;

	_states[index] = EntityState::Destroyed;

	// Call onDestroy
	for (BaseSystem* system : _systems){
		if ((system->mask & _masks[index]) == system->mask)
			system->onDestroy(id);
	}

	if (!_references[index])
		_eraseEntity(index);
}

inline void EntityManager::setEntityActive(uint64_t id, bool active){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version && _states[index] != EntityState::Destroyed);

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

	assert(index < _masks.size() && _states[index] && _versions[index] == version && BitHelper::getBit(T::type(), _masks[index]) && _states[index] != EntityState::Destroyed);

	_enabled[index] = BitHelper::setBit(T::type(), enabled, _enabled[index]);
}

template <typename T, typename ...Ts>
inline T* const EntityManager::addComponent(uint64_t id, Ts... args){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version && !BitHelper::getBit(T::type(), _masks[index]) && _states[index] != EntityState::Destroyed);

	if (T::type() >= _pools.size())
		_pools.resize(T::type() + 1);

	if (!_pools[T::type()])
		_pools[T::type()] = new ObjectPool<T>(T::chunk());

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
inline T* const EntityManager::getComponent(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	if (T::type() >= _pools.size() || !_pools[T::type()] || index > _states.size() || !_states[index] || _versions[index] != version || _states[index] == EntityState::Destroyed)
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

			system->onProcess(BitHelper::combine(i, _versions[i]), *std::get<Ts*>(components)...);
		}
	}
}

inline uint8_t EntityManager::getEntityState(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	return _states[index];
}

inline uint32_t EntityManager::getEntityMask(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	return _masks[index];
}

template<typename ...Ts>
inline bool EntityManager::hasComponents(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	assert(index < _masks.size() && _states[index] && _versions[index] == version);

	uint32_t mask = BitHelper::createMask<Ts...>();

	return (mask & _masks[index]) == mask;
}

inline bool EntityManager::entityExists(uint64_t id){
	uint32_t index = BitHelper::front(id);
	uint32_t version = BitHelper::back(id);

	return (index < _masks.size() && _states[index] && _versions[index] == version);
}