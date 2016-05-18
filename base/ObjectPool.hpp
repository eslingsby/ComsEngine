#pragma once

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <queue>
#include <vector>

#include <iostream>

template <class T>
class ObjectPool{
	const uint32_t _chunkSize; // Size to increase buffer by when full (elements)
	const uint32_t _elementSize; // Size of T (bytes)

	uint8_t* _buffer; // Byte buffer

	std::queue<uint32_t> _free; // List of free slots
	std::vector<uint32_t> _version; // Version of each object slot (freeing a slot increments version)

	uint32_t _size = 0; // Total size of buffer (elements)
	uint32_t _occupants = 0; // Amount of objects in the buffer (active objects)

	inline uint32_t _ver(uint64_t i) const{ // First half of 64 bit ID
		return (uint32_t)i;
	}

	inline uint32_t _loc(uint64_t i) const{ // Second half of 64 bit ID
		return (uint32_t)(i >> 32); 
	}

	inline uint64_t _id(uint32_t loc, uint32_t ver) const{ // Creates 64 bit ID from combining location and version of slot
		return ver + ((uint64_t)loc << 32); 
	}
	
	inline uint8_t* _conv(void* memory) const{
		assert(memory != nullptr);
		return (uint8_t*)memory;
	}

	inline void _expand(unsigned int chunks = 1){
		if (_size == 0)
			_buffer = _conv(malloc((_size + (_chunkSize * chunks)) * _elementSize)); // If empty, create chunk of memory
		else
			_buffer = _conv(realloc(_buffer, (_size + (_chunkSize * chunks)) * _elementSize)); // Else, expand current buffer by _chunkSize * chunks

		_size += _chunkSize;
		_version.resize(_size);
	}

public:
	ObjectPool() : _chunkSize(8), _elementSize(sizeof(T)){
		_expand(); // Initially expand to 1 chunk
	}

	~ObjectPool(){
		std::free(_buffer);
	}

	inline T* get(uint64_t id) const{
		uint32_t version = _ver(id); // Get version from slot ID
		uint32_t location = _loc(id); // Get location from slot ID

		if (location > _occupants || _version[location] == 0 || _version[location] != version)
			return nullptr;	// Return null if out of range or mismatching version type

		return reinterpret_cast<T*>(_buffer + (location * _elementSize)); // Cast from byte buffer location
	}

	inline void free(uint64_t id){
		T* object = get(id);

		// Break if out of range or mismatching version type (maybe too much?)
		assert(object != nullptr);

		object->~T();

		uint32_t location = _loc(id);

		_free.push(location); // Push free location to free queue

		_occupants--; // Decrement occupant count
		_version[location]++; // Increment version to ensure can't be accessed again
	}

	inline uint64_t insert(const T& value){
		uint32_t location;

		if (_free.size()){
			location = _free.front(); // Take free slot if available
			_free.pop();
		}
		else{
			if (_occupants == _size) // If buffer is full, expand by 1 chunk
				_expand();

			location = _occupants; // Location is at the end of occupants
		}

		memcpy(_buffer + location * _elementSize, &value, _elementSize); // Copy object to byte buffer

		_occupants++; // Increment occupant count

		if (_version[location] == 0)
			_version[location] = 1; // Initiate version to 1 if it equals 0

		return _id(location, _version[location]); // Return id of current location and version
	}

	void print() const{
		// Print a numbered list of garbage with slot versions (remove this, for testing only)
		for (size_t i = 0; i < _elementSize * _size; i++){
			if (i % _elementSize == 0)
				std::cout << "loc( " << i / _elementSize << " )\tver( " << _version[i / _elementSize] << " )\tval( ";

			std::cout << *(_buffer + i);

			if (i % _elementSize == _elementSize - 1)
				std::cout << " )\n";
		}
	}
};
