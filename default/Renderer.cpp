#include "Renderer.hpp"

#include "Engine.hpp"

#include <fstream>
#include <tiny_obj_loader.h>
#include <iostream>
#include <glm\mat4x4.hpp>

GLuint Renderer::_loadShader(std::string shaderPath, uint32_t type){
	std::fstream file;

	file.open((_shaderPath + shaderPath).c_str());

	if (!file){
		std::cout << "No shader file in " << _shaderPath + shaderPath << "\n";
		return 0;
	}

	std::string contents;

	while (!file.eof()){
		std::string line;
		std::getline(file, line);
		contents += line + "\n";
	}

	file.close();

	GLint shader = glCreateShader(type);

	const char* code = contents.c_str();

	glShaderSource(shader, 1, &code, 0);
		
	glCompileShader(shader);

	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success)
		return shader;

	int infoLogLength = 0;
	int maxLength = infoLogLength;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	char* infoLog = new char[maxLength];

	glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

	if (infoLogLength > 0)
		std::cout << infoLog << "\n";

	delete[] infoLog;

	return 0;
}

void Renderer::_reshape(int width, int height, float fov){
	//glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspectRatio = ((float)width / (float)height);

	gluPerspective(fov / aspectRatio, aspectRatio, 0.1, 1024 * 4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(90.f, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::Renderer(Engine* engine) : System(engine){
	_camera.linkManager(&_engine.manager);
}

void Renderer::load(){
	if (_engine.getConfig("window.x") == "")
		_engine.setConfig("window.x", "720");

	if (_engine.getConfig("window.y") == "")
		_engine.setConfig("window.y", "405");

	if (_engine.getConfig("window.title") == "")
		_engine.setConfig("window.title", _engine.getConfig("name"));

	if (_engine.getConfig("shader.vertex") == "")
		_engine.setConfig("shader.vertex", "Vertex.gls");

	if (_engine.getConfig("shader.fragment") == "")
		_engine.setConfig("shader.fragment", "Fragment.gls");

	_input = _engine.getSystem<Input>();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int windowWidth = std::stoi(_engine.getConfig("window.x"));
	int windowHeight = std::stoi(_engine.getConfig("window.y"));

	std::string windowTitle = _engine.getConfig("window.title");

	_shaderPath = _engine.getConfig("data");

	std::string vertexFile = _engine.getConfig("shader.vertex");
	std::string fragmentFile = _engine.getConfig("shader.fragment");

	if (_mainFShader)
		glDeleteShader(_mainFShader);

	if (_mainVShader)
		glDeleteShader(_mainVShader);

	if (_programId)
		glDeleteProgram(_programId);

	if (_context)
		SDL_GL_DeleteContext(_context);

	if (_window)
		SDL_DestroyWindow(_window);

	_window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	_context = SDL_GL_CreateContext(_window);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_ALPHA_TEST);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	assert(glewError == GLEW_OK);

	_programId = glCreateProgram();

	_mainVShader = _loadShader(vertexFile, GL_VERTEX_SHADER);

	glAttachShader(_programId, _mainVShader);

	_mainFShader = _loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	assert(_mainVShader && _mainFShader);

	glAttachShader(_programId, _mainFShader);

	glLinkProgram(_programId);

	_reshape(windowWidth, windowHeight);
}

void Renderer::reset(){
	_camera.invalidate();
}

void Renderer::update(){
	if (!_camera.valid()){
		_camera.create();
		_camera.addComponent<Transform>();
	}

	if (!_window)
		return;

	if (_input && _input->isQuit()){
		_engine.shutdown();
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Transform* transform = _camera.getComponent<Transform>();

	glMultMatrixf(&glm::mat4_cast(transform->rotation)[0][0]);

	glTranslatef(transform->position.x, transform->position.y, transform->position.z);

	glScalef(transform->scale.x, transform->scale.y, transform->scale.z);

	glUseProgram(_programId);

	_engine.manager.processEntities(this);

	glUseProgram(0);

	SDL_GL_SwapWindow(_window);
}

void Renderer::onProcess(uint64_t id, Transform& transform, Mesh& mesh){
	if (!mesh.loaded)
		return;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);

	// Loading vertex data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), 0);

	// Loading normal data
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 3 * sizeof(float), (void*)(mesh.vertexSize));

	// Loading texture data
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(float), (void*)(mesh.vertexSize + mesh.normalSize));

	GLint position = glGetAttribLocation(_programId, "in_position");
	GLint normal = glGetAttribLocation(_programId, "in_normal");
	GLint texcoord = glGetAttribLocation(_programId, "in_texcoord");

	if (position != -1){
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(position); // Positions
	}

	if (normal != -1){
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(mesh.vertexSize));
		glEnableVertexAttribArray(normal); // Normals
	}

	if (texcoord != -1){
		glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(mesh.vertexSize + mesh.normalSize));
		glEnableVertexAttribArray(texcoord); // Texcoords
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(transform.position.x, transform.position.y, transform.position.z);
	glMultMatrixf(&glm::mat4_cast(transform.rotation)[0][0]);
	glScalef(transform.scale.x, transform.scale.y, transform.scale.z);

	glDrawElements(GL_TRIANGLES, (int)mesh.indexSize, GL_UNSIGNED_INT, 0);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer::exit(){
	if (_window){
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}

void Renderer::onCreate(uint64_t id){
	Mesh* mesh = _engine.manager.getComponent<Mesh>(id);

	if (_meshes.find(mesh->source) != _meshes.end()){
		*mesh = _meshes[mesh->source];
		return;
	}

	assert(!mesh->loaded);

	// tinyobj containers
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string error;
	tinyobj::LoadObj(shapes, materials, error, (_shaderPath + mesh->source).c_str());

	if (!error.empty()){
		std::cout << error << "\n";
		return;
	}

	tinyobj::mesh_t obj = shapes[0].mesh;

	// Vertex buffer
	size_t vertexSize = obj.positions.size() * sizeof(float);
	size_t normalSize = obj.normals.size() * sizeof(float);
	size_t textureSize = obj.texcoords.size() * sizeof(float);
	size_t indexSize = obj.indices.size() * sizeof(unsigned int);

	// Setting up vertex buffer
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexSize + normalSize + textureSize, 0, GL_STATIC_DRAW);

	// Filling vertex buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &(obj.positions)[0]); // Positions

	if (normalSize)
		glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, &(obj.normals)[0]); // Normals

	if (textureSize)
		glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, textureSize, &(obj.texcoords)[0]); // Texcoords
	
	// Setting up index buffer
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, &(obj.indices[0]), GL_STATIC_DRAW);

	mesh->indexBuffer = indexBuffer;
	mesh->vertexBuffer = vertexBuffer;

	mesh->indexSize = indexSize;
	mesh->normalSize = normalSize;
	mesh->textureSize = textureSize;
	mesh->vertexSize = vertexSize;

	mesh->loaded = true;

	_meshes[mesh->source] = *mesh;
}

uint64_t Renderer::cameraId(){
	assert(_camera.valid());

	return _camera.id();
}

void Renderer::setCamera(uint64_t id){
	assert(_engine.manager.hasComponents<Transform>(id));

	_camera = id;
}

void Renderer::load(uint64_t id, std::string file){
	Mesh* mesh = _engine.manager.getComponent<Mesh>(id);

	assert(mesh);

	if (file == mesh->source)
		return;

	*mesh = Mesh(file);

	onCreate(id);
}
