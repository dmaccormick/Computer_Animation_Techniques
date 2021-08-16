#include "GameState.h"
#include "DisplayHandler.h"
#include "Collision.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <istream>

int GameState::numStates = 0;

//Constructor: increments the static numStates variable, also stores the time this state is being laoded using chrono functions
GameState::GameState()
{
	stateNumber = numStates;
	numStates++;
	timeOfStateStart = high_resolution_clock::now();
}

//Returns the state ID
int GameState::getStateNumber() const {
	return stateNumber;
}

//Returns the time in seconds as a float of when the state was loaded
float GameState::getTimeOfStateStart() const  
{
	high_resolution_clock::time_point timeProgramStart; //Does this default to 0?
	duration<float> dur = timeOfStateStart - timeProgramStart;
	return duration_cast<seconds>(dur).count();
}

//Returns how long in seconds that is state has been loaded
float GameState::getTimeOnState() const 
{
	high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	duration<float> timeElapsed = timeNow - timeOfStateStart;
	seconds timeElapsedSeconds = duration_cast<seconds>(timeElapsed);

	return (float)timeElapsedSeconds.count();
}

//Returns how many states there are total
int GameState::getTotalNumStates() {
	return numStates;
}


/* ---------------------------------------------------------------- */
/* ----- Homework state to be used for all homework questions -----*/
/* ---------------------------------------------------------------- */
std::vector<glm::vec3> loadPoints(const string& fileName)
{
	//Opens the file
	std::ifstream inFile(fileName);

	//Error check
	if (!inFile)
	{
		std::cout << "The file specified at the path (" << fileName << ") could not be opened to load the path! Exiting!" << std::endl;
		abort();
	}

	string nextLine;
	std::vector<glm::vec3> inPath;

	//Reads in the file
	while (getline(inFile, nextLine))
	{
		glm::vec3 tempPoint;
		sscanf_s(nextLine.c_str(), "%f, %f, %f", &tempPoint.x, &tempPoint.y, &tempPoint.z);
		inPath.push_back(tempPoint);
	}

	//Closes file for safety
	inFile.close();

	//Returns the path
	return inPath;
}

void savePoints(const string& fileName, std::vector<glm::vec3> path)
{
	//Opens file
	std::ofstream outFile(fileName);

	//Error check
	if (!outFile)
	{
		std::cout << "The file specified at the path (" << fileName << ") could not be opened to save the path! Exiting!" << std::endl;
		abort();
	}

	//Outputs the path to the file
	for (int i = 0; i < path.size(); i++)
		outFile << path[i].x << ", " << path[i].y << ", " << path[i].z << endl;
		
	//Closes file for safety
	outFile.close();
}

void drawPath(std::vector<glm::vec3> path)
{
	//Draws all of the points along the path
	for (int i = 0; i < path.size(); i++)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		glPointSize(10.0f);

		glBegin(GL_POINTS);
			glVertex3f(path[i].x, path[i].y, path[i].z);
		glEnd();

		glPointSize(1.0f);
	}

	//Draws the line segments between the points...does this segment by segment
	for (int i = 0; i < path.size() - 1; i++)
	{
		//Draws the line segment between the points
		glColor3f(0.0f, 1.0f, 0.0f);
		glLineWidth(3.0f);

		glBegin(GL_LINES);
		{
			glVertex3f(path[i].x, path[i].y, path[i].z);
			glVertex3f(path[i + 1].x, path[i + 1].y, path[i + 1].z);
		}
		glEnd();

		glLineWidth(1.0f);
	}
}

void HomeworkState::load()
{
	//The location of the file
	fileName = "./res/path.txt";

	//Loads the points in
	path = loadPoints(fileName);

	//The interpolating parameter is initially set to 0
	t = 0;

	//Time since last point creation is initially set to 0
	timeSinceLastPointCreation = 0.0f;

	//Mode is initially set to path creation mode
	pathCreationMode = true;

	//Starts at the first segment
	currentSegment = 0;

	//Set up the monkey object
	monkey.setMesh(MESH_MONKEY);
	monkey.setColour(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	//Set up the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
	gluLookAt(0, 3, 10, 0, 0, 0, 0, 1, 0);
}

void HomeworkState::update()
{
	//Set up the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
	gluLookAt(0, 3, 10, 0, 0, 0, 0, 1, 0);

	if (pathCreationMode) //If in path creation mode
	{
		timeSinceLastPointCreation += DH::getDeltaTime();

		//Adds a new point to the path if the user clicked the mouse
		if (previousMouseClick != DH::getLastMouseClick() && timeSinceLastPointCreation > 0.5f)
		{
			timeSinceLastPointCreation = 0;

			//Gets the last mouse click
			previousMouseClick = DH::getLastMouseClick();

			//Converts the mouse click position (in pixels from 0-800) to screen space (from -10 to 10)
			glm::vec2 screenPosition;

			int halfWidth = DH::getWidth() / 2;
			int halfHeight = DH::getHeight() / 2;

			if (previousMouseClick.position.x < halfWidth)
				screenPosition.x = (-10.0f) + (previousMouseClick.position.x / halfWidth)*(10.0f);
			else if (previousMouseClick.position.x == halfWidth)
				screenPosition.x = 0.0f;
			else if (previousMouseClick.position.x > halfWidth)
				screenPosition.x = ((previousMouseClick.position.x - halfWidth) / (halfWidth)) * (10.0f);

			if (previousMouseClick.position.y < halfHeight)
				screenPosition.y = (10.0f) + (previousMouseClick.position.y / halfWidth)*(-10.0f);
			else if (previousMouseClick.position.y == halfHeight)
				screenPosition.y = 0.0f;
			else if (previousMouseClick.position.y > halfHeight)
				screenPosition.y = ((previousMouseClick.position.y - halfHeight) / (halfHeight)) * (-10.0f);

			//Adds the new point to the path
			path.push_back(glm::vec3(screenPosition.x, screenPosition.y, -1.0f));
		}

		//Clears the path if you hit 'r'
		if (DH::getKey('r'))
			path.clear();

		//Saves the path if you hit 's'
		if (DH::getKey('s'))
			savePoints(fileName, path);

		//Switches to LERP mode when 2 is pressed
		if (DH::getKey('2'))
		{
			pathCreationMode = false;

			savePoints(fileName, path); //Autosaves path when switching to LERP mode
			monkey.setPosition(path[0]); //Moves the monkey to the first point
			t = 0; //Resets the interpolating parameter
			currentSegment = 0; //Resets the current LERP segment
		}
	}
	else
	{
		//Increment t by dt
		t += DH::getDeltaTime();

		//Resets t to 0 and increments the segment
		if (t >= 1.0f)
		{
			t = 0.0f;

			if (currentSegment < path.size() - 1)
				currentSegment++;
		}

		//LERPS the monkey's position and stops at the last point
		if (currentSegment < path.size() - 1)
			monkey.setPosition(MathHelper::LERP(path[currentSegment], path[currentSegment + 1], t));

		//Updates and draws the monkey
		monkey.update(DH::getDeltaTime());

		//Switches to path creation mode when 1 is pressed
		if (DH::getKey('1'))
			pathCreationMode = true;
	}
	
	//Draws the path as connected points and line segments if there any points to draw
	if (path.size() > 0)
		drawPath(path);
}