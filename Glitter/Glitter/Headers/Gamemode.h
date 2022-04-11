#pragma once
bool debugMode = false; // Allows camera/player to move up and down the y axis 

bool paused = false;
float totalTime = 90.0f; // Total number of seconds the player has to finish the game 
float finalTime = 0.0f; // To be set when the player crosses the finish line 
int numGiraffes = 0; // Number of giraffes the player has hit 


float calculateScore() {
	float timeRemaining = totalTime - finalTime; 

	return timeRemaining * numGiraffes;
}