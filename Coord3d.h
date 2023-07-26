/*
 * Project: zikzak
 * Filename: Coord3d.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Coord3d is a class to keep 3 values together, e.g. a 3-dimensional
 * 	 coordinate (though it can be used for other things, for example
 *   storing a movement vector). As it is only used as a combination of
 *   values, the x, y and z values are public.
 */

#ifndef COORD3D_H_
#define COORD3D_H_

class Coord3d
{
public:
	/** x component of vector */
	int x;

	/** y component of vector */
	int y;
	
	/** z component of vector */
	int z;
	
	/** Constructor: Set all values to 0 by default */
	Coord3d(int a = 0, int b = 0, int c = 0);
	
	/** Just for convenience, to set all values at once */
	void set(int a, int b, int c);
	
	/** Default destructor */
	virtual ~Coord3d();
};

#endif /*COORD3D_H_*/
