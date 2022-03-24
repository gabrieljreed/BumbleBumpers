#pragma once

#include <vector>
#include <iostream>

#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "Shader.h"

using namespace std;

class MeshModel {
public: 

	vector<mlVertex> vertices;
	vector<GLuint> indices;

    GLuint VAO, EBO, VBO;

    MeshModel(string ObjectName) {
        setupModel(ObjectName);
    }

	int setupModel(string ObjectName) {
        // ------------------------------------------------ MESH ------------------------------------------------
        mlModel model;
        string path = ObjectName + ".obj";
		if (!LoadModel("../Models", path, model)) return -1;

		// Store data from objects 
		vertices = model.meshes[0].vertices;
		indices = model.meshes[0].indices;

        // ------------------------------------------------ TEXTURES ------------------------------------------------
        // Genereate texture ID 
        unsigned int texture;
        glGenTextures(1, &texture);

        // Bind texture 
        glBindTexture(GL_TEXTURE_2D, texture);

        // Texture wrapping parameters 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Read in textures
        int width, height, channels;
        unsigned char* imageData = stbi_load("../Textures/orangeCrayon.png", &width, &height, &channels, 3);
        if (!imageData) return 0;

        if (imageData) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        }
        else {
            cout << "Failed to load texture" << endl;
        }

        // ------------------------------------------------ SHADER ------------------------------------------------

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(mlVertex),
            &vertices[0],
            GL_STATIC_DRAW);

        // Position attribute 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glEnable(GL_DEPTH_TEST);


		return 0;
	}

    void Draw(MeshShader& shader) {  // FIXME: Might not need to pass in the shader 
        glBindVertexArray(VAO);                                                     // Bind VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);           // Draw elements 
        glBindVertexArray(0);                                                       // Unbind VAO 
    }

    void translate();

    void rotate();

    void scale();
};

