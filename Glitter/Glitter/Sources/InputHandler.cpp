#include "InputHandler.h"
#include "Gamemode.h"
#include <iostream>

using namespace std;

void handleKeypress(GLFWwindow* window) {
    // Esc to quit 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        paused = false;
        if (!gameStarted) {
            gameStarted = true;
            startTime = static_cast<float>(glfwGetTime());
        }
    }

    if (!paused) {
        // WSAD movement 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPosition += cameraSpeed * cameraLookAt;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraSpeed * cameraLookAt;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPosition += glm::normalize(glm::cross(cameraLookAt, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition -= glm::normalize(glm::cross(cameraLookAt, cameraUp)) * cameraSpeed;

        if (debugMode) {
            // E and Q for up/down movement 
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
                cameraPosition -= cameraUp * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                cameraPosition += cameraUp * cameraSpeed;
        }
    }
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
    if (debugMode) {
        pitch += yOffset;
    }
    
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