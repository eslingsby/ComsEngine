#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>

// To-do
// - Convert chunkSize and size to bytes.
// - Null index on erase
// - Check if index is null on get and insert
// - Create pointer to index function

class BasePool{
	// Size to increase buffer by when full (elements).
	const uint32_t _chunkSize; 

	// Size of elements (bytes).
	const uint32_t _elementSize; 

	uint8_t* _buffer;

	// Size of buffer (elements).
	uint32_t _size = 0; 

	// Malloc memory check, converts to byte type.
	inline uint8_t* _conv(void* memory) const; 

	// Expands buffer by n chunks.
	inline void _expand(uint32_t chunks);

public:
	inline BasePool(uint32_t elementSize, uint32_t chunkSize);
	inline virtual ~BasePool();

	// Returns cast pointer to index location in byte buffer,
	// returns nullptr if index is empty, and halts if invalid. 
	// Typename must be same type or base type of element.
	//
	//		BasePool* pool = new ObjectPool<Transform>();
	//		pool->insert<Transform>(0, 100.f, 100.f, 100.f);
	//
	//		Transform* component = pool->get<Transform>(0);
	// 
	template <typename T>
	inline T* const get(uint32_t index) const;

	// Creates new object and inserts at index, halts if index isn't nullptr.
	// Returns pointer to created object for convenience.
	//
	//		BasePool* pool = new ObjectPool<Transform>();
	//
	//		Transform* component = pool->insert<Transform>(0, 100.f, 100.f, 100.f);
	//
	template <typename T, typename ...Ts>
	inline T* const insert(uint32_t index, Ts... args);

	// Removes from object pool, deletes object and calls destructor.
	// Halts if index is invalid or empty.
	// 
	//		BasePool* pool = new ObjectPool<Transform>();
	//		pool->insert<Transform>(0, 100.f, 100.f, 100.f);
	//
	//		pool->erase(0);
	//
	virtual inline void erase(uint32_t index) = 0;

	// Returns size of object element locations.
	inline uint32_t size() const;
};

template <class T>
class ObjectPool : public BasePool{
public:
	ObjectPool(uint32_t chunkSize = 8);

	inline void erase(uint32_t index) override;
};

inline BasePool::BasePool(uint32_t elementSize, uint32_t chunkSize) : _elementSize(elementSize), _chunkSize(chunkSize){
	_expand(1);
}

inline BasePool::~BasePool(){
	free(_buffer);
}

inline uint8_t* BasePool::_conv(void* memory) const{
	assert(memory != nullptr);

	return (uint8_t*)memory;
}

inline void BasePool::_expand(uint32_t chunks){
	if (_size == 0)
		_buffer = _conv(malloc(_chunkSize * chunks * _elementSize)); // If empty, create chunk of memory
	else
		_buffer = _conv(realloc(_buffer, (_size + (_chunkSize * chunks)) * _elementSize)); // Else, expand current buffer by _chunkSize * chunks

	_size += _chunkSize * chunks;
}

template<typename T>
inline T* const BasePool::get(uint32_t index) const{
	assert(sizeof(T) <= _elementSize);

	if (index >= _size)
		return nullptr;

	return reinterpret_cast<T*>(_buffer + (index * _elementSize)); // Cast from byte buffer index
}

template <typename T, typename ...Ts>
inline T* const BasePool::insert(uint32_t index, Ts... args){
	assert(sizeof(T) <= _elementSize);

	if (index >= _size){
		float chunks = (float)(index - _size) / (float)(_chunkSize - 1);
		_expand((uint32_t)ceil(chunks));
	}

	new(_buffer + (index * _elementSize)) T(args...);

	return get<T>(index); 
}

inline uint32_t BasePool::size() const{
	return _size;
}

template<class T>
inline ObjectPool<T>::ObjectPool(uint32_t chunkSize) : BasePool(sizeof(T), chunkSize){}

template<class T>
inline void ObjectPool<T>::erase(uint32_t index){
	T* component = get<T>(index);

	assert(component);

	component->~T();
}
