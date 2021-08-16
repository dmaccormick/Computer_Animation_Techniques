// Core Libraries
#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <string>
#include <math.h>
#include <glm/vec2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stdlib.h>

// 3rd Party Libraries
#include <GLUT\glut.h>

//Our libraries
#include "DisplayHandler.h"
#include "AssetManager.h"
#include "Mesh.h"

int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);
	glutInitWindowSize(DisplayHandler::getWidth(), DisplayHandler::getHeight());
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Daniel MacCormick (100580519) - Algorithms Homework");

	//Setup function call backs
	glutDisplayFunc(DisplayHandler::update);
	glutKeyboardFunc(DisplayHandler::checkKeyDown);
	glutKeyboardUpFunc(DisplayHandler::checkKeyUp);
	glutIdleFunc(DisplayHandler::idle);
	glutReshapeFunc(DisplayHandler::reshapeWindow);
	glutMouseFunc(DisplayHandler::checkMouseClick);
	glutMotionFunc(DisplayHandler::checkMouseMotion);
	glutPassiveMotionFunc(DisplayHandler::checkMousePassiveMotion);
	glutTimerFunc(1, DisplayHandler::timer, 0);
	
	//Perform initialization on the display and the assets themselves
	DisplayHandler::init();
	AM::assets()->loadAssets();
	GM::game()->setActiveState(STATE_HOMEWORK);

	//Start the event handler
	glutMainLoop();

	return 0;
}