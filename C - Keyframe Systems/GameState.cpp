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
void HomeworkState::initKeyframes()
{
	//Ball position keyframes
	ballPos.setTotalNumFrames(163);
	ballPos.addKey(Keyframe<glm::vec3>(glm::vec3(1.0f, 2.0f, 10.0f), 0));
	ballPos.addKey(Keyframe<glm::vec3>(glm::vec3(1.0f, 2.0f, 10.0f), 25));
	ballPos.addKey(Keyframe<glm::vec3>(glm::vec3(1.0f, 2.0f, -8.0f), 112));
	ballPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.0f, 2.0f, -11.0f), 150));
	ballPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.0f, 0.0f, -11.0f), 162));

	//Flag position keyframes
	flagPos.setTotalNumFrames(163);
	flagPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.5f, 1.75f, -12.0f), 0));
	flagPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.5f, 1.75f, -12.0f), 25));
	flagPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.5f, 1.75f, -12.0f), 112));
	flagPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.5f, 1.75f, -12.0f), 150));
	flagPos.addKey(Keyframe<glm::vec3>(glm::vec3(5.5f, 3.5f, -12.0f), 162));

	//Club rotation keyframes
	clubRot.setTotalNumFrames(163);
	clubRot.addKey(Keyframe<glm::vec3>(glm::vec3(-15.0f, 0.0f, 0.0f), 0));
	clubRot.addKey(Keyframe<glm::vec3>(glm::vec3(15.0f, 0.0f, 0.0f), 25));
	clubRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 0.0f), 112));
	clubRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 0.0f), 150));
	clubRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 0.0f), 162));

	//Blades rotation keyframes
	bladesRot.setTotalNumFrames(163);
	bladesRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 0.0f), 0));
	bladesRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 158.0f), 25));
	bladesRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 707.0f), 112));
	bladesRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 948.0f), 150));
	bladesRot.addKey(Keyframe<glm::vec3>(glm::vec3(0.0f, 0.0f, 1024.0f), 162));
}

void HomeworkState::pauseAllKeyframes(bool paused)
{
	ballPos.setPaused(paused);
	flagPos.setPaused(paused);
	clubRot.setPaused(paused);
	bladesRot.setPaused(paused);
}

void HomeworkState::jumpTo(bool end)
{
	if (end)
	{
		ballPos.jumpToEnd();
		flagPos.jumpToEnd();
		clubRot.jumpToEnd();
		bladesRot.jumpToEnd();
	}
	else
	{
		ballPos.jumpToStart();
		flagPos.jumpToStart();
		clubRot.jumpToStart();
		bladesRot.jumpToStart();
	}
}

void HomeworkState::jumpOneFrame(bool forward)
{
	if (forward)
	{
		ballPos.jumpToNextKeyframe();
		flagPos.jumpToNextKeyframe();
		clubRot.jumpToNextKeyframe();
		bladesRot.jumpToNextKeyframe();
	}
	else
	{
		ballPos.jumpToPreviousKeyframe();
		flagPos.jumpToPreviousKeyframe();
		clubRot.jumpToPreviousKeyframe();
		bladesRot.jumpToPreviousKeyframe();
	}
}

void HomeworkState::playDirection(bool forward)
{
	ballPos.setPlaysForward(forward);
	flagPos.setPlaysForward(forward);
	clubRot.setPlaysForward(forward);
	bladesRot.setPlaysForward(forward);
}

void HomeworkState::setLooping(bool loops)
{
	ballPos.setLoops(loops);
	flagPos.setLoops(loops);
	clubRot.setLoops(loops);
	bladesRot.setLoops(loops);
}

void HomeworkState::load()
{
	//Set up the club object
	club.setPosition(-0.5f, 2.0f, 11.0f);
	club.setMesh(MESH_CLUB);
	club.setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	//Set up the ball object
	ball.setScale(0.1f, 0.1f, 0.1f);
	ball.setPosition(1.0, 2.0, 10.0f);
	ball.setMesh(MESH_BALL);
	ball.setColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//Set up the course object
	course.setMesh(MESH_COURSE);
	course.setColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	//Set up the windmill object
	windmill.setPosition(glm::vec3(0.0f, 2.0f, -3.0f));
	windmill.setMesh(MESH_WINDMILL);
	windmill.setColour(glm::vec4(1.0f, 0.55f, 0.5f, 1.0f));

	//Set up the windmill blades
	blades.setPosition(glm::vec3(0.5f, 2.25f, -1.5f));
	blades.setMesh(MESH_BLADES);
	blades.setColour(glm::vec4(0.45f, 0.25f, 0.2f, 1.0f));

	//Set up the flag
	flag.setPosition(glm::vec3(5.5f, 1.75f, -12.0f));
	flag.setMesh(MESH_FLAG);
	flag.setColour(glm::vec4(1.0f, 0.0f, 0.33f, 1.0f));

	//Set up the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1.0f, 0.1f, 1000.0f);
	gluLookAt(5, 15, 5, 0, 0, 0, 0, 1, 0);

	//Set up the keyframes
	initKeyframes();

	//Set up lighting
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	GLfloat light_position[] = { 0, 100, 0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL); // final polygon color will be based on glColor and glMaterial
	glEnable(GL_DEPTH_TEST);
}

void HomeworkState::update()
{
	//Set up the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1.0f, 0.1f, 1000.0f);
	gluLookAt(5, 15, 5, 0, 0, 0, 0, 1, 0);

	timeSinceLastFrameJump += DH::getDeltaTime();

	/* Control keyframe playback */
	if (timeSinceLastFrameJump > 0.15f)
	{
		//Jump back one keyframe or forward one keyframe
		if (DH::getKey('q'))
		{
			jumpOneFrame(false); //Back
			timeSinceLastFrameJump = 0;
		}
		else if (DH::getKey('e'))
		{
			jumpOneFrame(true); //Forward
			timeSinceLastFrameJump = 0;
		}
	}

	//Pause and unpause
	if (DH::getKey('p'))
		pauseAllKeyframes(true);
	else if (DH::getKey('o'))
		pauseAllKeyframes(false);

	//Jump to start and end
	if (DH::getKey('z')) //Start
		jumpTo(false);
	else if (DH::getKey('c')) //End
		jumpTo(true);

	//Reverse or forward playback
	if (DH::getKey('r'))
		playDirection(false); //Reverse
	else if (DH::getKey('f'))
		playDirection(true); //Forward

	//Control looping
	if (DH::getKey('l')) //loops
		setLooping(true);
	else if (DH::getKey('k')) //doesn't loop
		setLooping(false);
	/* Control keyframe playback */

	//Set the keyframed values for each of the gameobject
	ball.setPosition(ballPos.update(DH::getDeltaTime()));
	flag.setPosition(flagPos.update(DH::getDeltaTime()));
	club.setRotation(clubRot.update(DH::getDeltaTime()));
	blades.setRotation(bladesRot.update(DH::getDeltaTime()));

	//Update all of the gameobjects
	course.update(DH::getDeltaTime());
	club.update(DH::getDeltaTime());
	ball.update(DH::getDeltaTime());
	windmill.update(DH::getDeltaTime());
	blades.update(DH::getDeltaTime());
	flag.update(DH::getDeltaTime());
}