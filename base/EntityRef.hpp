#pragma once

#include "EntityManager.hpp"

#include <cstdint>

class EntityRef{
	EntityManager* _manager = nullptr;
	uint64_t _id;

protected:
	bool _valid = false;

public:
	inline EntityRef();

	inline EntityRef(EntityManager* manager);
	inline EntityRef(EntityManager& manager);

	inline EntityRef(EntityManager* manager, uint64_t id);
	inline EntityRef(EntityManager& manager, uint64_t id);

	inline EntityRef(const EntityRef& other);

	inline ~EntityRef();

	inline virtual void operator=(const EntityRef& other);
	inline virtual void operator=(uint64_t id);
	
	inline operator bool();
	inline operator uint64_t();
	inline bool operator==(uint64_t id);

	inline void linkManager(EntityManager* manager);
	
	inline void invalidate();

	inline virtual void create();
	
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

inline EntityRef::EntityRef(){}

inline EntityRef::EntityRef(EntityManager* manager){
	linkManager(manager);
}

inline EntityRef::EntityRef(EntityManager& manager){
	linkManager(&manager);
}

inline EntityRef::EntityRef(EntityManager* manager, uint64_t id){
	linkManager(manager);
	(*this) = id;
}

inline EntityRef::EntityRef(EntityManager& manager, uint64_t id){
	linkManager(&manager);
	(*this) = id;
}

inline EntityRef::EntityRef(const EntityRef& other) : _manager(other._manager), _id(other._id){
	assert(_manager && other._valid && _manager->entityExists(_id));

	_valid = true;

	// increase reference count for id
	_manager->addReference(_id);
}

inline EntityRef::~EntityRef(){
	// if valid decrease reference count for id
	if (_valid)
		_manager->removeReference(_id);
}

inline void EntityRef::operator=(const EntityRef& other){
	assert(other._manager);

	_manager = other._manager;

	if (other._valid)
		(*this) = other._id;
}

inline void EntityRef::operator=(uint64_t id){
	assert(_manager && _id != id  && _manager->entityExists(id));

	// decrease reference count for old id
	if (_valid)
		_manager->removeReference(_id);

	_id = id;
	_valid = true;

	// increase reference count for new id
	_manager->addReference(_id);
}

inline EntityRef::operator bool(){
	return _valid && state() != EntityManager::EntityState::Destroyed;
}

inline EntityRef::operator uint64_t(){
	return _id;
}

inline bool EntityRef::operator==(uint64_t id){
	return _id == id;
}

inline void EntityRef::linkManager(EntityManager* manager){
	assert(manager);

	_manager = manager;
}

inline uint8_t EntityRef::state(){
	assert(_manager && _valid);

	return _manager->getEntityState(_id);
}

inline void EntityRef::invalidate(){
	assert(_manager);

	if (_valid){
		_manager->removeReference(_id);
		_valid = false;
	}
}

inline void EntityRef::create(){
	assert(_manager && !_valid);

	(*this) = _manager->createEntity();
}

inline uint32_t EntityRef::mask(){
	assert(_manager && _valid);

	return _manager->getEntityMask(_id);
}

inline uint64_t EntityRef::id(){
	assert(_valid);

	return _id;
}

inline bool EntityRef::valid(){
	return _valid;
}

inline void EntityRef::setActive(bool active){
	assert(_manager && _valid);

	_manager->setEntityActive(_id, active);
}

inline void EntityRef::destroy(){
	_manager->destroyEntity(_id);
}

template<typename T>
inline void EntityRef::setComponentEnabled(bool enabled){
	assert(_manager && _valid);

	_manager->setComponentEnabled<T>(_id, enabled);
}

template<typename T, typename ...Ts>
inline T* const EntityRef::addComponent(Ts... args){
	assert(_manager && _valid);

	return _manager->addComponent<T>(_id, args...);
}

template<typename T>
inline T* const EntityRef::getComponent(){
	assert(_manager && _valid);

	return _manager->getComponent<T>(_id);
}

template<typename ...Ts>
inline bool EntityRef::hasComponents(){
	assert(_manager && _valid);

	return _manager->hasComponents<Ts...>();
}
