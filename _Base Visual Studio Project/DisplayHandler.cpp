#include "DisplayHandler.h"

int DisplayHandler::windowWidth = 800;
int DisplayHandler::windowHeight = 800;
float DisplayHandler::aspectRatio = 1.0f;
int DisplayHandler::mousePosX = 0;
int DisplayHandler::mousePosY = 0;
bool DisplayHandler::keyDown[256];

//Sets up all of the OpenGL states
void DisplayHandler::init()
{
	//Sets the clear colour for the window
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//Enabling GL states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enabling and initializing the DevIL image loader
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}

//The draw function
void DisplayHandler::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GM::game()->updateActiveState();
	glutSwapBuffers();
}

//Called when a key is pressed
void DisplayHandler::checkKeyDown(unsigned char key, int x, int y)
{
	keyDown[key] = true;
}

//Called when a key is released
void DisplayHandler::checkKeyUp(unsigned char key, int x, int y)
{
	keyDown[key] = false;
}

//Called when the program is idle
void DisplayHandler::idle()
{
}

//Called every frame?
void DisplayHandler::timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY, timer, 0);
}

//Called when the window is resized
void DisplayHandler::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	aspectRatio = float(w) / float(h);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float stretch = getOrthoStretch();
	glOrtho(-10.0f * getOrthoStretch() , 10.0f * getOrthoStretch(), -10.0f, 10.0f, 0.10f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Called when any mouse button is clicked
void DisplayHandler::checkMouseClick(int button, int state, int x, int y)
{

}

//Called when the mouse is moved actively
void DisplayHandler::checkMouseMotion(int x, int y)
{

}

//Called when the mouse is passively moved
void DisplayHandler::checkMousePassiveMotion(int x, int y)
{
	mousePosX = x;
	mousePosY = y;
}

//Returns if the key is down that frame or not
bool DisplayHandler::getKey(unsigned char key)
{
	return keyDown[key];
}

//Returns the screen width
int DisplayHandler::getWidth() {
	return windowWidth;
}

//Returns the screen height
int DisplayHandler::getHeight() {
	return windowHeight;
}

//Returns the aspect ratio
float DisplayHandler::getAspectRatio() {
	return aspectRatio;
}

//Returns the value the sides of the orthographic window should be multiplied by to correct "aspect ratio"
float DisplayHandler::getOrthoStretch()
{
	float result;
	result = ((aspectRatio - 1) * 0.5) + 1;
	return result;
}

//Returns the cursor position X
int DisplayHandler::getMouseX() {
	return mousePosX;
}

//Returns the cursor position Y
int DisplayHandler::getMouseY() {
	return mousePosY;
}

float DisplayHandler::getDeltaTime() {
	return FRAME_DELAY;
}

float DisplayHandler::degToRad(float angle) {
	return angle * (3.1415f / 180.0f);
}

float DisplayHandler::radToDeg(float angle) {
	return angle * (180.0f / 3.1415f);
}