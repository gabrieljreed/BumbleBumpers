#pragma once
bool debugMode = true; // Allows camera/player to move up and down the y axis 

bool gameStarted = false;
bool paused = true;

float startTime = 0.0f;
float totalTime = 60.0f; // Total number of seconds the player has to finish the game 
float finalTime = 0.0f; // To be set when the player crosses the finish line 
int numGiraffes = 0; // Number of giraffes the player has hit 

bool scoreCalculated = false;


float calculateScore() {
	float timeRemaining = totalTime - finalTime; 

	return timeRemaining * (numGiraffes + 1);
}