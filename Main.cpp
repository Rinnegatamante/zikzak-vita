/*
 * Project: zikzak
 * Filename: Main.cpp
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Parse the commandline, instantiate all systems (graphics, sound, input
 *   and so on) and start the main game loop.
 */
 
#include <iostream>
#include <cstring>
#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"
#include "Gamestate.h"
#include "Sound.h"
#include "Config.h"
#include "Coord3d.h"

#ifdef __vita__
#include <dirent.h>
#include <vitasdk.h>
#include <vitaGL.h>
int _newlib_heap_size_user = 128 * 1024 * 1024;

char patched_fname[256];
void patch_fname(char *fname) {
	if (!strncmp(fname, "(null)/", 7)) {
		sprintf(patched_fname, "ux0:data/zik-zak/%s", &fname[7]);
	} else {
		sprintf(patched_fname, "ux0:data/zik-zak/%s", fname);
	}
}

extern "C" int __real_remove(const char *fname);
extern "C" int __wrap_remove(const char *fname) {
	printf("remove %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_remove(fname);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_remove(patched_fname);
}

extern "C" FILE *__real_fopen(char *fname, char *mode);
extern "C" FILE *__wrap_fopen(char *fname, char *mode) {
	printf("fopen %s\n", fname);
	if (strlen(fname) == 0)
		return NULL;
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_fopen(fname, mode);
	} else {
		patch_fname(fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_fopen(patched_fname, mode);
}

extern "C" int __real_stat(const char *fname, struct stat *buf);
extern "C" int __wrap_stat(const char *fname, struct stat *buf) {
	printf("stat %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_stat(fname, buf);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_stat(patched_fname, buf);
}

extern "C" DIR *__real_opendir(const char *fname);
extern "C" DIR *__wrap_opendir(const char *fname) {
	printf("opendir %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_opendir(fname);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_opendir(patched_fname);
}
#endif
using std::cout;

void displayHelp() {
	cout	<< "Zik Zak - v0.1 - Andreas Textor\n"
			<< "Usage: zikzak [-h|--help][-f][-x number][-y number][-nm]\n\n"
			<< "Options:\n"
			<< "\t-h  --help  - Display this information\n"
			<< "\t-f          - Run the game in fullscreen (default: off)\n"
			<< "\t-x number   - Set the horizontal resolution (default: 640)\n"
			<< "\t-y number   - Set the vertical resolution (default: 480)\n"
			<< "\t-nm         - Don't play music\n";
}

int main(int argc, char* argv[]) {
#ifdef __vita__
	vglInitExtended(32 * 1024 * 1024, 960, 544, 8 * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
	scePowerSetArmClockFrequency(444);
	scePowerSetBusClockFrequency(222);
	scePowerSetGpuClockFrequency(222);
	scePowerSetGpuXbarClockFrequency(166);
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
#endif
	
	bool fullscreen = true;
	bool music = true;
#ifdef __vita__
	int xres = 960;
	int yres = 544;
#else
	int xres = 800;
	int yres = 600;
#endif
	int lastlevel = 1;

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

	// load config first, to let commandline arguments override
	// the config settings
	Config c;
	c.load(music, fullscreen, xres, yres, lastlevel);
	
	// check commandline
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-f") == 0)
			fullscreen = true;
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			displayHelp();
			return 0;
		}
		if(strcmp(argv[i], "-x") == 0)
			if(argc-1 > i)
				xres = atoi(argv[i+1]);
			else {
				displayHelp();
				return 0;
			}
		if(strcmp(argv[i], "-y") == 0)
			if(argc-1 > i)
				yres = atoi(argv[i+1]);
			else {
				displayHelp();
				return 0;
			}
		if(strcmp(argv[i], "-nm") == 0)
			music = false;
	}
	
	// run game
	bool running = true;

	Graphics g;
	Sound s;
	Input i;
	Level l;
	Gamestate gs;
		
	int status = g.init(xres, yres, fullscreen);
	if(status != 0)
		return status;
		
	Player player1(1.0, -1.0, 1.0, 0.0, 60.0);
	
	if(l.load(0) != 0) {
		return 1;
	}
	
	status = s.init(music);
	if(music)
		s.playMusic();
	
	gs.resetTimer();
	gs.setOperationState(TITLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
	
	while(running) {
		g.renderscene(l, player1, s, gs);
		if(i.poll(player1, s, gs, l)) {
			// advance one level
			if(!player1.move(l, s)) {
				c.save(music, fullscreen, xres, yres, lastlevel);
				if(l.load(lastlevel) == 0) {
					lastlevel++;
					player1.setX(1.0);
					player1.setY(-1.0);
					player1.setZ(1.0);
					player1.setMovement(Coord3d(0,0,0));
					player1.clearBullets();
					gs.setOperationState(PLAY);
				} else {
					// no more levels found - that means
					// we have won \o/
					player1.setX(1.0);
					player1.setY(-1.0);
					player1.setZ(1.0);
					player1.setMovement(Coord3d(0,0,0));
					player1.clearBullets();
					gs.setOperationState(WON);
				}
			}
		} else {
			running = false;
		}
		
		if(l.getLevelNumber() == 0 || !player1.isMoving()) {
			gs.resetTimer();
		} else {
			// game over
			if(gs.getTimeLeft(l) <= 0) {
				l.load(lastlevel-1);
				player1.setX(1.0);
				player1.setY(-1.0);
				player1.setZ(1.0);
				player1.setMovement(Coord3d(0,0,0));
				player1.clearBullets();
				gs.setOperationState(GAMEOVER);
			}
		}		
		gs.loopDelay();
	}

	//c.save(music, fullscreen, xres, yres, lastlevel-1);
	s.cleanUp();
	g.cleanUp();
	
	return 0;
}
