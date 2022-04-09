#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "glitter.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ------------------------------------------------ CAMERA SETTINGS ------------------------------------------------
extern int windowWidth = mWidth;
extern int windowHeight = mHeight;

glm::vec3 cameraPosition = glm::vec3(0.0f, -2.0f, 5.0f);
glm::vec3 cameraLookAt = glm::vec3(0.0f, -2.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


const float cameraSpeed = 0.3f;
const float FOV = 80.0f;
const float nearClip = 0.001f;
const float farClip = 10000.0f;

// ------------------------------------------------ MOUSE SETTINGS ------------------------------------------------
extern bool firstMouse = true;
float lastX = windowWidth / 2;
float lastY = windowHeight / 2;
float pitch = 0.0f;
float yaw = -90.0f;

const float mouseSensitivity = 0.1f;

void handleKeypress(GLFWwindow* window);

void handleMouse(GLFWwindow* window, double xPos, double yPos);

#endif