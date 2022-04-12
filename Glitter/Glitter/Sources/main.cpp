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

    vector<float> pacingSpeeds{ .1, .1, .1, .1, .1, .09, .08, .07, .07, .06 };
    vector<float> pacingRanges{ 3, 2, 2, 3, 3, 3, 3, 3, 3, 3 };
    vector<glm::vec3> pacingAxis{ glm::vec3(1, 0, 0), glm::vec3(-1, 0, 1), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0) };

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
    irrklang::ISound* backgroundMusic;

    irrklang::ISoundSource* backgroundMusicSound = engine->addSoundSourceFromFile("../Audio/polka.wav");
    backgroundMusicSound->setDefaultVolume(0.2);
    irrklang::ISoundSource* boingSound = engine->addSoundSourceFromFile("../Audio/boing1.wav");
    boingSound->setDefaultVolume(50);

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
                backgroundMusic = engine->play2D(backgroundMusicSound, true);
            }
        }


        if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS) {
            //engine->play2D("../Audio/boing1.wav");

            //objects.at("OrangeCrayon").launch(glm::vec3(0, -1, 0), 4);

            startTime = static_cast<float>(glfwGetTime());
        }

        vector<float> cameraBounds;
        cameraBounds.push_back(cameraPosition[0] + 1.5);
        cameraBounds.push_back(cameraPosition[0] + -1.5);
        cameraBounds.push_back(cameraPosition[2] + 1.5);
        cameraBounds.push_back(cameraPosition[2] + -1.5);

        bool trackCol = false;
        int trackIndex = 0;
        for (int i = 0; i < track.size(); i++) {
            trackCol = CheckCollision(track.at(i), cameraBounds);
            if (trackCol) {
                trackIndex = i;
                break;
            }
        }
        if (trackCol) {
            //track.erase(track.begin() + trackIndex);
            trackCol = false;
        }

        float collideFrame = currentFrame;
        bool giraffeCol = false;
        int giraffeIndex = 0;
        for (int i = 0; i < giraffes.size(); i++) {
            giraffeCol = CheckCollision(giraffes.at(i), cameraBounds);
            if (giraffeCol) {
                giraffeIndex = i;
                giraffes[giraffeIndex].launch(glm::vec3(0, -1, 0), 4);
                collideFrame = static_cast<float>(glfwGetTime());
                if (giraffes[giraffeIndex].position.y == 0) {
                    numGiraffes++;
                    engine->play2D(boingSound);
                }
                break;
            }
        }
        /*if (giraffeCol) {
            numGiraffes++;
            giraffeCol = false;
        }*/
        if (collideFrame - currentFrame > 2 && giraffeCol) {
            giraffes.erase(giraffes.begin() + giraffeIndex - 1);
            pacingSpeeds.erase(pacingSpeeds.begin() + giraffeIndex - 1);
            pacingRanges.erase(pacingRanges.begin() + giraffeIndex - 1);
            pacingAxis.erase(pacingAxis.begin() + giraffeIndex - 1);
            giraffeCol = false;
        }

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
        for (int i = 0; i < giraffes.size(); i++) {
            giraffes[i].pace(pacingSpeeds[i], pacingRanges[i], pacingAxis[i]);
        }

        // BEE OVERLAY
        bee.Draw(beeShader);

        // TEXT RENDERING 
        if (!gameStarted) {
            RenderText(textShader, "BUMBLE BUMPERS", 25.0, windowHeight / 2 + 200, 1.0, glm::vec3(0, 1, 1));
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

                /*backgroundMusic->stop();
                backgroundMusic->drop();*/
                engine->stopAllSounds();
                engine->play2D("../Audio/win.wav");
            }
            

            RenderText(textShader, "Finished!", 25.0f, windowHeight - 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            RenderText(textShader, "Your score: " + to_string((int)calculateScore()), 25.0f, windowHeight - 200, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        }

        // LOSE STATE
        if (timeRemaining < 0 && gameStarted) {
            paused = true;

            if (!scoreCalculated) {
                engine->stopAllSounds();
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
