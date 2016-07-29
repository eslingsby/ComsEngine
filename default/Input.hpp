#pragma once

#include "System.hpp"

#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>
#include <SDL_keycode.h>

class Input : public System<>{	
	bool _quit = false;

	std::unordered_map<std::string, unsigned int> _comboNames;
	std::vector<std::pair<SDL_Keycode, SDL_Keycode>> _keyCombos;

	std::vector<bool> _keyOnly;

	std::vector<bool> _modDown;
	std::vector<bool> _modPre;

	std::vector<bool> _keyDown;

	std::vector<bool> _comboDown;
	std::vector<bool> _comboChecked;

	unsigned int _insert(const std::string& name, std::pair<SDL_Keycode, SDL_Keycode> pair);

public:
	Input(Engine* engine);

	void load() override;
	void update() override;

	// If quit input event triggered (window closed)
	bool isQuit();

	// Binds string to key or key combination
	void addInput(const std::string& name, SDL_Keycode modifier, SDL_Keycode key);
	void addInput(const std::string& name, SDL_Keycode key);
	
	// Check if key was down like consumable event
	bool wasDown(const std::string& name);

	// Check if key is currently down
	bool isDown(const std::string& name);
};