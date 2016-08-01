#pragma once

#include "System.hpp"

#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>
#include <SDL_Scancode.h>
#include <SDL_scancode.h>
#include <glm\vec2.hpp>

class Input : public System<>{	
	bool _quit = false;

	std::unordered_map<std::string, unsigned int> _comboNames;
	std::vector<std::pair<SDL_Scancode, SDL_Scancode>> _keyCombos;

	std::vector<bool> _keyOnly;

	std::vector<bool> _modDown;
	std::vector<bool> _modPre;

	std::vector<bool> _keyDown;

	std::vector<bool> _comboDown;
	std::vector<bool> _comboChecked;

	unsigned int _insert(const std::string& name, std::pair<SDL_Scancode, SDL_Scancode> pair);

public:
	Input(Engine* engine);

	void load() override;
	void update() override;

	// If quit input event triggered (window closed)
	bool isQuit();

	// Binds string to key or key combination
	void addInput(const std::string& name, SDL_Scancode modifier, SDL_Scancode key);
	void addInput(const std::string& name, SDL_Scancode key);
	
	// Check if key was down like consumable event
	bool wasDown(const std::string& name);

	// Check if key is currently down
	bool isDown(const std::string& name);

	bool mouseWasDown(unsigned int button);
	bool mouseIsDown(unsigned int button);

	glm::vec2 mousePos();
	glm::vec2 mouseRelativePos();

	void lockMouse(bool lock = true);

	int scrollAmount();
};