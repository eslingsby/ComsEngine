#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>

/*
- Convert chunkSize and size to bytes
*/

class BasePool{
	const uint32_t _chunkSize; // Size to increase buffer by when full (elements)
	const uint32_t _elementSize; // Size of elements (bytes)

	uint8_t* _buffer; // Byte buffer

	uint32_t _size = 0; // Total size of buffer (elements)

	inline uint8_t* _conv(void* memory) const; // Malloc memory check, converts to byte type

	inline void _expand(uint32_t chunks);

public:
	BasePool(uint32_t elementSize, uint32_t chunkSize);
	virtual ~BasePool();

	template <typename T>
	inline T* get(uint32_t index) const;

	template <typename T, typename ...Ts>
	inline T* insert(uint32_t index, Ts... args);

	virtual inline void erase(uint32_t index) = 0;

	inline uint32_t size() const;
};

template <class T>
class ObjectPool : public BasePool{
public:
	ObjectPool(uint32_t chunkSize = 8);

	inline void erase(uint32_t index) override;
};

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
inline T* BasePool::get(uint32_t index) const{
	assert(sizeof(T) <= _elementSize);

	if (index >= _size)
		return nullptr;

	return reinterpret_cast<T*>(_buffer + (index * _elementSize)); // Cast from byte buffer index
}


template <typename T, typename ...Ts>
inline T*  BasePool::insert(uint32_t index, Ts... args){
	assert(sizeof(T) <= _elementSize);

	if (index >= _size){
		float chunks = (float)(index - _size) / (float)(_chunkSize - 1);
		_expand((uint32_t)ceil(chunks));
	}

	//memcpy(_buffer + index * _elementSize, &value, _elementSize); // Copy object to byte buffer
	new(_buffer + index * _elementSize) T(args...);

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
