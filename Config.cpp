#include "Config.h"

Config::Config() {
	ostringstream temp;
#ifdef __vita__
	temp << "ux0:data/Zik-Zak/zikzak.cfg";
#else
#ifndef WIN32
	temp << getenv("HOME") << "/.zikzakrc";
#else
	// putting the config file in the home directory would probably work
	// on newer windows versions, but windows users are used to having
	// all files of a program in one directory.
	temp << "zikzak.cfg";
#endif
#endif
	filename = temp.str();
}

void Config::load(bool &music, bool &fullscreen, int &xres, int &yres, 
	int &lastlevel) {
	ifstream mfile(filename.c_str());

	// having no config file is no problem, just use defaults
	if(mfile.is_open()) {
		string line;
		string setting;
		int value;
		int space;
		while(!mfile.eof()) {
			getline(mfile, line);
			// ignore comment and blank lines
			if(!(line[0] == '#') && !line.empty()) {
				space = line.find(" ", 0);
				setting = line.substr(0,space);
				value = atoi(line.substr(space, line.length()-space).c_str());
				
				if(setting == "music")
					music = (value == 1);
				if(setting == "fullscreen")
					fullscreen = (value == 1);
				if(setting == "xres")
					xres = value;
				if(setting == "yres")
					yres = value;
				if(setting == "lastlevel")
					lastlevel = value;
			}
		}
	}
}

void Config::save(bool music, bool fullscreen, int xres, int yres, 
	int lastlevel) {
	ofstream mfile(filename.c_str());
	if(!mfile.is_open()) {
		cerr << "Unable to write config file: " << filename << "\n";
		return;
	}
	
	mfile << "# sound settings\n";
	mfile << "music " << (music==true?'1':'0') << "\n\n";
	mfile << "# graphics settings\n";
	mfile << "fullscreen " << (fullscreen==true?'1':'0') << "\n";
	mfile << "xres " << xres << "\n";
	mfile << "yres " << yres << "\n\n";
	mfile << "# level to start - it's up to you if you want to cheat...\n";
	mfile << "# just make sure that 1<=level<=number of levels\n";
	mfile << "lastlevel " << lastlevel << "\n";
	
	mfile.close();
}

Config::~Config() { }
