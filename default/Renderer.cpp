#include "Renderer.hpp"

#include "Engine.hpp"

#include <fstream>

bool Renderer::_loadShader(std::string shaderPath){
	std::fstream file;

	file.open((_shaderPath + shaderPath).c_str());

	if (!file)
		return false;

	std::string contents;

	while (!file.eof()){
		std::string line;
		std::getline(file, line);
		contents += line + "\n";
	}

	file.close();

	GLint shader = glCreateShader(GL_VERTEX_SHADER);

	const char* code = contents.c_str();

	glShaderSource(shader, 1, &code, 0);
	
	glCompileShader(shader);

	_shaders[shaderPath] = shader;

	return true;
}

Renderer::Renderer(Engine* engine) : System(engine){}

void Renderer::load(){
	_input = _engine.getSystem<Input>();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glewInit();

	_shaderPath = _engine.getConfig("data");

	if (_engine.getConfig("window.x") == "")
		_engine.setConfig("window.x", "720");

	if (_engine.getConfig("window.y") == "")
		_engine.setConfig("window.y", "405");

	if (_engine.getConfig("window.title") == "")
		_engine.setConfig("window.title", _engine.getConfig("name"));

	reset();
}

void Renderer::reset(){
	if (_window)
		SDL_DestroyWindow(_window);

	int x = std::stoi(_engine.getConfig("window.x"));
	int y = std::stoi(_engine.getConfig("window.y"));

	_window = SDL_CreateWindow(_engine.getConfig("window.title").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x, y, SDL_WINDOW_SHOWN);

	//_mainShader = glCreateProgram();
}

void Renderer::update(){
	if (!_window)
		return;

	if (_input && _input->isQuit()){
		_engine.shutdown();
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	_engine.manager.processEntities(this);
}

void Renderer::exit(){
	if (_window){
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}

void Renderer::onProcess(uint64_t id, Transform& script){
	// Render squares for each Transform
}
