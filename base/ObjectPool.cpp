#include "ObjectPool.hpp"

#include <iostream>

uint8_t* ObjectPool::_conv(void* memory) const{
	assert(memory != nullptr); 
	return (uint8_t*)memory;
}

ObjectPool::ObjectPool(uint32_t elementSize, uint32_t chunkSize) : _elementSize(elementSize), _chunkSize(chunkSize){
	_expand(); // Initially start with 1 chunk of memory
}

ObjectPool::~ObjectPool(){
	free(_buffer);
}

void ObjectPool::print() const{
	// Print a numbered list of garbage with slot versions (remove this, testing only)
	for (size_t i = 0; i < _elementSize * _size; i++){
		if (i % _elementSize == 0)
			std::cout << "loc( " << i / _elementSize << " )\tver( " << _version[i / _elementSize] << " )\tval( ";

		std::cout << *(_buffer + i);

		if (i % _elementSize == _elementSize - 1)
			std::cout << " )\n";
	}
}