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

// Renderer system. Subsribes to entities with Transform and Mesh components.
// Loads mesh using TinyObjLoader and OpenGL.
// Creates and updates OpenGL window.
// Renders entities with a mesh component.
// Handles a camera entity with a transform used as OpenGL camera, that can be assigned publicly.

class Renderer : public System<Transform, Mesh>{
	SDL_Window* _window = nullptr;
	SDL_GLContext _context = nullptr;

	GLuint _programId = 0;
	GLuint _mainVShader = 0;
	GLuint _mainFShader = 0;

	std::string _shaderPath;

	Input* _input = nullptr;

	std::unordered_map<std::string, Mesh> _meshes;

	GLuint _loadShader(std::string shaderPath, uint32_t type);

	void _reshape(int width, int height, float fov = 90.f, int drawDist = 1024 * 8);

	EntityRef _camera;

public:
	Renderer(Engine* engine);

	void load() override;
	void reset() override;
	void update() override;
	void exit() override;

	// Render all meshes
	void onProcess(uint64_t id, Transform& transform, Mesh& mesh) override;

	// Load mesh
	void onCreate(uint64_t id) override;

	// Return camera entity ID
	uint64_t cameraId();

	// Set camera entity
	void setCamera(uint64_t id);

	// Reload mesh file on entity
	void load(uint64_t id, std::string file);
};