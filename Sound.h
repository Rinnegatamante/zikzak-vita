/*
 * Project: zikzak
 * Filename: Sound.h
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Sound contains methods to load .wav files and an .ogg file and
 *   play a sound on demand
 */

#ifndef SOUND_H_
#define SOUND_H_
#include <iostream>
#include <vector>
#include <sstream>
#include "SDL.h"
#include "SDL_mixer.h"
using std::vector;
using std::string;
using std::cerr;
using std::ostringstream;

/** Enumeration for choosing which sound to play */
typedef enum Sounds {
	S_HIT = 0,
	S_SHOOT,
	S_SWITCH,
} Soundtype;

class Sound
{
private:
	/** Stores if the sound system has been initialized. If initializing
	 *  the sound system fails and playSound() is called, it just gets
	 *  ignored, thus making it possible to play the game even if no
	 *  sound hardware is available. */
	bool systemRunning;
	
	/** Stores the music */
	Mix_Music *music;
	
	/** Stores the different sounds. The order of the sounds loaded has to
	 *  accord to the above enumeration. */
	vector<Mix_Chunk*> sounds;
	
	/** Load a sound file and add it to the sounds vector */
	int loadSound(string filename);
	
	/** Load a music file */
	int loadMusic(string filename);
	
public:
	/** Try to initialize the sound system
	 *  Returns 0 on success and an SDL error code on failure */
	int init(bool music);
	
	/** Play a sound of the vector, use an enumeration value to specify
	 *  which. */
	void playSound(Soundtype s);
	
	/** Start playing the music. The music is automatically looped, so this
	 *  method has to be called only once */
	void playMusic();
	
	/** Shut down the sound system */
	void cleanUp();
	
	/** Default constructor */
	Sound();
	
	/** Default destructor */
	virtual ~Sound();
};

#endif /*SOUND_H_*/
