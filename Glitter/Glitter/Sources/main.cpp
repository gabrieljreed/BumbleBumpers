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

    vector<MeshModel> meshes;

    MeshModel crayon("TestCrayon", "orangeCrayon.png");
    crayon.translate(glm::vec3(0.0f, 0.05f, -10.0f));
    meshes.push_back(crayon);

    MeshModel crayon2("TestCrayon", "blueCrayon.png");
    crayon2.translate(glm::vec3(2.0f, 0.05f, -10.0f));
    meshes.push_back(crayon2);

    MeshModel floor("Floor", "blueCrayon.png");
    meshes.push_back(floor);

    MeshModel giraffe("Giraffe_Triangles", "Giraffe.png"); 
    giraffe.rotate(180, glm::vec3(1, 0, 0));
    meshes.push_back(giraffe);

    sceneShader.use();

    // Perspective matrix - this doesn't change, so we don't need to set it on every frame  
    glm::mat4 persp = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearClip, farClip);
    sceneShader.setMat4("persp", persp);

    float f = 0.0f;
    bool direction = true;
	
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

        //// Animation 
        //meshes[0].pace(0.1, 5, glm::vec3(0.0f, 0.05f, -10.0f), 'x');
        //meshes[1].pace(0.2, 12, glm::vec3(2.0f, 0.05f, -10.0f), 'z');

        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].Draw(sceneShader);
        }

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}
