/*
 * Project: zikzak
 * Filename: Input.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   The Input class has only one important method, poll(). Keyboard and
 *   Mouse events are processed according the the different gamestates.
 */

#ifndef INPUT_H_
#define INPUT_H_
#include "Gfx.h"
#include "Player.h"
#include "Sound.h"
#include "Gamestate.h"
#include "Coord3d.h"
#include "Level.h"

class Input
{
private:
	/** Store 0 or 1 for each axis, to remember if the player is shifting
	 *  that axis */
	Coord3d shiftingLevel;
	
public:

	/** The main input method that reads from keyboard and mouse */
	bool poll(Player &p, Sound &s, Gamestate &gs, Level &l);
	
	/** Default constructor */
	Input();
	
	/** Default destructor */
	virtual ~Input();
};

#endif /*INPUT_H_*/
