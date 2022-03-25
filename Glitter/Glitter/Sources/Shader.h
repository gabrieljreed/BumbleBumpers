#pragma once

#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "Shader.h"

#include <string>

using namespace std;

class MeshShader {
public:
	GLuint ID;

	MeshShader() {
		ID = LoadProgram("../Glitter/Shaders/basic.vert", "../Glitter/Shaders/basic.frag");
	}

	MeshShader(const char* vertexPath, const char* fragmentPath) {
		ID = LoadProgram(vertexPath, fragmentPath);
	}

	void use() {
		glUseProgram(ID);
	}

	// Uniform setting functions 
	void setMat4(const string& name, glm::mat4 matrix) {
		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
};

