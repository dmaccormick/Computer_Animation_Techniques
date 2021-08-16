#include "Texture2D.h"

Texture2D::Texture2D()
{
	width = 0;
	height = 0;
	textureID = -1;
}

Texture2D::Texture2D(char* fileName)
{
	width = 0;
	height = 0;
	textureID = -1;
	loadPNG(fileName);
}

void Texture2D::loadPNG(char* fileName)
{
	try
	{
		textureID = ilutGLLoadImage(fileName);
	}
	catch (...)
	{
		std::cout << "Unknown error when trying to load texture (" << fileName << ")! Aborting!" << std::endl;
		abort();
	}

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
}

void Texture2D::bind()
{
	if (textureID != -1)
		glBindTexture(GL_TEXTURE_2D, textureID);
	else
	{
		std::cout << "Error! Trying to bind a texture with an ID of -1! Aborting!" << std::endl;
		abort();
	}
}

void Texture2D::unbind()
{
	if (textureID != -1)
		glBindTexture(GL_TEXTURE_2D, NULL);
	else
	{
		std::cout << "Error! Trying to ubbind a texture with an ID of -1! Aborting!" << std::endl;
		abort();
	}
}

int Texture2D::getTextureWidth() const {
	return width;
}

int Texture2D::getTextureHeight() const {
	return height;
}

GLuint Texture2D::getTextureID() const {
	return textureID;
}