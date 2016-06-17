#pragma once

#include "EntityManager.hpp"

#include <cstdint>

class Entity{
	EntityManager* const _manager;
	uint64_t _id;

	bool _valid = false;

public:
	inline Entity(EntityManager* manager, uint64_t id);
	inline Entity(EntityManager* manager);
	inline Entity(const Entity& other);

	inline ~Entity();

	inline Entity operator=(const Entity& other);

	inline operator bool();

	inline void invalidate();

	inline void setActive(bool active);

	template <typename T>
	inline void setComponentEnabled(bool enabled);

	template <typename T, typename ...Ts>
	inline T* addComponent(Ts... args);

	template <typename T>
	inline T* getComponent();

	inline void destroy();

	inline uint8_t state();
	inline uint32_t mask();
	inline uint64_t id();
};

inline Entity::Entity(EntityManager* manager, uint64_t id) : _manager(manager), _id(id){
	assert(manager);

	_valid = true;

	// increase reference count for id
	_manager->_addReference(_id);
}

inline Entity::Entity(EntityManager* manager) : _manager(manager){
	assert(manager);

	_id = _manager->createEntity();
	_valid = true;

	// increase reference count for id
	_manager->_addReference(_id);
}

inline Entity::Entity(const Entity& other) : _manager(other._manager), _id(other._id){
	assert(other._valid);

	_valid = true;

	// increase reference count for id
	_manager->_addReference(_id);
}

inline Entity::~Entity(){
	// if valid decrease reference count for id
	if (_valid)
		_manager->_removeReference(_id);
}

inline Entity Entity::operator=(const Entity & other){
	assert(other._manager && other._valid && other._id != _id);

	// decrease reference count for old id
	_manager->_removeReference(_id);

	_id = other._id;
	_valid = true;

	// increase reference count for new id
	_manager->_addReference(_id);
}

inline Entity::operator bool(){
	return state() != EntityManager::EntityState::Destroyed;
}

inline uint8_t Entity::state(){
	assert(_valid);

	return _manager->getEntityState(_id);
}

inline void Entity::invalidate(){
	assert(_valid);

	// decrease reference count for id
	_manager->_removeReference(_id);

	_valid = false;
}

inline uint32_t Entity::mask(){
	assert(_valid);

	return _manager->getEntityMask(_id);
}

inline uint64_t Entity::id(){
	assert(_valid);

	return _id;
}

inline void Entity::setActive(bool active){
	assert(_valid);

	_manager->setEntityActive(_id, active);
}

inline void Entity::destroy(){
	invalidate();

	_manager->destroyEntity(_id);
}

template<typename T>
inline void Entity::setComponentEnabled(bool enabled){
	assert(_valid);

	_manager->setComponentEnabled<T>(_id, enabled);
}

template<typename T, typename ...Ts>
inline T* Entity::addComponent(Ts... args){
	assert(_valid);

	return _manager->addComponent<T>(_id, args...);
}

template<typename T>
inline T* Entity::getComponent(){
	assert(_valid);

	return _manager->getComponent<T>(_id);
}