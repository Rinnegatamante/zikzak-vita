/*
 * Project: zikzak
 * Filename: Level.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Everything that has to to with the level is done here:
 *   level loading, rendering of single blocks, switching red prisms
 *   and violet blocks.
 */

#ifndef LEVEL_H_
#define LEVEL_H_
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include "Gfx.h"
#include "Coord3d.h"
using std::ifstream;
using std::cerr;
using std::string;
using std::ostringstream;
using std::vector;

class Level
{
private:
	/** Level x size constant
	 *  (x is from left to right) */
	static const int xsize = 20;
	
	/** Level y size constant
	 *  (y is from bottom to top) */
	static const int ysize = 1;
	
	/** Level z size constant
	 *  (z is from front to back */	
	static const int zsize = 20;

	/** The actual level array.
	 *  This stores only the blocks, no dynents.
	 *  Each block is an integer value:
	 *   0 = floor/air
	 *   1 = wall
	 *   2 = upper left prism
	 *   3 = upper right prism
	 *   4 = lower left prism
	 *   5 = lower right prism
	 *   6 = rotatable upper left prism
	 *   7 = rotatable upper right prism
	 *   8 = rotatable lower left prism
	 *   9 = rotatable lower right prism
	 *  10 = finish
	 *  11 = red switch
	 *  12 = switchable upper left prism
	 *  13 = switchable upper right prism
	 *  14 = switchable lower left prism
	 *  15 = switchable lower right prism
	 *  16 = invisible wall
	 *  17 = teleporter orange
	 *  18 = teleporter cyan
	 *  19 = teleporter magenta
	 *  20 = teleporter dark green
	 *  21 = teleporter light blue
	 *  22 = violet switch floor
	 *  23 = violet floor
	 *  24 = violet wall
	 */
	int data[xsize][ysize][zsize];
	
	/** A timeout value for each block is stored, that determines
	 *  for how long a block is solid (and how long it is unusable, for
	 *  teleporters). */
	int timeout[xsize][ysize][zsize];
	
	/** Holds the number of current level rather than a filename.
	 *  Level 0 is the title screen level. */
	int currentLevel;
	
	/** How many seconds does the player have to solve the level */
	int seconds;
	
	/** Vector of messages (title and hints) of the level, that get
	 *  displayed before the level starts */
	vector<string> messages;
	
	/** Stores how the level is shifted. Each value of shift gets divided
	 *  by 7.0 and the result is added to the diplaying transformation */
	Coord3d shift;
	
	/** Method to draw a simple cube */
	void glDrawcube(float xtr, float ytr, float ztr);
	
	/** Method to draw a prism, rotation encodes how the prism is rotated:
	 *  0 - upper left, 1 = upper right, 2 = lower left, 3 = lower right */
	void glDrawprism(float xtr, float ytr, float ztr, int rotation);
	
	/** Method to draw a simple floor quad */
	void glDrawfloor(float xtr, float ytr, float ztr);
	
	/** Method to draw a simple four-sided pyramid */
	void glDrawpyramid(float xtr, float ytr, float ztr);
	
public:
	/** Level x size getter */
	static int getXSize();
	
	/** Level y size getter */	
	static int getYSize();
	
	/** Level z size getter */	
	static int getZSize();
	
	/** Load data/level${number}.map
	 *  Look at the loading routine and/or a level file to see how
	 *  the level is stored. */
	int load(int number);
	
	/** Current level number getter */
	int getLevelNumber();
	
	/** Returns how many seconds are intended for solving the current level */
	int getSeconds();
	
	/** Returns the value of a block at the given position c */
	int getBlock(Coord3d c);
	
	/** Return the number of lines in the message vector */
	int getNumMessages();
	
	/** Returns true if the block is in its timeout phase (that means,
	 *  it is solid/unuasable */
	bool getTimeout(Coord3d c);
	
	/** Initialize the timeout phase for the block at position c */
	void setTimeout(Coord3d c);
	
	/** Replace the value at position c in the level with block */
	void setBlock(Coord3d c, int block);
	
	/** renderBlock checks which kind of block is stored at position c and
	 *  displays it accordingly */
	void renderBlock(Coord3d c);
	
	/** Rotate any red prisms clockwise once */
	void switchBlocks();
	
	/** Toggle any violet blocks once */
	void switchDoors();
	
	/** Reset the view shift (center the shift on the player) */
	void resetShift();
	
	/** Shift the view on the x axis by s */
	void shiftX(int s);
	
	/** Shift the view on the y axis by s */
	void shiftY(int s);
	
	/** Shift the view on the z axis by s */
	void shiftZ(int s);
	
	/** x shift getter */
	int getXShift();
	
	/** y shift getter */
	int getYShift();
	
	/** z shift getter */
	int getZShift();
	
	/** Returns the message line in line num of the message vector */
	string getMessage(int num);
	
	/** Default constructor */
	Level();
	
	/** Default destructor */
	virtual ~Level();
};

#endif /*LEVEL_H_*/
