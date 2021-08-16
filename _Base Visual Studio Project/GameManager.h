#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <iostream>
#include "GameState.h"

/*
	GameManager class
	- Follows singleton pattern (see note below)
	- Handles the game states
	- Has an active state that can be loaded, updated, or changed

	Note:
	MUST CALL like this: GM::game()->
	Do not make another instance of this class!
*/
class GameManager
{
protected:
	GameManager(); //Protected constructor for singleton pattern

public:
	~GameManager();

	void setActiveState(STATE newActiveState); //Changes the active state
	void setDonePlaying(); //Call to end the program

	void loadActiveState(); //Loads the active state
	void updateActiveState(); //Updates the active state

	int getActiveState() const; //Returns the # of the active state
	bool getStillPlaying() const; //Returns if the program is over yet
	float getCurrentStateTime() const; //Returns the time elapsed on the current active state

	static GameManager* game(); //Singleton pattern, need to call this when accessing functions
	
private:
	static GameManager* inst; //Singleton pattern

	GameState* activeState; //Pointer to the active state, needed for the polymorphic ability to handle any state type
	STATE activeStateNumber; //Stores which state is currently active
	bool stillPlaying; //If the game is still running
};

typedef GameManager GM; //Typedef to make calling functions easier

#endif
