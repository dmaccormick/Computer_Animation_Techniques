#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "IL\ilut.h"
#include <string>
#include <iostream>
using std::string;

/*
	Texture class
	- Needs a filename as a parameter, should probably be "./res/img/XXXXXXX.png"
	- Not technically necessary but should be a .png
	- Call bind for rendering the object on screen with the texture and then unbind after doing this
*/
class Texture2D
{
public:
	Texture2D();  //Default constructor, useful for when initializing arrays / vectors
	Texture2D(char* fileName); //Actual constructor that takes the file path as a parameter...char* instead of string for the image loader

	void bind(); //Binds the texture for opengl to use it
	void unbind(); //Unbinds the texture from opengl

	int getTextureWidth() const; //Returns the width of the texture in pixels
	int getTextureHeight() const; //Returns the height of the texture in pixels
	GLuint getTextureID() const; //Returns the ID that was generated by OpenGL

private:
	int width; //The width of the texture in pixels
	int height; //The height of the texture in pixels
	GLuint textureID; //The ID of the texture, generated by OpenGL

	void loadPNG(char* fileName);
};

#endif
