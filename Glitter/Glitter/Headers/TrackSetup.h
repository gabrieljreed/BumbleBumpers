#pragma once

#include "glitter.hpp"
#include "MeshModel.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

map<string, MeshModel> setupTrack() {
	map<string, MeshModel> map;

	vector<MeshModel> walls;

	// Floor 
	MeshModel floor = MeshModel("Floor", "Street.jpg");
	map.insert({"Floor", floor});

	// Walls
	MeshModel wall = MeshModel("WallSidePivot", "Street.jpg");

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 6);
	wall.translate(4, 0, -10);
	walls.push_back(wall);
	
	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.translate(-4, 0, -7);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.rotate(30, glm::vec3(0, 1, 0));
	wall.translate(4, -1.6, -9.75);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 2);
	wall.rotate(30, glm::vec3(0, 1, 0));
	wall.translate(-4, -1.6, -6.75);
	walls.push_back(wall);



	for (unsigned int i = 0; i < walls.size(); i++) {
		map.insert({ "Wall" + to_string(i), walls[i] });
	}

	

	// Obstacles 

	return map;
}
