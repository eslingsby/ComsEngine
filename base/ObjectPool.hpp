#pragma once

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <queue>
#include <vector>

class ObjectPool{
	const uint32_t _chunkSize; // Size to increase buffer by when full (elements)
	const uint32_t _elementSize; // Size of elements (bytes)

	uint8_t* _buffer; // (bytes)

	std::queue<uint32_t> _free; // List of free slots
	std::vector<uint32_t> _version; // Version of each object slot (freeing a slot increments version)

	uint32_t _size = 0; // Total size of buffer (elements)
	uint32_t _occupants = 0; // Amount of objects in the buffer (active objects)

	uint8_t* _conv(void* memory) const; // Malloc memory check, converts to byte type

	inline void _expand(unsigned int chunks = 1);

	// ID byte conversion helpers
	static inline uint32_t _ver(uint64_t i);
	static inline uint32_t _loc(uint64_t i);
	static inline uint64_t _id(uint32_t loc, uint32_t ver);

public:
	ObjectPool(uint32_t elementSize, uint32_t chunkSize = 8);
	~ObjectPool();

	void print() const;

	template <typename T>
	inline T* get(uint64_t id) const;

	template <typename T>
	inline void erase(uint64_t id);

	template <typename T>
	inline uint64_t insert(const T& value);
};

inline uint32_t ObjectPool::_ver(uint64_t i){
	return (uint32_t)i;
}

inline uint32_t ObjectPool::_loc(uint64_t i){
	return (uint32_t)(i >> 32);
}

inline uint64_t ObjectPool::_id(uint32_t loc, uint32_t ver){
	return ver + ((uint64_t)loc << 32);
}

inline void ObjectPool::_expand(unsigned int chunks){
	if (_size == 0){
		_buffer = _conv(calloc(_chunkSize * chunks, _elementSize)); // If empty, create chunk of clear memory
	}
	else{
		_buffer = _conv(realloc(_buffer, (_size + (_chunkSize * chunks)) * _elementSize)); // Else, expand current buffer by _chunkSize * chunks
		memset(_buffer + (_occupants * _elementSize), 0, _chunkSize * _elementSize); // clear garbage memory created from realloc
	}

	_size += _chunkSize;
	_version.resize(_size);
}

template<typename T>
inline T* ObjectPool::get(uint64_t id) const{
	uint32_t version = _ver(id); // Get version from slot ID
	uint32_t location = _loc(id); // Get location from slot ID

	if (location > _occupants || _version[location] == 0 || _version[location] != version)
		return nullptr;	// Return null if out of range, version is zero, or mismatching version number

	return reinterpret_cast<T*>(_buffer + (location * _elementSize)); // Cast from byte buffer location
}

template <typename T>
inline void ObjectPool::erase(uint64_t id){
	T* object = get<T>(id);

	// Break if out of range or mismatching version type (maybe too much?)
	assert(object != nullptr);

	object->~T();

	uint32_t location = _loc(id);

	memset(_buffer + (location * _elementSize), 0, _elementSize); // Clear slot to zero

	_free.push(location); // Push free location to free queue

	_occupants--; // Decrement occupant count
	_version[location]++; // Increment version to ensure can't be accessed again
}

template <typename T>
inline uint64_t ObjectPool::insert(const T& value){
	uint32_t location;

	if (_free.size()){
		location = _free.front(); // Take free slot if available
		_free.pop();
	}
	else{
		if (_occupants == _size) // If buffer is full (no free slots), expand by 1 chunk
			_expand();

		location = _occupants; // Location is set to end of occupants
	}

	memcpy(_buffer + location * _elementSize, &value, _elementSize); // Copy object to byte buffer

	_occupants++; // Increment occupant count

	if (_version[location] == 0)
		_version[location] = 1; // Initiate version to 1 version equals 0

	return _id(location, _version[location]); // Return id by combining current location and version
}