#pragma once

#include "System.hpp"

#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>
#include <SDL_Scancode.h>
#include <SDL_scancode.h>
#include <glm\vec2.hpp>

// Input system. Subsribes to no components.
// Used to bind keyboard input to string identifiers.
// Use string identifiers to check for key presses.
// Also checks mouse and window input.

class Input : public System<>{	
	bool _quit = false;

	bool _mouseLDown = false;
	bool _mouseLWasDown = false;

	bool _mouseRDown = false;
	bool _mouseRWasDown = false;

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

	bool mouseLWasDown();
	bool mouseLIsDown();

	bool mouseRWasDown();
	bool mouseRIsDown();

	glm::vec2 mousePos();
	glm::vec2 mouseRelativePos();

	// Lock and hide mouse on window
	void lockMouse(bool lock = true);

	// Scroll amount (not working)
	int scrollAmount();
};