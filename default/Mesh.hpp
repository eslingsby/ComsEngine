#pragma once

#include "Component.hpp"

#include <gl\glew.h>
#include <gl\glu.h>

struct Mesh : public Component<Mesh>{
	char source[128];

	bool loaded = false;

	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;

	size_t indexSize = 0;
	size_t vertexSize = 0;
	size_t textureSize = 0;
	size_t normalSize = 0;

	Mesh(const std::string& source = ""){
		strcpy_s(Mesh::source, source.c_str());
	}

	void operator=(const Mesh& mesh){
		loaded = mesh.loaded;
		vertexBuffer = mesh.vertexBuffer;
		indexBuffer = mesh.indexBuffer;
		indexSize = mesh.indexSize;
		vertexSize = mesh.vertexSize;
		textureSize = mesh.textureSize;
		normalSize = mesh.normalSize;
		strcpy_s(source, mesh.source);
	}
};