#include "Sound.h"

void Sound::cleanUp() {
	Mix_HaltMusic();
	Mix_CloseAudio();
}

int Sound::loadSound(string filename) {
	ostringstream temp;
#ifndef WIN32
	temp << "data/" << filename;
#else
	temp << "data\\" << filename;
#endif
	filename = temp.str();
	
	Mix_Chunk *snd = Mix_LoadWAV(filename.c_str());
	if(snd) {
		Mix_VolumeChunk(snd, MIX_MAX_VOLUME/2);
		sounds.push_back(snd);
		return 0;
	} else {
		cerr << "Unable to load sound file: " << filename << "\n";
		return 1;
	}
}

int Sound::loadMusic(string filename) {
	ostringstream temp;
#ifndef WIN32
	temp << "data/" << filename;
#else
	temp << "data\\" << filename;
#endif
	filename = temp.str();
	
	music = Mix_LoadMUS(filename.c_str());
	if(!music) {
		cerr << "Unable to load music file: " << filename << "\n";
		return 1;
	} else
		return 0;
}

int Sound::init(bool music) {
	// initialize SDL for audio output
	if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) {
		cerr << "Unable to initialize SDL: " << SDL_GetError() << "\n";
 		return 1;
	}
	
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; // 16-bit stereo
	int audio_channels = 2;
	int audio_buffers = 4096;
	
	// open audio device
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		cerr << "Unable to open audio device!\n";
		return 1;
	}
	
	systemRunning = true;
	
	// load sounds
	bool status = 0;
	status |= loadSound("hit.wav");
	status |= loadSound("shoot.wav");
	status |= loadSound("switch.wav");
	if(music)
		status |= loadMusic("music.ogg");
	
	return status;
}

void Sound::playMusic() {
	if(systemRunning)
		Mix_PlayMusic(music, -1);
}

void Sound::playSound(Soundtype s) {
	if(systemRunning)
		Mix_PlayChannel(-1, *(sounds.begin()+s), 0);
}

Sound::Sound():systemRunning(false) { }
Sound::~Sound() { }
