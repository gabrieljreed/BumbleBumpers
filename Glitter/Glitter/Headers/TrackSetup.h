#pragma once

#include "glitter.hpp"
#include "MeshModel.h"

#include <map>
#include <string>

map<string, MeshModel> setupTrack() {
	map<string, MeshModel> map;

	// Floor 
	MeshModel floor = MeshModel("Floor", "Street.jpg");
	map.insert({"Floor", floor});

	// Walls
	MeshModel wall = MeshModel("Wall", "Street.jpg");
	map.insert({ "Wall1", wall });

	// Obstacles 

	return map;
}
