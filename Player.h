/*
 * Project: zikzak
 * Filename: Player.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Player inherits from class Dynent. A Player has a std::list of Bullets
 *   and a shoot() method. When it shoots, a new Bullet is created, and
 *   the method doBullets() keeps track of their timeToLive.
 *   A player also has pitch and yaw attributes and corresponding get
 *   and set methods.
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include <list>
#include <iostream>
#include "Dynent.h"
#include "Level.h"
#include "Bullet.h"
#include "Sound.h"
#include "Coord3d.h"
using std::list;

class Player : public Dynent
{
private:
	/** The pitch when viewing the level */
	double pitch;
	
	/** The yaw when viewing the level */
	double yaw;
	
	/** The list of bullets */
	list<Bullet*> bullets;
	
	/** Render bullets and check for their remaining time to live,
	 *  remove them when it is due */
	void doBullets(Level &l, Sound &s);
	
public:
	/** Create a new bullet with the player's current direction
	 *  and position */
	void shoot(Sound &s);
	
	/** Pitch getter */
	double getPitch();
	
	/** Yaw getter */
	double getYaw();
	
	/** Pitch setter */
	void setPitch(double pitch);
	
	/** Yaw setter */
	void setYaw(double yaw);
	
	/** Renders the player at its position in the level */
	void render(Level &l, Sound &s);
	
	/** Player-specific collision-detection "in" a block (e.g. prisms) */
	bool inWallCollision(Level &l, Coord3d c, Sound &s);
	
	/** Player-specific collision-detection before a block (like a wall) */
	bool beforeWallCollision(Level &l, Coord3d c, Sound &s);
	
	/** Returns true if the player is moving on any axis */
	bool isMoving();
	
	/** Clear the bullet list */
	void clearBullets();
	
	/** Constructor: set coordinates and pitch and yaw to 0 by default */
	Player(double x = 0.0, double y = 0.0, double z = 0.0, 
		double pitch = 0.0, double yaw = 0.0);
		
	/** Default destructor */
	virtual ~Player();
};

#endif /*PLAYER_H_*/
