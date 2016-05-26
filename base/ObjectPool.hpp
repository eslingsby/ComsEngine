#pragma once

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <vector>
#include <math.h>

// Currently expands in chunks of elements, but really should expand in chunks of fixed amounts of bytes
// The size parameter should also be in bytes instead of elements

class ObjectPool{
	const uint32_t _chunkSize; // Size to increase buffer by when full (elements)
	const uint32_t _elementSize; // Size of elements (bytes)

	uint8_t* _buffer; // Byte buffer

	std::vector<uint8_t> _occupied; // Smaller array to check slot availability

	uint32_t _size = 0; // Total size of buffer (elements)
	uint32_t _occupants = 0; // Amount of objects in the buffer (active objects)

	inline uint8_t* _conv(void* memory) const; // Malloc memory check, converts to byte type

	inline void _expand(uint32_t chunks);

	inline void _erase(uint32_t location); // Frees occupant slot at location

public:
	ObjectPool(uint32_t elementSize, uint32_t chunkSize = 8);
	~ObjectPool();

	inline bool check(uint32_t location) const;

	template <typename T>
	inline T* get(uint32_t location, bool implicit = false) const;

	template <typename T>
	inline void erase(uint32_t location);

	template <typename T>
	inline void erase(T* object);

	template <typename T>
	inline T* insert(uint32_t location, const T& value);

	inline uint32_t occupants() const;
	inline uint32_t size() const;

	void print() const;
};

inline uint8_t* ObjectPool::_conv(void* memory) const{
	assert(memory != nullptr);

	return (uint8_t*)memory;
}

inline void ObjectPool::_expand(uint32_t chunks){
	if (_size == 0)
		_buffer = _conv(malloc(_chunkSize * chunks * _elementSize)); // If empty, create chunk of memory
	else
		_buffer = _conv(realloc(_buffer, (_size + (_chunkSize * chunks)) * _elementSize)); // Else, expand current buffer by _chunkSize * chunks

	_size += _chunkSize * chunks;
	_occupied.resize(_size, 0);
}

inline void ObjectPool::_erase(uint32_t location){
	assert(location < _size);

	_occupied[location] = 0;
	_occupants--;
}

inline bool ObjectPool::check(uint32_t location) const{
	if (location >= _size || _occupied[location] == 0)
		return false;

	return true;
}

template<typename T>
inline T* ObjectPool::get(uint32_t location, bool implicit) const{
	assert(sizeof(T) == _elementSize);

	if (!implicit && !check(location))
		return nullptr;

	return reinterpret_cast<T*>(_buffer + (location * _elementSize)); // Cast from byte buffer location
}

template <typename T>
inline void ObjectPool::erase(uint32_t location){
	assert(sizeof(T) == _elementSize);

	T* object = get<T>(location);

	assert(object != nullptr);

	object->~T();
	_erase(location);
}

template <typename T>
inline void ObjectPool::erase(T* object){
	assert(_size != 0 && sizeof(T) == _elementSize);

	uint32_t location = (uint32_t)(object - get<T>(0, true));

	object->~T();
	_erase(location);
}

template <typename T>
inline T*  ObjectPool::insert(uint32_t location, const T& value){
	assert(sizeof(T) == _elementSize);

	if (location >= _size){
		float chunks = (float)(location - _size) / (float)(_chunkSize - 1);
		_expand((uint32_t)ceil(chunks));
	}
	else{
		if (_occupied[location] != 0)
			return nullptr;
	}

	memcpy(_buffer + location * _elementSize, &value, _elementSize); // Copy object to byte buffer

	_occupied[location] = 0xFF;
	_occupants++; 

	return get<T>(location, true); 
}

inline uint32_t ObjectPool::occupants() const{
	return _occupants;
}

inline uint32_t ObjectPool::size() const{
	return _size;
}