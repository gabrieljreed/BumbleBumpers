#pragma once

#include "glitter.hpp"
#include "MeshModel.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

vector<MeshModel> setupTrack() {
	vector<MeshModel> walls;

	// Floor 
	MeshModel floor = MeshModel("Floor", "Asphalt.jpg");
	floor.scale(100);
	floor.translate(0, 0, -43);
	walls.push_back(floor);

	// Walls
	MeshModel wall = MeshModel("WallSidePivot", "Street.jpg");

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 4);
	wall.rotate(90, glm::vec3(0, 1, 0));
	wall.translate(5, -1.6, 7);
	walls.push_back(wall);

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

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.rotate(90, glm::vec3(0, 1, 0));
	wall.translate(-6.75, -1.6, -11.75);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 7);
	wall.rotate(90, glm::vec3(0, 1, 0));
	wall.translate(-3, -1.6, -22);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 4);
	wall.rotate(45, glm::vec3(0, 1, 0));
	wall.translate(-22, -1.6, -22);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 8);
	wall.rotate(55, glm::vec3(0, 1, 0));
	wall.translate(-20, -1.6, -11.5);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.translate(-30, 0, -44);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 9);
	wall.translate(-38.5, 0, -49.5);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.rotate(-35, glm::vec3(0, 1, 0));
	wall.translate(-38.5, -1.6, -48.5);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 2);
	wall.rotate(-35, glm::vec3(0, 1, 0));
	wall.translate(-30, -1.6, -43.5);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 20);
	wall.rotate(-90, glm::vec3(0, 1, 0));
	wall.translate(-27.5, -1.6, -48);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 20);
	wall.rotate(-90, glm::vec3(0, 1, 0));
	wall.translate(-31, -1.6, -60);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.rotate(-30, glm::vec3(0, 1, 0));
	wall.translate(29, -1.6, -48);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 10);
	wall.rotate(0, glm::vec3(0, 1, 0));
	wall.translate(36, -1.6, -60);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 10);
	wall.rotate(0, glm::vec3(0, 1, 0));
	wall.translate(25, -1.6, -60);
	walls.push_back(wall);

	wall.resetTransformations();
	wall.scale(0.5);
	wall.scale(1, 1, 5);
	wall.rotate(90, glm::vec3(0, 1, 0));
	wall.translate(37, -1.6, -88);
	walls.push_back(wall);

	//MeshModel finishLine = MeshModel("CubeTriangles", "Asphalt.png");
	//finishLine.scale(glm::vec3(11, 1, 1));
	//finishLine.translate(30.5, 0.4, -79);
	//walls.push_back(finishLine);

	return walls;
}

// ------------------------------------------------ GIRAFFES ------------------------------------------------
vector<MeshModel> setupGiraffes() {
	vector<MeshModel> giraffes;

	MeshModel giraffe = MeshModel("Giraffe_Triangles", "Giraffe.png");

	giraffe.resetTransformations();
	giraffe.scale(0.3);
	giraffe.translate(0, 0, -3);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(-5, 0, -15);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(-20, 0, -17);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(-34, 0, -27);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(-34, 0, -47);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(-14, 0, -54);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(0, 0, -54);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(14, 0, -54);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(30, 0, -59);
	giraffes.push_back(giraffe);

	giraffe.translateAbsolute(30, 0, -75);
	giraffes.push_back(giraffe);

	return giraffes;
}
