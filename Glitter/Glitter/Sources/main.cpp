// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "MeshModel.h"
#include "Shader.h"
#include "InputHandler.h"


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
#include <map>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <WindowHandler.h>

using namespace std;

int main(int argc, char * argv[]) {

    auto mWindow = windowSetup();

    // ------------------------------------------------ DEFINE SCENE  ------------------------------------------------

    MeshShader sceneShader = MeshShader();

    map<string, MeshModel> objects;
    map<string, MeshModel>::iterator iter;

    MeshModel crayon("TestCrayon", "orangeCrayon.png", glm::vec3(0.0f, 0.05f, -10.0f));
    objects.insert({ "OrangeCrayon", crayon });

    MeshModel crayon2("TestCrayon", "blueCrayon.png", glm::vec3(2.0f, 0.05f, -10.0f));
    objects.insert({ "BlueCrayon", crayon2 });

    MeshModel floor("Floor", "blueCrayon.png");
    objects.insert({ "Floor", floor });

    MeshModel giraffe("Giraffe_Triangles", "Giraffe.png"); 
    giraffe.rotate(180, glm::vec3(1, 0, 0)); // FIXME: Rotation is weird 
    objects.insert({ "Giraffe", giraffe });

    sceneShader.use();

    // Perspective matrix - this doesn't change, so we don't need to set it on every frame  
    glm::mat4 persp = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearClip, farClip);
    sceneShader.setMat4("persp", persp);
	
    // ------------------------------------------------ RENDERING LOOP ------------------------------------------------

    while (glfwWindowShouldClose(mWindow) == false) {
        // Handle user input 
        handleKeypress(mWindow);

        if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS) {
            objects.at("OrangeCrayon").launch(glm::vec3(0, -1, 0), 4);
        }

        // Background Fill Color/Clear each frame 
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix 
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        sceneShader.setMat4("view", view);

        //// Animation 
        //meshes[0].pace(0.1, 5, glm::vec3(0.0f, 0.05f, -10.0f), 'x');
        //meshes[1].pace(0.2, 12, glm::vec3(2.0f, 0.05f, -10.0f), 'z');

        for (iter = objects.begin(); iter != objects.end(); ++iter) {
            iter->second.Draw(sceneShader);
        }

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}
