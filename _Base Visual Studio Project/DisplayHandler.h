#include <stdlib.h>
#include "GLUT\glut.h"
#include "IL\ilut.h"
#include "GameManager.h"

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

/*
	DisplayHandler class
	- Bunch of static functions that are used as the glut callback functions
	- Will most likely be replaced when switching to modern
	- Essentially everything that was in main contained within a class
*/
struct DisplayHandler
{
	static void init(); //Sets up all of the OpenGL states (ie: glEnable(GL_CULL_FACE))
	static void update(void); //The display callback function, called to draw things
	static void checkKeyDown(unsigned char key, int x, int y); //The keyboard down callback function, called when a key is pressed
	static void checkKeyUp(unsigned char key, int x, int y); //The keyboard up callback function, called when a key is released
	static void idle(void); //The idle callback function, called when the program is idle
	static void timer(int value); //The timer callback function, called every frame
	static void reshapeWindow(int w, int h); //The reshape window callback function, called when the game window is resized
	static void checkMouseClick(int button, int state, int x, int y); //The mouse click callback function, called when any mouse button is clicked
	static void checkMouseMotion(int x, int y); //The active mouse motion callback function, called when moving while clicking?
	static void checkMousePassiveMotion(int x, int y); //The passive mouse motion callback function, called when just moving the mouse

	static bool getKey(unsigned char key); //Returns if the key is being pressed that frame, smoother than checkKeyDown
	static int getWidth(); //Returns the window width in pixels
	static int getHeight(); //Returns the window height in pixels
	static float getAspectRatio(); //Returns the aspect ratio (width / height)
	static float getOrthoStretch(); //Returns the value the sides of the orthographic window should be multiplied by to correct "aspect ratio"
	static int getMouseX(); //Returns the mouse cursor's X position
	static int getMouseY(); //Returns the mouse cursor's Y position
	static float getDeltaTime(); //Returns dt
	static float degToRad(float angle); //Converts an angle from degrees to radians and returns it
	static float radToDeg(float angle); //Converts an angle from radians to degrees and returns it

	static const int FRAMES_PER_SECOND = 60; //Const representing how many FPS we want in the window
	static const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; //The frame delay in milliseconds, passed to the timer function
	static int windowWidth; //The window width in pixels
	static int windowHeight; //The window height in pixels
	static float aspectRatio; //The aspect ratio, stored as float
	static int mousePosX; //The mouse cursor's X position
	static int mousePosY; //The mouse cursor's Y position

	static bool keyDown[256]; //Array of all of the keys, holding if they are down that frame or not
};

typedef DisplayHandler DH;

#endif