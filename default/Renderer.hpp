#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Input.hpp"

#include <SDL.h>

class Renderer : public System<Transform>{
	SDL_Window* _window = nullptr;
	Input* _input = nullptr;

public:
	Renderer(Engine* engine);

	void load() override;
	void reset() override;
	void update() override;
	void exit() override;

	void onProcess(uint64_t id, Transform& script) override;
};