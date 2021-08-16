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
glm::vec3 getRandPosition() {
	return glm::vec3(float(rand() % 19 + (-9)), float(rand() % 19 + (-9)), 0.0f);
}

glm::vec3 getRandColour() 
{
	return glm::vec3(float(rand() / float(RAND_MAX)), float(rand() / float(RAND_MAX)), float(rand() / float(RAND_MAX)));
}

void HomeworkState::load()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	
	//Seed random number generator
	srand(time(0));

	//Init all of the objects
	for (int i = 0; i < 50; i++)
		objects[i] = Object(getRandPosition(), glm::vec3(float(DH::getMouseX()), float(DH::getMouseY()), 0.0f), getRandColour());
}

void HomeworkState::update()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	//Converts the mouse position (in pixels from 0-800) to screen space (from -10 to 10)
	glm::vec2 screenPosition(0.0f, 0.0f);

	float halfWidth = DH::getWidth() / 2.0f;
	float halfHeight = DH::getHeight() / 2.0f;

	if (DH::getMouseX() < halfWidth)
		screenPosition.x = (-10.0f) + (float(DH::getMouseX()) / halfWidth)*(10.0f);
	else if (DH::getMouseX() == halfWidth)
		screenPosition.x = 0.0f;
	else if (DH::getMouseX() > halfWidth)
		screenPosition.x = ((float(DH::getMouseX()) - halfWidth) / (halfWidth)) * (10.0f);

	if (DH::getMouseY() < halfHeight)
		screenPosition.y = (10.0f) + (float(DH::getMouseY()) / halfWidth)*(-10.0f);
	else if (DH::getMouseY() == halfHeight)
		screenPosition.y = 0.0f;
	else if (DH::getMouseY() > halfHeight)
		screenPosition.y = ((float(DH::getMouseY()) - halfHeight) / (halfHeight)) * (-10.0f);
	//End of converting mouse

	//Switches between seeking and fleeing//
	if (DH::getKey('s'))
	{
		for (int i = 0; i < 50; i++)
			objects[i].seeking = true;
	}
	else if (DH::getKey('f'))
	{
		for (int i = 0; i < 50; i++)
			objects[i].seeking = false;
	}
	
	//Updates all of the objects
	for (int i = 0; i < 50; i++)
		objects[i].update(glm::vec3(float(screenPosition.x), float(screenPosition.y), 0.0f));
}