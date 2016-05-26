#include "ObjectPool.hpp"

#ifdef _DEBUG
#include <iostream>
#endif

ObjectPool::ObjectPool(uint32_t elementSize, uint32_t chunkSize) : _elementSize(elementSize), _chunkSize(chunkSize){
	_expand(1);
}

ObjectPool::~ObjectPool(){
	free(_buffer);
}

void ObjectPool::print() const{
#ifdef _DEBUG
	for (size_t i = 0; i < _elementSize * _size; i++){
		if (i % _elementSize == 0)
			std::cout << "loc( " << i / _elementSize << " )\tocc( " << ((_occupied[i / _elementSize] == 0xFF) ? "true" : "false") << " )\tval( ";

		std::cout << *(_buffer + i);

		if (i % _elementSize == _elementSize - 1)
			std::cout << " )\n";
	}
#endif
}