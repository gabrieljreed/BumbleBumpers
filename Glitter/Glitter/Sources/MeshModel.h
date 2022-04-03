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

    glm::mat4 transform;

    glm::mat4 translateMat;
    glm::mat4 rotateMat; 
    glm::mat4 scaleMat;

    // Texture parameters 
    unsigned char* imageData;
    int width, height;

    bool direction;
    float pacePosition;

    MeshModel(string ObjectName, string TextureName) {
        setupModel(ObjectName, TextureName);

        //transform = glm::mat4(1.0f);
        translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        rotateMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1, 0, 0));
        scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));

        direction = true;
        pacePosition = 0.0f;
    }

	int setupModel(string ObjectName, string TextureName) {
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
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Texture wrapping parameters 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Read in textures
        int channels;
        string texturePath = "../Textures/" + TextureName;
        imageData = stbi_load(texturePath.c_str(), &width, &height, &channels, 3);
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

        


		return 0;
	}

    void Draw(MeshShader& shader) {
        if (imageData) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        }

        transform = translateMat * rotateMat * scaleMat;
        shader.setMat4("transform", transform);

        glBindVertexArray(VAO);                                                     // Bind VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);           // Draw elements 
        glBindVertexArray(0);                                                       // Unbind VAO 
    }

    void translate(glm::vec3 translateAmount) {
        translateMat = glm::translate(glm::mat4(1.0f), translateAmount);
    }

    void rotate(float degrees, glm::vec3 direction) {
        rotateMat = glm::rotate(glm::mat4(1.0f), degrees, direction);
    }

    void scale(glm::vec3 scaleAmount) {
        scaleMat = glm::scale(glm::mat4(1.0f), scaleAmount);
    }

    void pace(float speed, float range, glm::vec3 center, char axis) {
        if (direction) {
            pacePosition += speed;
        }
        else {
            pacePosition -= speed;
        }

        if (pacePosition > range) {
            direction = false;
        }
        else if (pacePosition < -range) {
            direction = true;
        }

        glm::vec3 translateAxis;

        if (axis == 'x') {
            translateAxis = glm::vec3(pacePosition, 0, 0);
        }
        else if (axis == 'y') {
            translateAxis = glm::vec3(0, pacePosition, 0);
        }
        else if (axis == 'z') {
            translateAxis = glm::vec3(0, 0, pacePosition);
        }
        else {
            translateAxis = glm::vec3(0, 0, 0);
            cout << "ERROR - INVALID AXIS" << endl; 
        }

        glm::vec3 translateAmount = center + translateAxis;

        translate(translateAmount);
    }
};

