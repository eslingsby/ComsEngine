#pragma once

#include "Component.hpp"

#include <gl\glew.h>
#include <gl\glu.h>
#include <string>

struct Mesh : public Component<Mesh>{
	const std::string source;

	bool loaded = false;

	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;

	size_t indexSize = 0;
	size_t vertexSize = 0;
	size_t textureSize = 0;
	size_t normalSize = 0;

	Mesh(const std::string& source) : source(source){}
};