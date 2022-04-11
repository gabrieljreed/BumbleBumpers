#include "CollisionHandler.h"

bool CheckCollision(MeshModel& one, MeshModel& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.position.x + one.distPosX >= two.position.x &&
        two.position.x + two.distPosX >= one.position.x;
    // collision y-axis?
    bool collisionZ = one.position.z + one.distPosZ >= two.position.z &&
        two.position.z + two.distPosZ >= one.position.z;
    // collision only if on both axes
    return collisionX && collisionZ;
    //return false;
}