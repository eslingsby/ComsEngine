#pragma once

#include "EntityRef.hpp"

template <class T>
class ComponentRef : public EntityRef{
	T* _component = nullptr;

public:
	inline ComponentRef(EntityManager* manager, uint64_t id);
	inline ComponentRef(EntityManager& manager, uint64_t id);

	inline void operator=(const EntityRef& other) override;
	inline void operator=(uint64_t id) override;

	inline T& component();
};

template <class T>
inline ComponentRef<T>::ComponentRef(EntityManager* manager, uint64_t id) : EntityRef(manager, id){
	_component = getComponent<T>();
	assert(component);
}

template <class T>
inline ComponentRef<T>::ComponentRef(EntityManager& manager, uint64_t id) : EntityRef(manager, id){
	_component = getComponent<T>();
	assert(_component);
}

template <class T>
inline void ComponentRef<T>::operator=(const EntityRef& other){
	EntityRef::operator=(other);
	_component = getComponent<T>();
	assert(_component);
}

template <class T>
inline void ComponentRef<T>::operator=(uint64_t id){
	EntityRef::operator=(id);
	_component = getComponent<T>();
	assert(_component);
}

template <class T>
inline T& ComponentRef<T>::component(){
	assert(_valid && _component);
	return *_component;
}