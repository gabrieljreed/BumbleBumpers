// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"
#include "MeshModel.h"
#include "Shader.h"
#include "InputHandler.h"
#include "TrackSetup.h"
#include "Gamemode.h"
#include "TextHandler.h"

// Text Headers
#include <ft2build.h>
#include FT_FREETYPE_H

// Sound headers
#include "irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

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

float deltaTime = 0.0f;
float lastFrame = 0.0f;

using namespace std;

int main(int argc, char * argv[]) {

    // Window setup 
    auto mWindow = windowSetup();

    // Audio setup 
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine)
        return 0; // Error starting up sound device 

    // Text setup 
    MeshShader textShader = MeshShader("text2D.vert", "text2D.frag");
    textShader.use();

    glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight));
    textShader.setMat4("projection", orthoProjection);
    setupText();

    // ------------------------------------------------ DEFINE SCENE  ------------------------------------------------
    // Lighting
    glm::vec3 lightPosition(1.0f, 1.0f, 0.5f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // Meshes
    MeshShader sceneShader = MeshShader("basic.vert", "basic.frag");
    

    map<string, MeshModel> objects;
    map<string, MeshModel>::iterator iter;

    // To insert objects into the scene, declare them here and add them to the objects map with a unique name

    MeshModel giraffe("Giraffe_Triangles", "Giraffe.png"); 
    giraffe.scale(0.5);
    objects.insert({ "Giraffe", giraffe });

    map<string, MeshModel> track = setupTrack();
    for (iter = track.begin(); iter != track.end(); ++iter) {
        objects.insert({ iter->first, iter->second });
    }

    sceneShader.use();

    // Perspective matrix - this doesn't change, so we don't need to set it on every frame  
    glm::mat4 persp = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearClip, farClip);
    sceneShader.setMat4("persp", persp);

    sceneShader.setVec3("lightColor", lightColor);
    sceneShader.setVec3("lightPosition", lightPosition);
	
    // ------------------------------------------------ RENDERING LOOP ------------------------------------------------

    float startTime = 0.0f;
    bool giraffeHit = false;

    while (glfwWindowShouldClose(mWindow) == false) {
        sceneShader.use();

        // Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle user input 
        handleKeypress(mWindow);

        if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS) {
            engine->play2D("../Audio/boing1.wav");

            objects.at("OrangeCrayon").launch(glm::vec3(0, -1, 0), 4);

            startTime = static_cast<float>(glfwGetTime());
            giraffeHit = true;
        }

        if (currentFrame - startTime > 2 && giraffeHit) {
            // Kill after 2 seconds
            // This only works if you hit one giraffe at a time
            // Might need to revisit this if we want to do more than that  
            objects.erase("OrangeCrayon");
            giraffeHit = false;
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

        textShader.use();
        RenderText(textShader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        sceneShader.use();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    engine->drop();
    
    return EXIT_SUCCESS;
}
