#include "Mesh.h"
Mesh::Mesh()
{
	numFaces = 0;
}

Mesh::Mesh(const string& fileName)
{
	numFaces = 1;

	loadOBJ(fileName);
}

void Mesh::draw(bool textured) const
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < numFaces; i++)
			faces[i].draw(textured);
	}
	glEnd();
}

void Mesh::loadOBJ(const string& fileName)
{
	/* Creates vectors for the position data, the uv data, and the normal data. It also makes one that holds the information for the object's faces */
	std::vector<float> posData;
	std::vector<float> uvData;
	std::vector<float> normalData;
	std::vector<int> faceData;

	/* Loads the file from the string */
	ifstream inOBJ(fileName);

	/* Checks if the file load failed and exits / informs user if it does */
	if (inOBJ.fail())
	{
		cout << "ERROR: " + fileName + " failed to open. Exiting..." << endl;
		abort();
	}

	/* Will temporarily hold the next part of the file up to the next space */
	string nextInput;

	/* At which portion of the file the parser is at, 0 = header, 1 = posData, 2 = uvData, 3 = normalData, 4 = face data */
	int inputStage = 0;

	/* Parses through the file and reads in the data, ignoring the comments and whatnot */
	while (inOBJ >> nextInput)
	{
		/* Transitions between stages of file data reading depending on what type of data is being read in. This is detected by certain key characters within the file. v = posData, vt = textureData, etc */
		if (nextInput.back() == 'v')
			inputStage = 1;
		else if (nextInput.back() == 't')
			inputStage = 2;
		else if (nextInput.back() == 'n')
			inputStage = 3;
		else if (nextInput.back() == 'f')
			inputStage = 4;

		if (inputStage == 4)
		{
			if (nextInput.back() == '#')
			{
				cout << "Error! OBJ contains more than one model inside it! This loader isn't able to handle that yet :( The program will continue but will break from the .obj file read-in loop!" << endl;
				break;
			}
		}

		/* Converts all of the read in data to the floats necessary to be stored. If the input can't be converted to a float (ie: a letter), it is ignored */
		try
		{
			if (inputStage == 1) // Pos Data Stage
				posData.push_back(stof(nextInput)); //Converts the data to a float and adds it to the vector
			else if (inputStage == 2) // UV (Texture) Data Stage
				uvData.push_back(stof(nextInput));
			else if (inputStage == 3) // Normal Data Stage
				normalData.push_back(stof(nextInput));
			else if (inputStage == 4) // Face Data Stage
			{
				if (nextInput.length() > 1) //Checks to make sure the input isn't just an 'f'
				{
					/* Finds the seperators in the face data and splits up the string into substrings in order to properly read it in -> faces are saved like 15/3/90 and read in all at once, need to seperate the #'s from the /'s */
					int div1 = 0, div2 = 0;

					div1 = nextInput.find_first_of('/');
					div2 = nextInput.find_last_of('/');

					faceData.push_back(stof(nextInput.substr(0, div1)));

					if (div2 > div1 + 1)
						faceData.push_back(stof(nextInput.substr(div1 + 1, div2 - div1 - 1)));
					else
						faceData.push_back(0);

					faceData.push_back(stof(nextInput.substr(div2 + 1, string::npos)));
				}
			}
		}
		catch (std::invalid_argument)
		{
			continue; //Continues on parsing and ignores the input if it can't convert the value to a float, ie: its a letter
		}
		catch (...) //Catches any other unknown errors
		{
			cout << "Unknown error occured in the OBJ loader for file: " << fileName << "! Please inform either Daniel or Jackson!" << endl;
			abort();
		}
	}

	inOBJ.close(); //Closes the file since we finished reading from it

	/* Sorts the data into the actual faces of the mesh */
	numFaces = faceData.size() / 9;

	for (int i = 0; i < numFaces; i++)
	{
		Face newFace;
		Vertex newVertices[3];

		for (int j = 0; j < 3; j++)
		{
			int posNumber = faceData[(i * 9) + (j * 3)] - 1;
			int uvNumber = faceData[(i * 9) + (j * 3) + 1] - 1;
			int normalNumber = faceData[(i * 9) + (j * 3) + 2] - 1;

			//Ensures that position data was supplied by the modelling program
			if (posData.size() > 0)
			{
				newVertices[j].position.x = posData[posNumber * 3];
				newVertices[j].position.y = posData[(posNumber * 3) + 1];
				newVertices[j].position.z = posData[(posNumber * 3) + 2];
			}

			//Ensures that uv data was supplied
			if (uvData.size() > 0)
			{
				newVertices[j].uvCoord.x = uvData[uvNumber * 2];
				newVertices[j].uvCoord.y = uvData[(uvNumber * 2) + 1];
			}

			//Ensures that normal data was supplied
			if (normalData.size() > 0)
			{
				newVertices[j].normal.x = normalData[(normalNumber * 3)];
				newVertices[j].normal.y = normalData[(normalNumber * 3) + 1];
				newVertices[j].normal.z = normalData[(normalNumber * 3) + 2];
			}
		}

		newFace = Face(newVertices[0], newVertices[1], newVertices[2]);
		faces.push_back(newFace);
	}
	//orderVertices();
}

void Mesh::output() const
{
	cout << "============ MESH ===============" << endl;
	cout << "NumFaces = " << numFaces << endl;
	cout << "NumVerts = " << numFaces * 3 << endl;

	for (int i = 0; i < numFaces; i++)
		faces[i].output();
}

//Need to order the vertices in a counter clockwise direction, basically calculates the z direction of the face normal (towards or away from the camera) and rewinds the vertices if needed
void Mesh::orderVertices()
{
	for (int i = 0; i < numFaces; i++)
	{
		glm::vec3 aToB = glm::vec3(faces[i].vertices[1].position.x - faces[i].vertices[0].position.x, faces[i].vertices[1].position.y - faces[i].vertices[0].position.y, 0);
		glm::vec3 bToC = glm::vec3(faces[i].vertices[2].position.x - faces[i].vertices[1].position.x, faces[i].vertices[2].position.y - faces[i].vertices[1].position.y, 0);

		float zValue = aToB.x * bToC.y - aToB.y * bToC.x;

		/* Checks if the z-direction of the cross product of the vertex vectors is -ve. If it is, the face needs to be rewound which just involves switching 2 of the vertices */
		if (zValue < 0.0f)
		{
			Vertex temp = faces[i].vertices[0];

			faces[i].vertices[0] = faces[i].vertices[2];
			faces[i].vertices[2] = temp;
		}
	}
}