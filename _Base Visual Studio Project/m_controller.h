#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <vector>
#define BUTTON_A XINPUT_GAMEPAD_A
#define BUTTON_B XINPUT_GAMEPAD_B
#define BUTTON_X XINPUT_GAMEPAD_X
#define BUTTON_Y XINPUT_GAMEPAD_Y
#define DPAD_LEFT XINPUT_GAMEPAD_DPAD_LEFT
#define DPAD_RIGHT XINPUT_GAMEPAD_DPAD_RIGHT
#define DPAD_UP XINPUT_GAMEPAD_DPAD_UP
#define DPAD_DOWN XINPUT_GAMEPAD_DPAD_DOWN
#define BUTTON_LB XINPUT_GAMEPAD_LEFT_SHOULDER
#define BUTTON_RB XINPUT_GAMEPAD_RIGHT_SHOULDER
#define BUTTON_LS XINPUT_GAMEPAD_LEFT_THUMB
#define BUTTON_RS XINPUT_GAMEPAD_RIGHT_THUMB
#define BUTTON_SELECT XINPUT_GAMEPAD_BACK
#define BUTTON_START XINPUT_GAMEPAD_START
static float degToRad = 3.14159f / 180.0f;
static float radToDeg = 180.0f / 3.14159f;
static int numControllers = 0;
int getNextAvailableControllerId();
	
		

class MController
{
public:
	MController() {}
	MController(int id);
	int		index;
	XINPUT_STATE state;
	void	getInputs();
	bool	checkButton(DWORD button);
	bool	isConnected();
	std::vector<DWORD> inputs;
	float	getLeftStickAngle();
	float	getRightStickAngle();
	float	lT;
	float	rT;
	float	lX = 0.0f,
			lY = 0.0f,
			rX = 0.0f,
			rY = 0.0f;


};

