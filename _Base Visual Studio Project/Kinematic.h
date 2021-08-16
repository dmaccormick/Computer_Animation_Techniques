#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "GameObject.h"

/*
	Kinematic class
	- Child of GameObject class
	- Big difference is that this class is controlled by physics
	- Polymorphic update function uses physics to update the position of the object and then calls parent update to handle drawing

	Note:
	When passing the accel into a setter or constructor, pass in the acceleration EXCLUDING gravity and then choose whether or not to add gravity in after the fact by setting the affectedByGravity bool to true or false
*/
class Kinematic : public GameObject
{
public:
	Kinematic(); //Default constructor
	Kinematic(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl); //Constructor with just the transform properties
	Kinematic(bool gravityAffected, glm::vec3 accel, glm::vec3 vel, float mass); //Constructor with just the physics properties
	Kinematic(MESH_NAME meshName, TEXTURE_NAME texName); //Constructor with just the asset properties
	Kinematic(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, bool gravityAffected, glm::vec3 accel, glm::vec3 vel, float startMass); //Constructor with the transform and physics properties
	Kinematic(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, bool gravityAffected, glm::vec3 accel, glm::vec3 vel, float startMass, MESH_NAME meshName, TEXTURE_NAME texName); //Constructor with all the properties
	~Kinematic(); //Destructor, currently doesn't really do anything

	void setAccel(glm::vec3 newAccel); //Setters for acceleration
	void setAccel(float newX, float newY, float newZ);
	void addAccel(glm::vec3 amount); //Add acceleration onto the existing amount
	void addAccel(float amountX, float amountY, float amountZ);

	void setVelocity(glm::vec3 newVelocity); //Setters for velocity
	void setVelocity(float newX, float newY, float newZ);
	void addVelocity(glm::vec3 amount); //Add velocity onto the existing amount
	void addVelocity(float amountX, float amountY, float amountZ);

	void setMass(float newMass); //Set the mass
	void addMass(float amount); //Add to the mass

	void setAffectedByGravity(bool affected); //Sets if the object will be affected by gravity consistently

	glm::vec3 getAccel() const; //Returns the acceleration
	glm::vec3 getVelocity() const; //Returns the velocity
	float getMass() const; //Returns the mass
	bool getAffectedByGravity() const; //Returns if the object is affecte by gravity

	virtual void update(float dt); //Polymorphic update function, positions the object using physics and then calls the parent update function to draw it properly

private:
	bool affectedByGravity; //If the object is affected by gravity or not...important because it will affect the inital acceleration passed into the constructor
	glm::vec3 acceleration; //The instantaneous acceleration of the object, initially set to gravity constant below
	glm::vec3 velocity; //The instantaneous velocity of the object, calculated by integrating acceleration into the previous frame's velocity
	float mass; //How heavy the object is (in kg)

	static float dragConstant; //Constant drag force applied to all objects opposite their direction of travel, slows them down and stops them if no force is applied
	static glm::vec3 gravity; //Constant downward force, not necessarily -9.81 m/s/s but currently set to this

	friend std::ostream& operator << (std::ostream& os, const Kinematic& kinematicObject); //Overloaded stream insertion operator to display values, starts by calling parent GameObject's operator and then adds on the physics values
};

#endif
