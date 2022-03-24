// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Shader.h"


// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void handleKeypress(GLFWwindow* window);

void handleMouse(GLFWwindow* window, double xPos, double yPos);

// ------------------------------------------------ CAMERA SETTINGS ------------------------------------------------
int windowWidth = mWidth;
int windowHeight = mHeight;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraLookAt   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp       = glm::vec3(0.0f, 1.0f, 0.0f);

const float cameraSpeed = 0.1f;
const float FOV = 80.0f;
const float nearClip = 0.001f;
const float farClip = 10000.0f;

// ------------------------------------------------ MOUSE SETTINGS ------------------------------------------------
bool firstMouse = true;
float lastX = windowWidth / 2;
float lastY = windowHeight / 2;
float pitch = 0.0f;
float yaw = -90.0f;

const float mouseSensitivity = 0.1f;


int main(int argc, char * argv[]) {
    
    // ------------------------------------------------ WINDOW SETTINGS ------------------------------------------------
    // Load GLFW and Create a Window

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(windowWidth, windowHeight, "Bumble Bumpers", nullptr, nullptr);

    
    //glfwGetWindowSize(mWindow, &windowWidth, &windowHeight);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Window mouse settings 
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, handleMouse);

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);

    Shader crayonShader("../Glitter/Shaders/basic.vert", "../Glitter/Shaders/basic.frag");

    Model crayon();

    crayonShader.use();

    // ------------------------------------------------ MODELS ------------------------------------------------

    // Read objects 
    //mlModel model;
    //if (!LoadModel("../Models", "TestCrayon.obj", model)) return -1;

    //// Store data from objects 
    //vector<mlVertex> verts;
    //vector<GLuint> indices;

    //verts = model.meshes[0].vertices;
    //indices = model.meshes[0].indices;


    //mlModel model2;
    //if (!LoadModel("../Models", "Floor.obj", model2)) return -1;

    //// Store data from objects 
    //vector<mlVertex> verts2;
    //vector<GLuint> indices2;

    //verts2 = model2.meshes[0].vertices;
    //indices2 = model2.meshes[0].indices;

    // ------------------------------------------------ TEXTURES ------------------------------------------------

    // Genereate texture ID 
    //unsigned int texture;
    //glGenTextures(1, &texture);

    //// Bind texture 
    //glBindTexture(GL_TEXTURE_2D, texture);

    //// Texture wrapping parameters 
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //// Read in textures
    //int width, height, channels;
    //unsigned char* imageData = stbi_load("../Textures/orangeCrayon.png", &width, &height, &channels, 3);
    //if (!imageData) return 0;

    //if (imageData) {
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    //}
    //else {
    //    cout << "Failed to load texture" << endl;
    //}

    // ------------------------------------------------ SHADERS ------------------------------------------------
    // Import shader code
    //GLuint myShader = LoadProgram("../Glitter/Shaders/basic.vert", "../Glitter/Shaders/basic.frag");

    //GLuint VAO, EBO, VBO;
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &EBO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    //glBufferData(GL_ARRAY_BUFFER,
    //    verts.size() * sizeof(mlVertex),
    //    &verts[0],
    //    GL_STATIC_DRAW);

    //// Position attribute 
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// TexCoord attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glEnable(GL_DEPTH_TEST);

    //glUseProgram(myShader);

    // Perspective matrix - this doesn't change, so we don't need to set it on every frame  
    glm::mat4 persp = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearClip, farClip);
    crayonShader.setMat4("persp", persp);
    /*GLint perspLoc = glGetUniformLocation(crayonShader.ID, "persp");
    glUniformMatrix4fv(perspLoc, 1, GL_FALSE, glm::value_ptr(persp));*/
	
    // ------------------------------------------------ RENDERING LOOP ------------------------------------------------

    while (glfwWindowShouldClose(mWindow) == false) {
        // Handle user input 
        handleKeypress(mWindow);

        // Background Fill Color/Clear each frame 
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix 
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        crayonShader.setMat4("view", view);
        GLint viewLoc = glGetUniformLocation(myShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Transformation matrix 
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.05f, -10.0f));
        transform = glm::rotate(transform, 3.14f, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, 3.14f / 4, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
        GLint transformLoc = glGetUniformLocation(myShader, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);                                                     // Bind VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);           // Draw elements 

        glBindVertexArray(0);                                                       // Unbind VAO (not 100% necessary here because there's only one VAO)

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}

void handleKeypress(GLFWwindow* window) {
    // Esc to quit 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // WSAD movement 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraLookAt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraLookAt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraLookAt, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraLookAt, cameraUp)) * cameraSpeed;
}

void handleMouse(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;
    lastX = xPos;
    lastY = yPos;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Clamp values so they don't go over 
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraLookAt = glm::normalize(direction);
}
