#include "GameState.h"
#include "DisplayHandler.h"
#include "glm\gtx\string_cast.hpp"
#include "Collision.h"
#include "MathHelper.h"

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
void HomeworkState::load()
{
	//Load all of the meshes in
	morphTargets.push_back(Mesh("./res/mesh/PoseA_Base.obj"));
	morphTargets.push_back(Mesh("./res/mesh/PoseB_CoreTwist.obj"));
	morphTargets.push_back(Mesh("./res/mesh/PoseC_Crouch.obj"));
	morphTargets.push_back(Mesh("./res/mesh/PoseD_Star.obj"));
	morphTargets.push_back(Mesh("./res/mesh/PoseE_CraneKick.obj"));
	morphTargets.push_back(Mesh("./res/mesh/PoseF_Lunge.obj"));
	morphTargets.push_back(Mesh("./res/mesh/PoseG_Dance.obj"));

	for (unsigned int i = 0; i < morphTargets.size(); i++)
		meshData.push_back(morphTargets[i].getVertices());

	morphedMesh = morphTargets[0].getVertices();

	//Load the textures
	sniperDiffuse = Texture2D("./res/img/sniper_Diffuse.png");
	ground = Texture2D("./res/img/ground.jpg");

	//Set the default for the variables
	rotation = 0.0f;
	activeModel = 0;
	targetModel = 0;
	t = 0.0f;
	speedMultiplier = 1.0f;

	//Set up GL states
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
}

void HomeworkState::update()
{
	//Set up the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1.0f, 0.1f, 50.0f);
	gluLookAt(0.0f, 1.25, 4.0f, 0, 0.25f, 0, 0, 1, 0);

	//Rotate the model based on key presses
	if (DH::getKey('a'))
		rotation--;
	else if (DH::getKey('d'))
		rotation++;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(rotation, 0.0f, 1.0f, 0.0f);

	//Switch the active model based on key presses
	if (DH::getKey('1'))
	{
		targetModel = 0;
		t = 0.0f;
	}
	else if (DH::getKey('2'))
	{
		t = 0.0f;
		targetModel = 1;
	}
	else if (DH::getKey('3'))
	{
		t = 0.0f;
		targetModel = 2;
	}
	else if (DH::getKey('4'))
	{
		t = 0.0f;
		targetModel = 3;
	}
	else if (DH::getKey('5'))
	{
		t = 0.0f;
		targetModel = 4;
	}
	else if (DH::getKey('6'))
	{
		t = 0.0f;
		targetModel = 5;
	}
	else if (DH::getKey('7'))
	{
		t = 0.0f;
		targetModel = 6;
	}

	if (DH::getKey('r'))
		speedMultiplier += 0.1f;
	else if (DH::getKey('f'))
		speedMultiplier -= 0.1f;

	//Increment t
	if (activeModel != targetModel)
		t += (DH::getDeltaTime() * speedMultiplier);

	//Draw the ground plane
	ground.bind();
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-2.0f, -2.5f, -2.0f);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(2.0f, -2.5f, -2.0f);
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(2.0f, -2.5f, 2.0f);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-2.0f, -2.5f, 2.0f);
	}
	glEnd();

	//Set the morphed mesh to be the lerp result of the current and target meshes
	if (targetModel != activeModel)
	{
		for (unsigned int i = 0; i < morphedMesh.size(); i++)
		{
			morphedMesh[i].position = LERP(meshData[activeModel][i].position, meshData[targetModel][i].position, t);
			morphedMesh[i].uvCoord = LERP(meshData[activeModel][i].uvCoord, meshData[targetModel][i].uvCoord, t);
			morphedMesh[i].normal = LERP(meshData[activeModel][i].normal, meshData[targetModel][i].normal, t);
		}
	}
	
	//Cap the t value
	if (t > 1.0f)
	{
		activeModel = targetModel;
		t = 1.0f;
	}
	
	//Draw the morphed mesh
	sniperDiffuse.bind();
	drawMorphedMesh();

	//Output data to the screen
	DH::drawText2D(("CURRENT = " + std::to_string(activeModel) + "   TARGET = " + std::to_string(targetModel) + "   SPEED = " + std::to_string(speedMultiplier)), glm::vec4(1.0f), 0, 0);
}

void HomeworkState::drawMorphedMesh()
{
	glBegin(GL_TRIANGLES);
	{
		for (unsigned int i = 0; i < morphedMesh.size(); i++)
		{
			glNormal3f(morphedMesh[i].normal.x, morphedMesh[i].normal.y, morphedMesh[i].normal.z);
			glTexCoord2f(morphedMesh[i].uvCoord.x, morphedMesh[i].uvCoord.y);
			glVertex3f(morphedMesh[i].position.x, morphedMesh[i].position.y, morphedMesh[i].position.z);
		}
	}
	glEnd();
}