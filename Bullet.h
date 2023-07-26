/*
 * Project: zikzak
 * Filename: Bullet.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Bullet inherits from class Dynent. A bullet has a timeToLive which
 *   can be decremented and checked using the isAlive() method.
 *   The parent (probably the player) has to dispose it if isAlive() is false.
 */
#ifndef BULLET_H_
#define BULLET_H_
#include "Dynent.h"
#include "Level.h"
#include "Sound.h"
#include "Coord3d.h"

class Bullet : public Dynent
{
private:
	/** The number of Ticks after which the bullet should be removed */
	int timeToLive;
	
public:
	/** Updates the timeToLive and returns true if the ttl is > 0 */
	bool isAlive();	
	
	/** Renders the bullet on its position in the level */
	void render(Level &l, Sound &s);
	
	/** Bullet-specific collision-detection "in" a block (e.g. prisms) */
	bool inWallCollision(Level &l, Coord3d c, Sound &s);
	
	/** Bullet-specific collision-detection before a block (like a wall) */
	bool beforeWallCollision(Level &l, Coord3d c, Sound &s);
	
	/** Constructor: direction is the initial moving direction:
	 * 0 = up, 1 = left, 2 = down, 3 = right */
	Bullet(double x = 0.0, double y = 0.0, double z = 0.0, int direction = 0);
	
	/** Default destructor */
	virtual ~Bullet();
};

#endif /*BULLET_H_*/
