#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Input.hpp"
#include "EntityRef.hpp"

#include <unordered_map>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

class Renderer : public System<Transform, Mesh>{
	SDL_Window* _window = nullptr;
	SDL_GLContext _context = nullptr;

	GLuint _programId = 0;
	GLuint _mainVShader = 0;
	GLuint _mainFShader = 0;

	std::string _shaderPath;

	Input* _input = nullptr;

	GLuint _loadShader(std::string shaderPath, uint32_t type);

	void _reshape(int width, int height, float fov = 90.f);

	EntityRef _camera;

public:
	Renderer(Engine* engine);

	void load() override;
	void reset() override;
	void update() override;
	void exit() override;

	void onProcess(uint64_t id, Transform& transform, Mesh& mesh) override;

	void onCreate(uint64_t id) override;

	uint64_t cameraId();

	void setCamera(uint64_t id);
};