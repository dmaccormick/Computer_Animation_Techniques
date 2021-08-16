#ifndef COLLISION_H
#define COLLISION_H

#include "glm\glm.hpp"
#include "glm\gtx\projection.hpp"
#include "CollisionBoxes.h"
#include "GameObject.h"

struct Collision
{
	Collision(bool _status, glm::vec3 pen) : status(_status), penetration(pen) {}
	bool status;
	glm::vec3 penetration;
	
	operator bool() const { return status; }
};

class CollisionHandler
{
public:
	//Same Type Collision
	static Collision SPHEREvSPHERE(const Col_Sphere& a, const Col_Sphere& b);
	static Collision AABBvAABB(const Col_AABB& a, const Col_AABB& b);
	static Collision OBBvOBB(const Col_OBB& a, const Col_OBB& b);

	//Type A vs Type B
	static Collision SPHEREvAABB(const Col_Sphere& a, const Col_AABB& b);
	static Collision SPHEREvOBB(const Col_Sphere& a, const Col_OBB& b);
	static Collision AABBvOBB(const Col_AABB& a, const Col_OBB& b);

	//Type B vs Type A
	static Collision AABBvSPHERE(const Col_AABB& a, const Col_Sphere& b);
	static Collision OBBvSPHERE(const Col_OBB& a, const Col_Sphere& b);
	static Collision OBBvAABB(const Col_OBB& a, const Col_AABB& b);

	//Gameobject VS Gameobject
	static Collision OBJECTvOBJECT(const GameObject& a, const GameObject& b);
};
typedef CollisionHandler CH;

#endif
