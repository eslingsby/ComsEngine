#include "Identification.hpp"

#include "Engine.hpp"

Identification::Identification(Engine* engine) : System(engine){}

void Identification::onCreate(uint64_t id){
	Identifier* identifier = _engine.manager.getComponent<Identifier>(id);

	if (identifier->name != "" && !hasName(identifier->name)){
		_names[identifier->name] = id;
		identifier->validName = true;
	}

	if (identifier->layer != ""){
		assert(_layers[identifier->layer].find(id) == _layers[identifier->layer].end());
		_layers[identifier->layer].insert(id);
	}
}

void Identification::onDestroy(uint64_t id){
	Identifier* identifier = _engine.manager.getComponent<Identifier>(id);

	if (identifier->name != "" && identifier->validName)
		_names.erase(identifier->name);

	if (identifier->layer != ""){
		_layers[identifier->layer].erase(id);

		if (!_layers[identifier->layer].size())
			_layers.erase(identifier->layer);
	}
}

uint64_t Identification::getByName(const std::string& name){
	assert(hasName(name));

	return _names[name];
}

uint64_t Identification::getByLayer(const std::string& layer, size_t number){
	assert(hasLayer(layer) > number);

	auto iter = _layers[layer].begin();

	if (number)
		std::advance(iter, number);

	return *iter;
}

bool Identification::hasName(const std::string& name){
	if (_names.find(name) == _names.end())
		return false;
	
	return true;
}

size_t Identification::hasLayer(const std::string& layer){
	if (_layers.find(layer) == _layers.end())
		return 0;

	return _layers[layer].size();
}