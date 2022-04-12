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
#include "CollisionHandler.h"

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
    MeshShader beeShader = MeshShader("cameraSpaceMesh.vert", "basic.frag");

    MeshModel bee("beeUV", "beeTexture.png");
    bee.rotate(180, glm::vec3(1, 0, 0)); // Bee model always needs to be rotated 180 degrees 
    bee.translate(0, 2, -2);

    vector<MeshModel> track = setupTrack();
    vector<MeshModel> giraffes = setupGiraffes();

    sceneShader.use();

    // Perspective matrix - this doesn't change, so we don't need to set it on every frame  
    glm::mat4 persp = glm::perspective(FOV, (float)windowWidth / (float)windowHeight, nearClip, farClip);
    sceneShader.setMat4("persp", persp);
    beeShader.use();
    beeShader.setMat4("persp", persp);
    sceneShader.use();

    sceneShader.setVec3("lightColor", lightColor);
    sceneShader.setVec3("lightPosition", lightPosition);
    beeShader.use();
    beeShader.setVec3("lightColor", lightColor);
    beeShader.setVec3("lightPosition", lightPosition);
	
    // ------------------------------------------------ RENDERING LOOP ------------------------------------------------

    //float startTime = 0.0f;
    bool giraffeHit = false;

    while (glfwWindowShouldClose(mWindow) == false) {
        sceneShader.use();

        // Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int timeElapsed = (int)(currentFrame - startTime);
        int timeRemaining = (int)(totalTime - timeElapsed);

        // Handle user input 
        handleKeypress(mWindow);

        if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_PRESS) {
            paused = false;
            if (!gameStarted) {
                gameStarted = true;
                startTime = static_cast<float>(glfwGetTime());
                engine->play2D("../Audio/go.wav");
                engine->play2D("../Audio/polka.wav", true);
            }
        }


        if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS) {
            //engine->play2D("../Audio/boing1.wav");

            //objects.at("OrangeCrayon").launch(glm::vec3(0, -1, 0), 4);

            startTime = static_cast<float>(glfwGetTime());
            /*giraffeHit = CheckCollision(objects.at("Giraffe"), objects.at("Bee"));
            if (giraffeHit) {
                objects.erase("Giraffe");
            }*/
        }

        //if (currentFrame - startTime > 2 && giraffeHit) {
        //    // Kill after 2 seconds
        //    // This only works if you hit one giraffe at a time
        //    // Might need to revisit this if we want to do more than that  
        //    objects.erase("OrangeCrayon");
        //    giraffeHit = false;
        //}

        // Background Fill Color/Clear each frame 
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix 
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        sceneShader.setMat4("view", view);
        
        // DRAW OBJECTS 
        for (unsigned int i = 0; i < track.size(); i++) {
            track[i].Draw(sceneShader);
        }

        for (unsigned int i = 0; i < giraffes.size(); i++) {
            giraffes[i].Draw(sceneShader);
        }

        // GIRAFFES PACING 
        giraffes[0].pace(0.1, 3, glm::vec3(1, 0, 0));
        giraffes[1].pace(0.1, 2, glm::vec3(-1, 0, 1));
        giraffes[2].pace(0.1, 2, glm::vec3(0, 0, 1));
        giraffes[3].pace(0.1, 3, glm::vec3(1, 0, 0));
        giraffes[4].pace(0.1, 3, glm::vec3(1, 0, 1));
        giraffes[5].pace(0.09, 3, glm::vec3(0, 0, 1));
        giraffes[6].pace(0.08, 3, glm::vec3(0, 0, 1));
        giraffes[7].pace(0.07, 3, glm::vec3(0, 0, 1));
        giraffes[8].pace(0.07, 3, glm::vec3(1, 0, 1));
        giraffes[9].pace(0.06, 3, glm::vec3(1, 0, 0));

        // BEE OVERLAY
        bee.Draw(beeShader);

        // TEXT RENDERING 
        if (!gameStarted) {
            RenderText(textShader, "Press Enter to start!", 25.0, windowHeight / 2, 1.0, glm::vec3(1, 1, 1));
        }

        if (gameStarted && !paused) {
            RenderText(textShader, "Time remaining: " + to_string(timeRemaining), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            RenderText(textShader, "Giraffes hit: " + to_string(numGiraffes), 25.0f, windowHeight - 100, 1.0f, glm::vec3(1.0, 0, 0));
            //sceneShader.use();
        }

        // WIN STATE
        if (cameraPosition.z < -80) {
            paused = true;

            if (!scoreCalculated) {
                finalTime = static_cast<float>(glfwGetTime());
                scoreCalculated = true;

                engine->play2D("../Audio/win.wav");
            }
            

            RenderText(textShader, "Finished!", 25.0f, windowHeight - 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            RenderText(textShader, "Your score: " + to_string((int)calculateScore()), 25.0f, windowHeight - 200, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        }

        // LOSE STATE
        if (timeRemaining < 0 && gameStarted) {
            paused = true;

            if (!scoreCalculated) {
                engine->play2D("../Audio/lose.wav");
                scoreCalculated = true;
            }

            RenderText(textShader, "You ran out of time!", 25.0f, windowHeight - 100, 1.0f, glm::vec3(1, 0.0f, 0.0f));
        }

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    engine->drop();
    
    return EXIT_SUCCESS;
}
