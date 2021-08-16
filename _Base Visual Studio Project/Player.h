#ifndef PLAYER_H
#define PLAYER_H

#include "Kinematic.h"
#define INITIAL_POINTS 0
/*
Player class, child of Kinematic
Game object with physics (Kinematic) but it also contains extra information like points, controller id, etc.
*/
class Player : public Kinematic
{
public:
	//All constructors are inherited from Kinematic, exept they initialize points value to 0
	Player() : Kinematic() {}
	Player(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) : Kinematic(pos, rot, scl) {}; //Constructor with just the transform properties
	Player(bool gravityAffected, glm::vec3 accel, glm::vec3 vel, float mass) : Kinematic(gravityAffected, accel, vel, mass) {}; //Constructor with just the physics properties
	Player(MESH_NAME meshName, TEXTURE_NAME texName) : Kinematic(meshName, texName) {}; //Constructor with just the asset properties
	Player(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, bool gravityAffected, glm::vec3 accel, glm::vec3 vel, float startMass) : Kinematic(pos, rot, scl, gravityAffected, accel, vel, startMass) {}; //Constructor with the transform and physics properties
	Player(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, bool gravityAffected, glm::vec3 accel, glm::vec3 vel, float startMass, MESH_NAME meshName, TEXTURE_NAME texName) : Kinematic(pos, rot, scl, gravityAffected, accel, vel, startMass, meshName, texName) {}; //Constructor with all the properties
	~Player() {}

	void addPoints(int points); // Gives the player points
	void takePoints(int points);//Takes points away from the player
	int getPoints();            //Gets number of points

	void transferPointsTo(Player target, int amount); //Transfers some points to the target player
	void takePointsFrom(Player target, int amount);   //Takes points from the target player
	virtual void update(float deltaTime);

private:
	int points = 0;
};

#endif
