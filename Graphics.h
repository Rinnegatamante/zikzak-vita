/*
 * Project: zikzak
 * Filename: Graphics.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Graphics holds the methods to load a texture, build and display
 *   text and is responsible for creating the window and rendering context.
 *   The font building and displaying code was taken from a Nehe tutorial.
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "Gfx.h"
#include "Level.h"
#include "Player.h"
#include "Sound.h"
#include "Gamestate.h"
#include "Coord3d.h"
using std::string;
using std::cerr;
using std::ostringstream;

class Graphics
{
private:
	/** Array to hold the textures. Currently, we have just one texture,
	 *  namely the font bitmap. */
	GLuint textures[1];
	
	/** The base value of the font display list */
	GLuint fontbase;

	/** Loads a texture file */
	int loadTexture(string filename);
	
	/** This method creates an OpenGL display list from the font texture */
	void buildFont();
	
	void drawFont(int val);
	
public:
	/** glPrint prints the string at a given position on the screen.
	 *  The coordinates and scale are relative to a 640x480 screen size.
	 *  That means you can call glPrint at e.g. 1024x768 and the text has
	 *  the same relative size and position. The lower left corner of the
	 *  screen has coordinate (0,0), the upper right corner has (640,480). */
	void glPrint(GLint x, GLint y, const char *string, float scale);
	
	/** Initialize a window and OpenGL drawing context
	 *  Returns 0 on success and the SDL error code on failure. */
	int init(int xres = 640, int yres = 480, bool fullscreen = false);
	
	/** The main rendering method: View transformation is done here,
	 *  and render methods of all blocks and the player are called */
	void renderscene(Level &l, Player &p, Sound &s, Gamestate &gs);
	
	/** Release the mouse cursor and call SDL_Quit() */
	void cleanUp();
	
	/** Default constructor */
	Graphics();
	
	/** Default destructor */
	virtual ~Graphics();
};

#endif /*GRAPHICS_H_*/
