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
/* ----- Homework state to be used for all homework questions ----- */
/* ---------------------------------------------------------------- */
void HomeworkState::drawPath()
{
	//Draws all of the points along the path
	for (unsigned int i = 0; i < path.size(); i++)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		glPointSize(10.0f);

		glBegin(GL_POINTS);
			glVertex3f(path[i].x, path[i].y, path[i].z);
		glEnd();

		glPointSize(1.0f);
	}

	//The next point to be drawn, will be different depending on the interpolation type
	glm::vec3 segmentPoint;
	glm::vec3 segmentColour;

	//Draws the dotted line segments between the points
	if (currentInterpolationType == InterpolationType::BEZIER)
	{
		for (unsigned int segmentNumber = 0; segmentNumber < path.size() - 1; segmentNumber += 3)
		{
			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				if (path.size() - segmentNumber == 1)
				{
					segmentPoint = MathHelper::BEZIER(path[segmentNumber], t);
					segmentColour = MathHelper::BEZIER(pathColours[segmentNumber], t);
				}
				else if (path.size() - segmentNumber == 2)
				{
					segmentPoint = MathHelper::BEZIER(path[segmentNumber], path[segmentNumber + 1], t);
					segmentColour = MathHelper::BEZIER(pathColours[segmentNumber], pathColours[segmentNumber + 1], t);
				}
				else if (path.size() - segmentNumber == 3)
				{
					segmentPoint = MathHelper::BEZIER(path[segmentNumber], path[segmentNumber + 1], path[segmentNumber + 2], t);
					segmentColour = MathHelper::BEZIER(pathColours[segmentNumber], pathColours[segmentNumber + 1], pathColours[segmentNumber + 2], t);
				}
				else
				{
					segmentPoint = MathHelper::BEZIER(path[segmentNumber], path[segmentNumber + 1], path[segmentNumber + 2], path[segmentNumber + 3], t);
					segmentColour = MathHelper::BEZIER(pathColours[segmentNumber], pathColours[segmentNumber + 1], pathColours[segmentNumber + 2], pathColours[segmentNumber + 3], t);
				}

				//Draws the point
				glColor3f(segmentColour.x, segmentColour.y, segmentColour.z);
				glPointSize(2.0f);

				glBegin(GL_POINTS);
				glVertex3f(segmentPoint.x, segmentPoint.y, segmentPoint.z);
				glEnd();

				glPointSize(1.0f);
			}
		}
	}
	else
	{
		for (unsigned int segmentNumber = 0; segmentNumber < path.size() - 1; segmentNumber++)
		{
			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				if (currentInterpolationType == InterpolationType::LERP)
				{
					segmentPoint = MathHelper::LERP(path[segmentNumber], path[segmentNumber + 1], t);
					segmentColour = MathHelper::LERP(pathColours[segmentNumber], pathColours[segmentNumber + 1], t);
				}
				else if (currentInterpolationType == InterpolationType::CATMULL)
				{
					if (currentSegment < path.size() - 1)
					{
						if (path.size() == 1)
						{
							segmentPoint = path[0];
							segmentColour = pathColours[0];
						}
						else if (path.size() == 2)
						{
							segmentPoint = MathHelper::CATMULL(path[0], path[0], path[1], path[1], t);
							segmentColour = MathHelper::CATMULL(pathColours[0], pathColours[0], pathColours[1], pathColours[1], t);
						}
						else if (path.size() == 3)
						{
							if (segmentNumber == 0)
							{
								segmentPoint = MathHelper::CATMULL(path[0], path[0], path[1], path[2], t);
								segmentColour = MathHelper::CATMULL(pathColours[0], pathColours[0], pathColours[1], pathColours[2], t);
							}
							else if (segmentNumber == 1)
							{
								segmentPoint = MathHelper::CATMULL(path[0], path[1], path[2], path[2], t);
								segmentColour = MathHelper::CATMULL(pathColours[0], pathColours[1], pathColours[2], pathColours[2], t);
							}
						}
						else
						{
							if (segmentNumber == 0) //If at the first segment, uses the first point twice
							{
								segmentPoint = MathHelper::CATMULL(path[segmentNumber], path[segmentNumber], path[segmentNumber + 1], path[segmentNumber + 2], t);
								segmentColour = MathHelper::CATMULL(pathColours[segmentNumber], pathColours[segmentNumber], pathColours[segmentNumber + 1], pathColours[segmentNumber + 2], t);
							}
							else if (segmentNumber == path.size() - 2) //If at the last segment, uses the last point twice
							{
								segmentPoint = MathHelper::CATMULL(path[segmentNumber - 1], path[segmentNumber], path[segmentNumber + 1], path[segmentNumber + 1], t);
								segmentColour = MathHelper::CATMULL(pathColours[segmentNumber - 1], pathColours[segmentNumber], pathColours[segmentNumber + 1], pathColours[segmentNumber + 1], t);
							}
							else //At any other segment, uses 4 points as normal
							{
								segmentPoint = MathHelper::CATMULL(path[segmentNumber - 1], path[segmentNumber], path[segmentNumber + 1], path[segmentNumber + 2], t);
								segmentColour = MathHelper::CATMULL(pathColours[segmentNumber - 1], pathColours[segmentNumber], pathColours[segmentNumber + 1], pathColours[segmentNumber + 2], t);
							}
						}
					}
				}
				//Draws the point
				glColor3f(segmentColour.x, segmentColour.y, segmentColour.z);
				glPointSize(2.0f);

				glBegin(GL_POINTS);
				glVertex3f(segmentPoint.x, segmentPoint.y, segmentPoint.z);
				glEnd();

				glPointSize(1.0f);
			}
		}
	}
}

void HomeworkState::load()
{
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
	
	//Sets the base interpolation type to LERP
	currentInterpolationType = InterpolationType::LERP;

	//Sets the base to be not connected end points
	connectedEndPoints = false;
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

			//Adds a random colour for the point that will get interpolated along the curve
			pathColours.push_back(MathHelper::randomVec3(0.0f, 1.0f));
		}

		//Clears the path if you hit 'r'
		if (DH::getKey('r'))
		{
			path.clear();
			pathColours.clear();
			currentSegment = 0;
		}

		//Switches to interpolation mode when 2 is pressed
		if (DH::getKey('2'))
		{
			if (path.size() > 0) //Can only switch out of path creation mode if there is a path
			{
				pathCreationMode = false;

				monkey.setPosition(path[0]); //Moves the monkey to the first point
				t = 0; //Resets the interpolating parameter
				currentSegment = 0; //Resets the current interpolation segment
			}
		}
	}
	else //If in interpolation mode
	{
		//Increment t by dt
		t += DH::getDeltaTime();

		//Resets t to 0 and increments the segment
		if (t >= 1.0f)
		{
			t = 0.0f;

			if (currentInterpolationType == InterpolationType::BEZIER)
				currentSegment += 3;
			else
				currentSegment++;

			if (currentSegment >= path.size() - 1)
			{
				monkey.setPosition(path[0]);
				currentSegment = 0;
			}
		}

		//Moves the monkey and draws the path depending on the interpolation type
		if (currentInterpolationType == InterpolationType::LERP)
		{
			//LERP's the monkey's position and stops at the last point
			if (currentSegment < path.size() - 1)
				monkey.setPosition(MathHelper::LERP(path[currentSegment], path[currentSegment + 1], t));
		}
		else if (currentInterpolationType == InterpolationType::CATMULL)
		{
			//Interpolates the monkey's position using catmull and stops at the last point
			if (currentSegment < path.size() - 1)
			{
				if (path.size() == 1)
					monkey.setPosition(path[0]);
				else if (path.size() == 2)
					monkey.setPosition(MathHelper::CATMULL(path[0], path[0], path[1], path[1], t));
				else if (path.size() == 3)
				{
					if (currentSegment == 0)
						monkey.setPosition(MathHelper::CATMULL(path[0], path[0], path[1], path[2], t));
					else if (currentSegment == 1)
						monkey.setPosition(MathHelper::CATMULL(path[0], path[1], path[2], path[2], t));
				}
				else
				{
					if (currentSegment == 0) //If at the first segment, uses the first point twice
						monkey.setPosition(MathHelper::CATMULL(path[currentSegment], path[currentSegment], path[currentSegment + 1], path[currentSegment + 2], t));
					else if (currentSegment == path.size() - 2) //If at the last segment, uses the last point twice
						monkey.setPosition(MathHelper::CATMULL(path[currentSegment - 1], path[currentSegment], path[currentSegment + 1], path[currentSegment + 1], t));
					else //At any other segment, uses 4 points as normal
						monkey.setPosition(MathHelper::CATMULL(path[currentSegment - 1], path[currentSegment], path[currentSegment + 1], path[currentSegment + 2], t));
				}
			}
		}
		else if (currentInterpolationType == InterpolationType::BEZIER)
		{
			if (path.size() - currentSegment == 1)
				monkey.setPosition(MathHelper::BEZIER(path[currentSegment], t));
			else if (path.size() - currentSegment == 2)
				monkey.setPosition(MathHelper::BEZIER(path[currentSegment], path[currentSegment + 1], t));
			else if (path.size() - currentSegment == 3)
				monkey.setPosition(MathHelper::BEZIER(path[currentSegment], path[currentSegment + 1], path[currentSegment + 2], t));
			else
				monkey.setPosition(MathHelper::BEZIER(path[currentSegment], path[currentSegment + 1], path[currentSegment + 2], path[currentSegment + 3], t));
		}

		//Updates and draws the monkey
		monkey.update(DH::getDeltaTime());

		//Switches to path creation mode when 1 is pressed
		if (DH::getKey('1'))
			pathCreationMode = true;

		//Resets the monkey's position if 'r' is pressed
		if (DH::getKey('r'))
		{
			monkey.setPosition(path[0]); //Moves the monkey to the first point
			t = 0; //Resets the interpolating parameter
			currentSegment = 0; //Resets the current interpolation segment
		}

		//Switches between connected and disconnected end points
		if (DH::getKey('u') && connectedEndPoints) //Disconnected
		{
			std::cout << "Switching to disconnected end points. Object will JUMP BACK to start of path" << std::endl;
			connectedEndPoints = false;
			pathColours.pop_back();
			path.pop_back();
		}
		else if (DH::getKey('i') && !connectedEndPoints) //Connected
		{
			std::cout << "Switching to connected end points. Object will INTERPOLATE BACK to start of path" << std::endl;
			path.push_back(path[0]);
			pathColours.push_back(path[0]);
			connectedEndPoints = true;
		}
	}

	//Draws the path using the current interpolating mode
	if (path.size() > 0)
		drawPath();

	//Switches between lerp, catmull, and bezier with L, C, and B
	if (DH::getKey('l'))
	{
		currentInterpolationType = InterpolationType::LERP;
		std::cout << "Switching interpolation type to LERP" << std::endl;
	}
	else if (DH::getKey('c'))
	{
		currentInterpolationType = InterpolationType::CATMULL;
		std::cout << "Switching interpolation type to CATMULL" << std::endl;
	}
	else if (DH::getKey('b'))
	{
		currentInterpolationType = InterpolationType::BEZIER;
		std::cout << "Switching interpolation type to BEZIER" << std::endl;
	}	
}