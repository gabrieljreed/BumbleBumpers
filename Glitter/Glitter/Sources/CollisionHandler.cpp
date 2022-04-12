#include "CollisionHandler.h"

bool CheckCollision(MeshModel& one, MeshModel& two) // AABB - AABB collision
{
    /*// collision x-axis?
    bool posCollisionX = one.position.x + one.distPosX >= two.position.x &&
        two.position.x + two.distPosX >= one.position.x;
    bool negCollisionX = one.position.x + one.distNegX >= two.position.x &&
        two.position.x + two.distNegX >= one.position.x;
    // collision y-axis?
    bool posCollisionZ = one.position.z + one.distPosZ >= two.position.z &&
        two.position.z + two.distPosZ >= one.position.z;
    bool negCollisionZ = one.position.z + one.distNegZ >= two.position.z &&
        two.position.z + two.distNegZ >= one.position.z;
    // collision only if on both axes
    return true; // collisionX&& collisionZ;
    //return false;*/

    //bool colX = (one._minX <= two._maxX && two._maxX <= one._maxX) || 
    //    (one._minX <= two._minX && two._minX <= one._maxX) ||
    //    (one._minX >= two._minX && two._maxX >= one._maxX);
    //bool colZ = (one._minZ <= two._maxZ && two._maxZ <= one._maxZ) || 
    //    (one._minZ <= two._minZ && two._minZ <= one._maxZ);
    

    bool colX = (one._maxX >= two._maxX && two._maxX >= one._minX) ||
        (one._maxX >= two._minX && two._minX >= one._minX) ||
        (one._maxX >= two._maxX && one._minX <= two._minX) ||
        (two._maxX >= one._maxX && two._minX <= one._minX);
    bool colZ = (one._maxZ >= two._maxZ && two._maxZ >= one._minZ) ||
        (one._maxZ >= two._minZ && two._minZ >= one._minZ) ||
        (one._maxZ >= two._maxZ && one._minZ <= two._minZ) ||
        (two._maxZ >= one._maxZ && two._minZ <= one._minZ);
    return colX && colZ;
}