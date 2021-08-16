#include "GameState.h"
#include "DisplayHandler.h"
#include "glm\gtx\string_cast.hpp"
#include "Collision.h"

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
/*
	Daniel MacCormick - 100580519
	Computer Animation: Algorithms & Techniques
	Dr. Andrew Hogue

	Question Number	 =	NUMBER
	Question Stage	 =	STAGE
*/
void HomeworkState::load()
{
}

void HomeworkState::update()
{
}