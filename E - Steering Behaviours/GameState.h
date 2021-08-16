#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>
#include <ctime>
#include <chrono>
#include <stdlib.h> //Weird thing where stdlib HAS to be included and HAS to be above glut.h due to glut redefining exit in a weird way
#include <GLUT\glut.h>
#include "Kinematic.h"
#include "m_controller.h"
#include "Player.h"
struct MouseClick
{
	glm::vec2 position = glm::vec2(0, 0);
	int state = 0;
	int button = 0;

	void output()
	{
		printf("POS: %f, %f\n", position.x, position.y);
		printf("STATE: %d\n", state);
		printf("BUTTON: %d\n\n", button);
	}

	friend bool operator !=(MouseClick& a, MouseClick& b)
	{
		return !(a.position == b.position);
	}
};
static MouseClick lastMouseClick;

using namespace std::chrono;

//All of the states in the game, state number is represented by a word (ie: STATE_TEST represents 0)
enum STATE : int
{
	STATE_HOMEWORK,

	NUM_STATES
};

/*
	GameState class
	- Each part of the game (Menu, credits, main game, etc) are all unique states
	- All unique states will be their own class that inherits from this one
	- Polymorphism allows for a pointer to a GameState within GameManager to call the virtual load and update functions for each unique state
*/
class GameState
{
public:
	GameState();
	~GameState() {}

	virtual void load() {} //Load and initialize all of the assets used in each specific game state when they are first set up (called when game state is switched)
	virtual void update() {}  //Handle all movement and drawing code for each specific game state every frame (called every frame, main loop essentially)

	int getStateNumber() const; //Return the current state number
	float getTimeOfStateStart() const; //Returns the time in seconds of when this state was switched to
	float getTimeOnState() const; //Returns the time in seconds that this state has been loaded
	int getTotalNumStates(); //Returns the total number of states in the game

private:
	int stateNumber; //Essentially the ID of the game state
	high_resolution_clock::time_point timeOfStateStart; //The time in seconds of when this state was loaded as a float
	static int numStates; //A static int that is incremented every time a new state is created in order to keep track of how many states are loaded
};

struct Object
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 movementDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
	float movementSpeed = 0.05f;
	bool seeking = true;

	Object() {}
	Object(glm::vec3 startPosition, glm::vec3 startTarget, glm::vec3 objColour) : position(startPosition), targetPosition(startTarget), colour(objColour) {}

	void update(glm::vec3 targetPosition)
	{
		movementDirection = targetPosition - position;
		movementDirection = glm::normalize(movementDirection);

		//Moves towards the target if seeking, away if fleeing
		if (seeking)
			movementDirection *= movementSpeed;
		else
			movementDirection *= -movementSpeed;

		position += movementDirection;

		glColor3f(colour.x, colour.y, colour.z);
		glPointSize(10.0f);
		glBegin(GL_POINTS);
			glVertex3f(position.x, position.y, position.z);
		glEnd();
		glPointSize(1.0f);
	}
};

//The state to be used for all of the homework questions
class HomeworkState : public GameState
{
public:
	HomeworkState() {}
	~HomeworkState() {}

	void load();
	void update();

private:
	Object objects[50];
};

#endif
