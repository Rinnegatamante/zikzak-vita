#ifndef CONFIG_H_
#define CONFIG_H_
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
using std::ostringstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cerr;

class Config
{
private:
	/** filename stores the path of the configfile to read and write */
	string filename;
	
public:
	/** Load all configurable values from the configfile and return
	 * them using references */
	void load(bool &music, bool &fullscreen, int &xres, int &yres,
		int &lastlevel);
	
	/** Save all configurable values to the configfile. This overwrites
	 * the old one, if existant */
	void save(bool music, bool fullscreen, int xres, int yres, int lastlevel);
	
	/** Default constructor */
	Config();
	
	/** Default destructor */
	virtual ~Config();
};

#endif /*CONFIG_H_*/
