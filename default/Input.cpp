#include "Input.hpp"

#include "Engine.hpp"

#include <SDL.h>

unsigned int Input::_insert(const std::string& name, std::pair<SDL_Scancode, SDL_Scancode> pair){
	unsigned int index = (unsigned int)_keyCombos.size();

	_keyCombos.resize(index + 1);
	_modDown.resize(index + 1);
	_modPre.resize(index + 1);
	_keyDown.resize(index + 1);
	_comboDown.resize(index + 1);
	_comboChecked.resize(index + 1);
	_keyOnly.resize(index + 1);

	_keyCombos[index] = pair;

	_comboNames[name] = index;

	return index;
}

Input::Input(Engine* engine) : System(engine){}

void Input::load(){
	SDL_Init(SDL_INIT_EVENTS);
}

bool Input::isQuit(){
	if (_quit){
		_quit = false;
		return true;
	}

	return false;
}

void Input::update(){
	SDL_Event e;

	while (SDL_PollEvent(&e)){
		if (e.type == SDL_QUIT){
			_quit = true;
			continue;
		}

		if (e.type != SDL_KEYDOWN && e.type != SDL_KEYUP)
			continue;

		for (unsigned int i = 0; i < _keyCombos.size(); i++){
			if (e.type == SDL_KEYDOWN){
				if (e.key.keysym.scancode == _keyCombos[i].first){
					if (_keyDown[i])
						_modPre[i] = true;

					_modDown[i] = true;
				}
				else if (e.key.keysym.scancode == _keyCombos[i].second){
					_keyDown[i] = true;
				}
			}
			else if (e.type == SDL_KEYUP){
				if (e.key.keysym.scancode == _keyCombos[i].first ){
					_modDown[i] = false;
				}
				else if (e.key.keysym.scancode == _keyCombos[i].second){
					_keyDown[i] = false;
					_modPre[i] = false;
				}
			}

			if (((_modDown[i] && !_modPre[i]) || _keyOnly[i]) && _keyDown[i] && !_comboDown[i]){
				_comboDown[i] = true;
				_comboChecked[i] = true;
			}
			else if (_comboDown[i] && !_keyDown[i]){
				_comboDown[i] = false;
			}
		}
	}
}

void Input::addInput(const std::string& name, SDL_Scancode modifier, SDL_Scancode key){
	// search _keyCombos for pair(modifier, key)

	// if pair not in _keyCombos then resize vectors and add

	_insert(name, std::make_pair(modifier, key));

	// set _comboNames[name] to false and vector index
}

void Input::addInput(const std::string& name, SDL_Scancode key){
	unsigned int index = _insert(name, std::make_pair((SDL_Scancode)0, key));

	_keyOnly[index] = true;
}

bool Input::wasDown(const std::string& name){
	assert(_comboNames.find(name) != _comboNames.end());

	unsigned int index = _comboNames[name];

	if (_comboChecked[index]){
		_comboChecked[index] = false;
		return true;
	}

	return false;

	// check _comboNames for name

	// get vector index from _comboNames

	// if true at _comboChecked[index] set false then return true

	// else return false
}

bool Input::isDown(const std::string & name){
	assert(_comboNames.find(name) != _comboNames.end());

	unsigned int index = _comboNames[name];

	return _comboDown[index];
}

glm::vec2 Input::mousePos(){
	int x, y;
	SDL_GetMouseState(&x, &y);

	return glm::vec2(x, y);
}

glm::vec2 Input::mouseRelativePos(){
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);

	return glm::vec2(x, y);
}

void Input::lockMouse(bool lock){
	if (lock)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

bool Input::mouseWasDown(unsigned int button){
	return false;
}

bool Input::mouseIsDown(unsigned int button){
	return false;
}

int Input::scrollAmount(){
	return 0;
}
