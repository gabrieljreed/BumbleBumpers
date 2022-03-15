// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <windows.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "Bumble Bumpers", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------ TESTING ------------------------------------------------

    TCHAR Buffer[MAX_PATH];
    DWORD dwRet;

    dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
    cout << dwRet << endl;

    // ------------------------------------------------ MODELS ------------------------------------------------

    // Read objects 
    mlModel model;
    if (!LoadModel("../Models", "Giraffe.obj", model)) return -1;

    // Store data from objects 
    vector<mlVertex> verts;
    vector<GLuint> indices;

    verts = model.meshes[0].vertices;
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

    // ------------------------------------------------ SHADERS ------------------------------------------------
    // Import shader code
    GLuint myShader = LoadProgram("C:/Users/gabri/Desktop/455/Glitter/Glitter/Shaders/basic.vert", "C:/Users/gabri/Desktop/455/Glitter/Glitter/Shaders/basic.frag");

    GLuint VAO, EBO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER,
        verts.size() * sizeof(mlVertex),
        &verts[0],
        GL_STATIC_DRAW);

    // Position attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

	
    // ------------------------------------------------ RENDERING LOOP ------------------------------------------------

    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// **********************************
		// Add rendering code here

		// **********************************

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}
