#pragma once

#include "Component.hpp"

#include <gl\glew.h>
#include <gl\glu.h>
#include <string>

struct Mesh : public Component<Mesh>{
	std::string source;

	bool loaded = false;

	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;

	size_t indexSize = 0;
	size_t vertexSize = 0;
	size_t textureSize = 0;
	size_t normalSize = 0;

	Mesh(const std::string& source = "") : source(source){}

	void operator=(const Mesh& mesh){
		loaded = mesh.loaded;
		vertexBuffer = mesh.vertexBuffer;
		indexBuffer = mesh.indexBuffer;
		indexSize = mesh.indexSize;
		vertexSize = mesh.vertexSize;
		textureSize = mesh.textureSize;
		normalSize = mesh.normalSize;
		source = mesh.source;
	}
};