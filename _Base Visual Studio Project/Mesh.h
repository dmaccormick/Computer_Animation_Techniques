#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <GLUT\glut.h>
#include <vector>
#include <fstream>
#include <glm\glm.hpp>

using std::cout;
using std::ifstream;
using std::string;
using std::endl;

/*
	Simple Vertex struct that basically just has an ordered-triplet. Struct instead of class due to just holding some data & ease of access
*/
struct Vertex
{
	Vertex()
	{
		position = glm::vec3(0.0f);
		uvCoord = glm::vec3(0.0f);
		normal = glm::vec3(0.0f);
	}

	Vertex(glm::vec3 pos, glm::vec3 uv, glm::vec3 n)
	{
		position = pos;
		uvCoord = uv;
		normal = n;
	}

	void output()
	{
		cout << "Vertex === " << endl;
		cout << "POS: " << position.x << ", " << position.y << ", " << position.z << endl;
		cout << "UV: " << uvCoord.x << ", " << uvCoord.y << endl;
		cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << endl << endl;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoord;
};

/*
	Simple Face struct, contains 3 vertices
*/
struct Face
{
	Face() 
	{
		vertices = new Vertex[3];
	}

	Face(Vertex a, Vertex b, Vertex c)
	{
		vertices = new Vertex[3];
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;

		//Calculate and store the face normal for lighting
		glm::vec3 p0 = vertices[1].position - vertices[0].position;
		glm::vec3 p1 = vertices[2].position - vertices[0].position;
		faceNormal = glm::cross(p0, p1);
	}

	~Face()
	{
		//delete[] vertices;
		//vertices = 0;
	}

	void draw(bool textured) const
	{
		//Pass the face normal to OpenGl for lighting
		glNormal3f(faceNormal.x, faceNormal.y, faceNormal.z);

		for (int i = 0; i < 3; i++)
		{
			//If the object is textured, passes the tex coords to opengl too
			if (textured)
				glTexCoord2f(vertices[i].uvCoord.x, vertices[i].uvCoord.y);

			//Passes the vertices to OpenGl
			glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
		}
	}

	void output() const
	{
		cout << "FACE -------------------" << endl;

		for (int i = 0; i < 3; i++)
			vertices[i].output();

		cout << "\n\n";
	}

	Vertex* vertices;
	glm::vec3 faceNormal;
};

/*
Mesh class that is basically just currently an array of faces
Takes in a string for the fileName to load the faces from
*/
class Mesh
{
public:
	Mesh();
	Mesh(const string& fileName); //The mesh takes a string for the fileName that it is loading in OBJ format, this will be passed to the OBJLoader in order to get the vertices from it
	~Mesh() {};

	void draw(bool textured) const; //Called every frame...passes the vertices to immediate openGl in the main screen callback function
	void output() const;

private:
	std::vector<Face> faces; //Vector of faces
	int numFaces; //How many vertices are in the mesh, calculated when the obj is loaded

	void loadOBJ(const string& fileName); //private utility functions that actually loads the obj passed to the mesh in the constructor
	void orderVertices();
};

#endif
