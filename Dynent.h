/*
 * Project: zikzak
 * Filename: Dynent.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Everything that isn't a static object (stored in the level array, that is)
 *   is a dynent (dynamic entity). Player and Bullet inherit from Dynent and
 *   add some specific methods (like rendering), these methods are abstract
 *   in Dynent.
 */

#ifndef DYNENT_H_
#define DYNENT_H_
#include <cmath>
#include "Gfx.h"
#include "Level.h"
#include "Sound.h"
#include "Coord3d.h"

class Dynent
{
protected:
	/** x position in level */
	double x;

	/** y position in level */
	double y;
	
	/** z position in level */	
	double z;
	
	/** Moving speed is calculated as follows:
	 *  move * 10.0/speed (where move is either 0 (when the dynent isn't
	 *  moving on this axis) or 1 (when it is). This means the greater
	 *  speed is, the slower the dynent becomes. */
	double speed;
	
	/** Movement on x-axis (1 or -1) */
	int xmove;
	
	/** Movement on y-axis (1 or -1) */
	int ymove;
	
	/** Movement on z-axis (1 or -1) */
	int zmove;
	
public:
	/** Renders the dynent on its position in the level */
	virtual void render(Level &l, Sound &s) = 0;
	
	/** Collision-detection "in" a block (e.g. prisms) */
	virtual bool inWallCollision(Level &l, Coord3d c, Sound &s) = 0;
	
	/** Collision-detection before a block (like a wall) */
	virtual bool beforeWallCollision(Level &l, Coord3d c, Sound &s) = 0;
	
	/** Move the dynent in the level. The sound object is necessary to
	 *  play sounds on collision etc. 
	 *  The most of the game logic is done here as everying necessary is
	 *  done to move the dynent (for example, reversing the direction and
	 *  playing a sound when hitting a wall).
	 *  Also, the object-specific collision-detection methods are called
	 *  here.
	 * */
	virtual bool move(Level &l, Sound &s);
	
	/** Sets the moving direction; -1, 0 or 1 for each axis */
	virtual void setMovement(Coord3d c);
	
	/** x coordinate getter */
	virtual double getX();
	
	/** y coordinate getter */
	virtual double getY();
	
	/** z coordinate getter */
	virtual double getZ();
	
	/** x coordinate setter */
	virtual void setX(double x);
	
	/** y coordinate setter */
	virtual void setY(double y);
	
	/** z coordinate setter */
	virtual void setZ(double z);
	
	/** This method is called when a dynent moves into a teleporter.
	 *  Coord3d from is the place where the source teleporter is,
	 *  the appropriate destination teleporter is searched and the dynent
	 *  is moved there. */
	virtual void teleport(Coord3d from, Level &l);
	
	/** Constructor: Set initial coordinates and speed to 0. */
	Dynent(double x = 0.0, double y = 0.0, double z = 0.0, double speed = 0.0);
	
	/** Default destructor */
	virtual ~Dynent();
};

#endif /*DYNENT_H_*/
