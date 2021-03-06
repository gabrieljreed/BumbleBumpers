#pragma once

#include <vector>
#include <iostream>
#include <math.h>

#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "Shader.h"

# define pi           3.14159265358979323846

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

    unsigned int texture;

    //Bounding Box parameters
    float _maxX, _minX, _maxZ, _minZ;
    float distNegX, distPosX, distNegZ, distPosZ;

    // Texture parameters 
    unsigned char* imageData;
    int width, height;

    // Object params 
    glm::vec3 position;
    glm::vec3 objectScale;

    // Movement parameters 
    float direction;
    float pacePosition;

    // Launch params 
    bool launching;
    glm::vec3 launchDirection;
    float launchSpeed;
    

    MeshModel(string ObjectName, string TextureName) {
        setupModel(ObjectName, TextureName);

        //transform = glm::mat4(1.0f);
        translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        rotateMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1, 0, 0));
        scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));

        rotate(180, glm::vec3(1, 0, 0));

        position = glm::vec3(0, 0, 0);
        objectScale = glm::vec3(1, 1, 1);

        /*distPosX = _maxX - position.x;
        distNegX = position.x - _minX;
        distPosZ = _maxZ - position.z;
        distNegZ = position.x - _minZ;*/

        direction = 1;
        pacePosition = 0.0f;

        launching = false;
    }

    MeshModel(string ObjectName, string TextureName, glm::vec3 initPosition) {
        setupModel(ObjectName, TextureName);

        //transform = glm::mat4(1.0f);
        translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        rotateMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1, 0, 0));
        scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));

        direction = 1;
        pacePosition = 0.0f;
        position = initPosition;

        /*distPosX = _maxX - position.x;
        distNegX = position.x - _minX;
        distPosZ = _maxZ - position.z;
        distNegZ = position.x - _minZ;*/

        launching = false;

        translate(position);
    }

	int setupModel(string ObjectName, string TextureName) {
        // ------------------------------------------------ MESH ------------------------------------------------
        mlModel model;
        string path = ObjectName + ".obj";
		if (!LoadModel("../Models", path, model)) return -1;

		// Store data from objects 
        for (unsigned int i = 0; i < model.meshes.size(); i++) {
            vertices.insert(vertices.end(), model.meshes[i].vertices.begin(), model.meshes[i].vertices.end());
            indices.insert(indices.end(), model.meshes[i].indices.begin(), model.meshes[i].indices.end());
        }

        // ------------------------------------------------ BOUNDING BOX ------------------------------------------------

        float maxX = -1 * std::numeric_limits<float>::infinity();;
        float minX = std::numeric_limits<float>::infinity();;
        float maxZ = -1 * std::numeric_limits<float>::infinity();;
        float minZ = std::numeric_limits<float>::infinity();;
        for (unsigned int i = 0; i < vertices.size(); i++) {
            if (vertices[i].position[0] > maxX) {
                maxX = vertices[i].position[0];
            }
            else if (vertices[i].position[0] < minX) {
                minX = vertices[i].position[0];
            }

            if (vertices[i].position[2] > maxZ) {
                maxZ = vertices[i].position[2];
            }
            else if (vertices[i].position[2] < minZ) {
                minZ = vertices[i].position[2];
            }
        }

        _maxX = maxX;
        _minX = minX;
        _maxZ = maxZ;
        _minZ = minZ;

        // ------------------------------------------------ TEXTURES ------------------------------------------------
        // Genereate texture ID 
        
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

        // Normal attribute 
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

		return 0;
	}

    void Draw(MeshShader& shader) {
        shader.use();
        glBindVertexArray(VAO);                                                     // Bind VAO
        
        if (imageData) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        }

        //// Bind texture 
        //glActiveTexture(0);
        //glBindTexture(GL_TEXTURE_2D, texture);

        transform = translateMat * rotateMat * scaleMat;
        shader.setMat4("transform", transform);

        if (launching) {
            translate(launchDirection * launchSpeed);
        }
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);           // Draw elements 
        glBindVertexArray(0);                                                       // Unbind VAO 
    }

    void translate(glm::vec3 translateAmount) {
        position += translateAmount;
        _maxX += translateAmount[0];
        _minX += translateAmount[0];
        _maxZ += translateAmount[2];
        _minZ += translateAmount[2];
        translateMat = glm::translate(glm::mat4(1.0f), position);
    }

    void translate(const float& x, const float& y, const float& z) {
        glm::vec3 translateAmount = glm::vec3(x, y, z);
        translate(translateAmount);
    }

    void translateAbsolute(const float& x, const float& y, const float& z) {
        glm::vec3 newPosition = glm::vec3(x, y, z);
        glm::vec3 translateAmount = newPosition - position;

        _maxX += translateAmount[0];
        _minX += translateAmount[0];
        _maxZ += translateAmount[2];
        _minZ += translateAmount[2];

        position.x = x;
        position.y = y;
        position.z = z;
        translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    }

    void translateAbsolute(const glm::vec3& newPosition) {
        glm::vec3 translateAmount = newPosition - position;

        _maxX += translateAmount[0];
        _minX += translateAmount[0];
        _maxZ += translateAmount[2];
        _minZ += translateAmount[2];
        position = newPosition;
        translateMat = glm::translate(glm::mat4(1.0f), newPosition);
    }

    void rotate(float degrees, glm::vec3 direction) {
        /*float angle1 = atan2(_maxZ, _maxX);
        float distance1 = sqrt(_maxX * _maxX + _maxZ * _maxZ);
        //angle1 += rotation;
        float rotatedX1 = sin(angle1) * distance1;
        float rotatedZ1 = cos(angle1) * distance1;*/
        rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), direction);
        glm::vec4 x1 = glm::vec4(_maxX, 0, 0, 1);
        glm::vec4 x2 = glm::vec4(_minX, 0, 0, 1);
        glm::vec4 z1 = glm::vec4(0, 0, _maxZ, 1);
        glm::vec4 z2 = glm::vec4(0, 0, _minZ, 1);
        _maxX = (x1 * rotateMat)[0];
        _minX = (x2 * rotateMat)[0];
        _maxZ = (z1 * rotateMat)[2];
        _minZ = (z2 * rotateMat)[2];
        //printf("yo");
    }

    void scale(glm::vec3 scaleAmount) {
        objectScale *= scaleAmount;
        _maxX *= scaleAmount[0];
        _minX *= scaleAmount[0];
        _maxZ *= scaleAmount[2];
        _minZ *= scaleAmount[2];
        scaleMat = glm::scale(glm::mat4(1.0f), objectScale);
    }

    void scale(float scaleAmount) {
        objectScale *= scaleAmount;
        _maxX *= scaleAmount;
        _minX *= scaleAmount;
        _maxZ *= scaleAmount;
        _minZ *= scaleAmount;
        scaleMat = glm::scale(glm::mat4(1.0f), objectScale);
    }

    void scale(const float& x, const float& y, const float& z) {
        glm::vec3 scaleAmount = glm::vec3(x, y, z);
        scale(scaleAmount);
    }

    void resetTransformations() {
        position = glm::vec3(0, 0, 0);
        objectScale = glm::vec3(1, 1, 1);
        rotate(180, glm::vec3(1, 0, 0));
    }

    void pace(float speed, float range, glm::vec3 axis) {
        glm::vec3 translateAxis = glm::vec3(speed) * axis;

        translateAxis = direction * translateAxis;

        pacePosition += speed * direction;

        if (pacePosition > range) {
            // Switch directions 
            direction = -1;
        }
        else if (pacePosition < -range) {
            direction = 1;
        }

        translate(translateAxis);
    }

    void launch(glm::vec3 direction, float speed) {
        launching = true;
        launchDirection = direction;
        launchSpeed = speed;
    }
};

