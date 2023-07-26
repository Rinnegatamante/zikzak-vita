/*
 * Project: zikzak
 * Filename: Gamestate.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Gamestate handles the main game loop delay and the operation state
 *   (playing, won, game over etc.)
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#include <ctime>
#include "Gfx.h"
#include "Sound.h"
#include "Level.h"

/** The operation states define the different situations in the game */
typedef enum Operationstates {
	TITLE = 0,
	PLAY,
	GAMEOVER,
	WON,
} Operationstate;

class Gamestate
{
private:
	/** startTime holds the time stamp of the moment when the level is
	 *  started */
	long startTime;
	
	/** currentState stores the actual gamestate */
	Operationstate currentState;

	/** Returns the number of internal ticks left until the loop delay
	 *  is over */
	Uint32 tickTimeLeft();
	
public:
	/** Operation state getter */
	Operationstate getOperationState();
	
	/** Operation state setter */
	void setOperationState(Operationstate state);

	/** This method is called in the main game loop and waits some ticks */
	void loopDelay();
	
	/** Resets the game timer (the one that says how many seconds you
	 *  have left to solve the level */
	void resetTimer();
	
	/** getTimeLeft gets the number of seconds intended to solve the level
	 *  from the level object and calculates how many seconds you have left */
	int getTimeLeft(Level &l);
	
	/** Default constructor */
	Gamestate();
	
	/** Default destructor */
	virtual ~Gamestate();
};

#endif /*GAMESTATE_H_*/
