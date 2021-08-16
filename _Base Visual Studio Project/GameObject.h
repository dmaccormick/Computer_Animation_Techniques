#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <vector>
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "AssetManager.h"
#include "CollisionBoxes.h"

/*
	GameObject class
	- Has standard position, rotation, and scale
	- Also has a pointer for a mesh and a texture which points to one of the loaded assets in the asset manager
	- Physics features have been moved to the Kinematic class solely (which is a child of this class)
	- Polymorphic update function allows any children of this to have unique behaviour (ie: interact with physics)
*/
class GameObject
{
public:
	GameObject(); //Default constructor
	GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale); //Constructor with just transform components
	GameObject(MESH_NAME meshName, TEXTURE_NAME texName); //Constructor with just the asset components
	GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, MESH_NAME meshName, TEXTURE_NAME texName); //Constructor with both the transform components and the asset components
	~GameObject(); //Destructor, cleans up the pointers
	
	void setPosition(glm::vec3 newPosition); //Various ways to set the position
	void setPosition(float newX, float newY, float newZ);
	void setPositionX(float newX);
	void setPositionY(float newY);
	void setPositionZ(float newZ);

	void setRotation(glm::vec3 newRotation); //Various ways to set the rotation
	void setRotation(float newX, float newY, float newZ);
	void setRotationX(float newX);
	void setRotationY(float newY);
	void setRotationZ(float newZ);

	void setScale(glm::vec3 newScale); //Various ways to set the scale
	void setScale(float newX, float newY, float newZ);
	void setScaleX(float newX);
	void setScaleY(float newY);
	void setScaleZ(float newZ);

	void setMesh(MESH_NAME newMesh); //Change the mesh by passing one of the mesh names from the enum in asset manager
	void setTexture(TEXTURE_NAME newTexture); //Change the texture by passing one of the texture names from the enum in asset manager
	void setColour(glm::vec4 colourRGBA); //Change the colour and alpha value of the mesh
	void setCollisionBox(glm::vec3 position, glm::vec3 extent); //Change the position and extent of the collision box if the default is not desired

	glm::vec3 getPosition() const; //Returns the position 
	glm::vec3 getRotation() const; //Returns the rotation
	glm::vec3 getScale() const; //Returns the scale
	glm::mat4 getLocalToWorldMatrix() const; //Returns the local to world matrix
	glm::vec4 getColour() const; //Returns the colour as a vec4 (RGBA)
	Col_OBB getCollisionBox() const; //Returns the collision box object attached to this gameobject

	void addToPosition(glm::vec3 addition); //Can alter the position easily
	void addToPosition(float additionX, float additionY, float additionZ);
	void addToRotation(glm::vec3 addition); //Can alter the rotation easily
	void addToRotation(float additionX, float additionY, float additionZ);
	void addToScale(glm::vec3 addition); //Can alter the scale easily
	void addToScale(float additionX, float additionY, float additionZ);

	virtual void update(float dt); //Polymorphic update function, handles the movement and drawing of the gameobject every frame

protected:
	glm::vec3 position; //The position stored as an x,y,z
	glm::vec3 rotation; //The rotation (in degress) stored as an x,y,z
	glm::vec3 scale; //The scale stored as an x,y,z
	glm::vec4 colour; //The colour in RGBA
	Mesh* mesh; //Pointer to a pre-loaded mesh in asset manager allows this to be changed at runtime without having to load a new mesh 
	Texture2D* texture; //Pointer to a pre-loaded texture in asset manager allows this to changed at runtime without having to load a new mesh
	glm::mat4 localToWorld; //Matrix that will be used to convert the object's transformations to the world so it is positioned properly
	Col_OBB collisionBox; //Automatically generates an OBB collider that fits the gameobject

	friend std::ostream& operator << (std::ostream& os, const GameObject& gameObject); //Overloaded stream insertion operator, allows us to easily output properties of this object
};
#endif
