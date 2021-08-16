#include "GameObject.h"
#include "glm\gtx\string_cast.hpp"
/*
	Constructors and destructor
*/
GameObject::GameObject()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	mesh = 0;
	texture = 0;
	localToWorld = glm::mat4(0.0f);
	colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	collisionBox = Col_OBB(position, scale);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
{
	position = pos;
	rotation = rot;
	scale = scl;
	mesh = 0;
	texture = 0;
	localToWorld = glm::mat4(0.0f);
	colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	collisionBox = Col_OBB(position, scale);
}

GameObject::GameObject(MESH_NAME meshName, TEXTURE_NAME texName)
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	mesh = &AM::assets()->getMesh(meshName);
	texture = &AM::assets()->getTexture2D(texName);
	localToWorld = glm::mat4(0.0f);
	colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	collisionBox = Col_OBB(position, scale);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, MESH_NAME meshName, TEXTURE_NAME texName)
{
	position = pos;
	rotation = rot;
	scale = scl;
	mesh = &AM::assets()->getMesh(meshName);
	texture = &AM::assets()->getTexture2D(texName);
	localToWorld = glm::mat4(0.0f);
	colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	collisionBox = Col_OBB(position, scale);
}

GameObject::~GameObject()
{
	delete mesh;
	delete texture;

	mesh = NULL;
	texture = NULL;
}

/*
	Position setters
*/
void GameObject::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}

void GameObject::setPosition(float newX, float newY, float newZ) {
	position = glm::vec3(newX, newY, newZ);
}

void GameObject::setPositionX(float newX) {
	position.x = newX;
}

void GameObject::setPositionY(float newY) {
	position.y = newY;
}

void GameObject::setPositionZ(float newZ) {
	position.z = newZ;
}

/*
	Rotation setters
*/
void GameObject::setRotation(glm::vec3 newRotation) {
	rotation = newRotation;
}

void GameObject::setRotation(float newX, float newY, float newZ) {
	rotation = glm::vec3(newX, newY, newZ);
}

void GameObject::setRotationX(float newX) {
	rotation.x = newX;
}

void GameObject::setRotationY(float newY) {
	rotation.y = newY;
}

void GameObject::setRotationZ(float newZ) {
	rotation.z = newZ;
}

/*
	Scale setters
*/
void GameObject::setScale(glm::vec3 newScale) {
	scale = newScale;
}

void GameObject::setScale(float newX, float newY, float newZ) {
	scale = glm::vec3(newX, newY, newZ);
}

void GameObject::setScaleX(float newX) {
	scale.x = newX;
}

void GameObject::setScaleY(float newY) {
	scale.y = newY;
}

void GameObject::setScaleZ(float newZ) {
	scale.z = newZ;
}

/*
	Asset setters
*/
void GameObject::setMesh(MESH_NAME newMesh) {
	mesh = &AM::assets()->getMesh(newMesh);
}

void GameObject::setTexture(TEXTURE_NAME newTexture) {
	texture = &AM::assets()->getTexture2D(newTexture);
}

void GameObject::setColour(glm::vec4 newColourRGBA) {
	colour = newColourRGBA;
}

void GameObject::setCollisionBox(glm::vec3 position, glm::vec3 extent) {
	collisionBox = Col_OBB(position, extent);
}

/*
	Getters
*/
glm::vec3 GameObject::getPosition() const {
	return position;
}

glm::vec3 GameObject::getRotation() const {
	return rotation;
}

glm::vec3 GameObject::getScale() const {
	return scale;
}

glm::mat4 GameObject::getLocalToWorldMatrix() const {
	return localToWorld;
}

glm::vec4 GameObject::getColour() const {
	return colour;
}

Col_OBB GameObject::getCollisionBox() const {
	return collisionBox;
}

/*
	Adders
*/
void GameObject::addToPosition(glm::vec3 addition) {
	position += addition;
}

void GameObject::addToPosition(float additionX, float additionY, float additionZ) {
	position += glm::vec3(additionX, additionY, additionZ);
}

void GameObject::addToRotation(glm::vec3 addition) {
	rotation += addition;
}

void GameObject::addToRotation(float additionX, float additionY, float additionZ) {
	rotation += glm::vec3(additionX, additionY, additionZ);
}

void GameObject::addToScale(glm::vec3 addition) {
	scale += addition;
}

void GameObject::addToScale(float additionX, float additionY, float additionZ) {
	scale += glm::vec3(additionX, additionY, additionZ);
}

//Update function that properly handles positioning the game object and also drawing the model
void GameObject::update(float dt)
{
	//Create the scaling matrix
	glm::mat4 scaleMatrix = glm::scale(scale);

	//Create the individual rotation matrices and then the concatenated one
	glm::mat4 rotMatrix_X = glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotMatrix_Y = glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotMatrix_Z = glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 fullRotationMatrix = rotMatrix_Z * rotMatrix_Y * rotMatrix_X;

	//Create the translation matrix
	glm::mat4 translationMatrix = glm::translate(position);

	//Compiles the transformation together in the correct order: Scale -> Rotate -> Translate (Note the right to left notation)
	localToWorld = translationMatrix * fullRotationMatrix * scaleMatrix;

	//Passes the matrix to OpenGL which automatically applies the transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(localToWorld));

	//Checks if there is a texture assigned before trying to bind it, otherwise temporarily disables textures so it renders with colours
	if (texture != 0)
		texture->bind();
	else
	{
		glDisable(GL_TEXTURE_2D);
		glColor4f(colour.x, colour.y, colour.z, colour.w);
	}
		
	//Checks if there is a mesh assigned before tyring to draw it
	if (mesh != 0)
	{
		if (texture != 0)
			mesh->draw(true);
		else
			mesh->draw(false);
	}
		
	//Checks if there is a texture assigned before trying to unbind it, otherwise re-enables textures
	if (texture != 0)
		texture->unbind();
	else
		glEnable(GL_TEXTURE_2D);

	//Loads identity for cleanliness
	glLoadIdentity();
}

/*
	Overloaded operators
*/
std::ostream& operator << (std::ostream& os, const GameObject& object)
{
	glm::vec3 pos = object.getPosition();
	glm::vec3 rot = object.getRotation();
	glm::vec3 scl = object.getScale();
	glm::vec4 col = object.getColour();

	os << "----- GameObject -----" << std::endl;
	os << "POS: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
	os << "ROT: " << rot.x << ", " << rot.y << ", " << rot.z << std::endl;
	os << "SCL: " << scl.x << ", " << scl.y << ", " << scl.z << std::endl;
	os << "COLOUR: " << col.x << ", " << col.y << ", " << col.z << ", " << col.w << std::endl;
	os << "MATRIX: \n" << glm::to_string(object.getLocalToWorldMatrix()) << std::endl << std::endl;

	return os;
}