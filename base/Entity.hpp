#pragma once

#include "EntityManager.hpp"

#include <cstdint>

class Entity{
	EntityManager* _manager = nullptr;
	uint64_t _id;

	bool _valid = false;

public:
	inline Entity();

	inline Entity(EntityManager* manager);
	inline Entity(EntityManager& manager);

	inline Entity(EntityManager* manager, uint64_t id);
	inline Entity(EntityManager& manager, uint64_t id);

	inline Entity(const Entity& other);

	inline ~Entity();

	inline void operator=(const Entity& other);
	inline void operator=(uint64_t id);
	
	inline operator bool();
	inline operator uint64_t();
	inline bool operator==(uint64_t id);

	inline void linkManager(EntityManager* manager);
	
	inline void invalidate();

	inline void create();
	
	inline void setActive(bool active);
	
	template <typename T>
	inline void setComponentEnabled(bool enabled);
	
	template <typename T, typename ...Ts>
	inline T* const addComponent(Ts... args);
	
	template <typename T>
	inline T* const getComponent();
	
	inline void destroy();
	
	template <typename ...Ts>
	inline bool hasComponents();
	
	inline uint8_t state();
	inline uint32_t mask();
	inline uint64_t id();
	inline bool valid();
};

inline Entity::Entity(){}

inline Entity::Entity(EntityManager* manager){
	linkManager(manager);
}

inline Entity::Entity(EntityManager& manager){
	linkManager(&manager);
}

inline Entity::Entity(EntityManager* manager, uint64_t id){
	linkManager(manager);
	(*this) = id;
}

inline Entity::Entity(EntityManager& manager, uint64_t id){
	linkManager(&manager);
	(*this) = id;
}

inline Entity::Entity(const Entity& other) : _manager(other._manager), _id(other._id){
	assert(_manager && other._valid && _manager->entityExists(_id));

	_valid = true;

	// increase reference count for id
	_manager->addReference(_id);
}

inline Entity::~Entity(){
	// if valid decrease reference count for id
	if (_valid)
		_manager->removeReference(_id);
}

inline void Entity::operator=(const Entity& other){
	assert(other._manager);

	_manager = other._manager;

	if (other._valid)
		(*this) = other._id;
}

inline void Entity::operator=(uint64_t id){
	assert(_manager && _id != id  && _manager->entityExists(id));

	// decrease reference count for old id
	if (_valid)
		_manager->removeReference(_id);

	_id = id;
	_valid = true;

	// increase reference count for new id
	_manager->addReference(_id);
}

inline Entity::operator bool(){
	return _valid && state() != EntityManager::EntityState::Destroyed;
}

inline Entity::operator uint64_t(){
	return _id;
}

inline bool Entity::operator==(uint64_t id){
	return _id == id;
}

inline void Entity::linkManager(EntityManager* manager){
	assert(manager);

	_manager = manager;
}

inline uint8_t Entity::state(){
	assert(_manager && _valid);

	return _manager->getEntityState(_id);
}

inline void Entity::invalidate(){
	assert(_manager);

	if (_valid){
		_manager->removeReference(_id);

		_valid = false;
	}
}

inline void Entity::create(){
	assert(_manager && !_valid);

	(*this) = _manager->createEntity();
}

inline uint32_t Entity::mask(){
	assert(_manager && _valid);

	return _manager->getEntityMask(_id);
}

inline uint64_t Entity::id(){
	assert(_valid);

	return _id;
}

inline bool Entity::valid(){
	return _valid;
}

inline void Entity::setActive(bool active){
	assert(_manager && _valid);

	_manager->setEntityActive(_id, active);
}

inline void Entity::destroy(){
	invalidate();

	_manager->destroyEntity(_id);
}

template<typename T>
inline void Entity::setComponentEnabled(bool enabled){
	assert(_manager && _valid);

	_manager->setComponentEnabled<T>(_id, enabled);
}

template<typename T, typename ...Ts>
inline T* const Entity::addComponent(Ts... args){
	assert(_manager && _valid);

	return _manager->addComponent<T>(_id, args...);
}

template<typename T>
inline T* const Entity::getComponent(){
	assert(_manager && _valid);

	return _manager->getComponent<T>(_id);
}

template<typename ...Ts>
inline bool Entity::hasComponents(){
	assert(_manager && _valid);

	return _manager->hasComponents<Ts...>();
}
