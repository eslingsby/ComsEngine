#include "Renderer.hpp"

#include "Engine.hpp"

Renderer::Renderer(Engine* engine) : System(engine){}

void Renderer::load(){
	_input = _engine.getSystem<Input>();

	assert(_input);

	SDL_Init(SDL_INIT_VIDEO);

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
}

void Renderer::update(){
	if (!_window)
		return;

	if (_input->isQuit()){
		SDL_DestroyWindow(_window);
		_window = nullptr;

		_engine.shutdown();
		return;
	}

	SDL_UpdateWindowSurface(_window);

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
