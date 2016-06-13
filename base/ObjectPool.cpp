#include "ObjectPool.hpp"

BasePool::BasePool(uint32_t elementSize, uint32_t chunkSize) : _elementSize(elementSize), _chunkSize(chunkSize){
	_expand(1);
}

BasePool::~BasePool(){
	free(_buffer);
}