// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "MeshModel.h"
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

    MeshShader sceneShader = MeshShader();

    MeshModel crayon("TestCrayon", "orangeCrayon.png");
    crayon.translate(glm::vec3(0.0f, 0.05f, -10.0f));

    MeshModel crayon2("TestCrayon", "orangeCrayon.png");
    crayon2.translate(glm::vec3(2.0f, 0.05f, -10.0f));

    MeshModel floor("Floor", "orangeCrayon.png");

    sceneShader.use();

    // Perspective matrix - this doesn't change, so we don't need to set it on every frame  
    glm::mat4 persp = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearClip, farClip);
    sceneShader.setMat4("persp", persp);
	
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
        sceneShader.setMat4("view", view);

        crayon.Draw(sceneShader);
        crayon2.Draw(sceneShader);
        //floor.Draw(sceneShader);

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

    // E and Q for up/down movement 
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPosition -= cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPosition += cameraUp * cameraSpeed;
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

    yaw -= xOffset;
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
