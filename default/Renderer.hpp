#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Input.hpp"

#include <unordered_map>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

class Renderer : public System<Transform>{
	SDL_Window* _window = nullptr;
	SDL_GLContext _context = nullptr;

	GLuint _mainShader = 0;

	std::string _shaderPath;

	Input* _input = nullptr;

	std::unordered_map<std::string, GLuint> _shaders;

	bool _loadShader(std::string shaderPath);

public:
	Renderer(Engine* engine);

	void load() override;
	void reset() override;
	void update() override;
	void exit() override;

	void onProcess(uint64_t id, Transform& script) override;
};