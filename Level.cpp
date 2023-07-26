#include "Level.h"

void Level::resetShift() {
	shift.set(0, 0, 0);
}

int Level::getXShift() {
	return shift.x;
}

int Level::getYShift() {
	return shift.y;
}

int Level::getZShift() {
	return shift.z;
}

void Level::shiftX(int s) {
	shift.x += s;
}

void Level::shiftY(int s) {
	shift.y += s;
}

void Level::shiftZ(int s) {
	shift.z += s;
}

int Level::getNumMessages() {
	return messages.size();	
}

string Level::getMessage(int num) {
	return messages[num];
}

int Level::getBlock(Coord3d c) {
	return data[c.x][c.y][c.z];
}

void Level::setBlock(Coord3d c, int block) {
	data[c.x][c.y][c.z] = block;
}

int Level::getXSize() {
	return Level::xsize;
}

int Level::getYSize() {
	return Level::ysize;
}

int Level::getZSize() {
	return Level::zsize;
}

int Level::load(int number) {
	messages.clear();
	char a;
		
	ostringstream filename;
#ifndef WIN32
	filename << "data/level" << number << ".map";
#else
	filename << "data\\level" << number << ".map";
#endif
	
	ifstream mfile (filename.str().c_str());
	
	if(!mfile.is_open()) {
		cerr << "Unable to open level file: " << filename.str() << "\n";
		return 1;
	}
	
	// fixme: make this more error-proof (handle broken/invalid files)	
	for(int z = Level::zsize-1; z >= 0; z--) {
		for(int x = 0; x < Level::xsize; x++) {
			a = mfile.get();
			if(a >= 'a' && a <= 'z')
				data[x][0][z] = (int)(a - 'a' + 10);
			else
				data[x][0][z] = (int)(a - '0');
		}
		// handle CR-LF correctly
		mfile.get();
		if((a = mfile.get()) != 10)
			mfile.putback(a);
	}
	string line;
	getline(mfile, line);
	seconds = atoi(line.c_str());
	
	while(!mfile.eof()) {
		getline(mfile, line);
		if(line[0] != '#' && line.length() > 1)
			messages.push_back(line);
	}
	
	mfile.close();
	currentLevel = number;
	return 0;
}

int Level::getLevelNumber() {
	return currentLevel;
}

int Level::getSeconds() {
	return seconds;
}

Level::Level() {
	messages.clear();
	
	for(int y = 0; y < Level::ysize; y++) {
		for(int x = 0; x < Level::xsize; x++) {
			for(int z = 0; z < Level::zsize; z++) {
				if(x==0 || z==0 || x==Level::xsize-1 || z==Level::zsize-1)
					data[x][y][z] = 1;
				else
					data[x][y][z] = 0;
				timeout[x][y][z] = 0;
			}
		}
	}
	
	int a, b;
	srand((unsigned)time(NULL));
	for(int i = 0; i <= 20; i++) {
		a = (rand()%(Level::xsize-2))+1;
		b = (rand()%(Level::zsize-2))+1;
		data[a][0][b] = (rand()%4)+2;
	}
		
	//data[3][0][5] = 1;
	data[1][0][3] = 2;
	
	resetShift();
	
}

void Level::glDrawprism(float xtr, float ytr, float ztr, int rotation) {
	GLfloat sz = 0.49f;
		
	glBegin(GL_QUADS);
	if(rotation == 0 || rotation == 1) {
		// back face
		glNormal3f(0.0f, 0.0f,-1.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
	}
	if(rotation == 0 || rotation == 2) {
		// left face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
	}
	if(rotation == 1 || rotation == 3) {
		// right face
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
	}
	if(rotation == 2 || rotation == 3) {
		// front face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
	}
	if(rotation == 0 || rotation == 3) {
		//  / face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
	}
	if(rotation == 1 || rotation == 2) {
		//  \ face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
	}
	glEnd();
}

void Level::glDrawpyramid(float xtr, float ytr, float ztr) {
	GLfloat sz = 0.49f;
		
	glBegin(GL_TRIANGLES);
	// front face
	glVertex3f(-sz+xtr, -sz+ytr, sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr, sz+ztr);
	glVertex3f(    xtr,  sz+ytr,    ztr);
	// left face
	glVertex3f(-sz+xtr, -sz+ytr, sz+ztr);
	glVertex3f(-sz+xtr, -sz+ytr,-sz+ztr);
	glVertex3f(    xtr,  sz+ytr,    ztr);
	// back face
	glVertex3f(-sz+xtr, -sz+ytr,-sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr,-sz+ztr);
	glVertex3f(    xtr,  sz+ytr,    ztr);
	// right face
	glVertex3f( sz+xtr, -sz+ytr,-sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr, sz+ztr);
	glVertex3f(    xtr,  sz+ytr,    ztr);
	glEnd();
}

void Level::glDrawfloor(float xtr, float ytr, float ztr) {
	glBegin(GL_QUADS);
	// bottom face
	glNormal3f( 0.0f, -0.5f, 0.0f);
	glVertex3f(-0.5f+xtr, -0.5f+ytr-0.2, -0.5f+ztr);
	glVertex3f( 0.5f+xtr, -0.5f+ytr-0.2, -0.5f+ztr);
	glVertex3f( 0.5f+xtr, -0.5f+ytr-0.2,  0.5f+ztr);
	glVertex3f(-0.5f+xtr, -0.5f+ytr-0.2,  0.5f+ztr);
    glEnd();
}

void Level::glDrawcube(float xtr, float ytr, float ztr) {
	GLfloat sz = 0.49f;
	glBegin(GL_QUADS);
	
	// front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
	glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
	glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
	
	// back face
	glNormal3f(0.0f, 0.0f,-1.0f);
	glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
	glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
	glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
	
	// top face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
	glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
	glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
	glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
	
	// right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
	glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
	glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
	
	// left face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
	glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
	glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
	glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
	
	// bottom face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
	glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
	glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
	glEnd();	
}

bool Level::getTimeout(Coord3d c) {
	return timeout[c.x][c.y][c.z] > 0;
}

void Level::setTimeout(Coord3d c) {
	timeout[c.x][c.y][c.z] = 30;
}

void Level::renderBlock(Coord3d c) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(0.1f);
#ifndef __vita__
	glEnable(GL_LINE_SMOOTH);
#endif
	float xtr = (float)c.x;
	float ytr = Level::ysize-(float)c.y;
	float ztr = Level::zsize-(float)c.z;

	if(--timeout[c.x+1][c.y][c.z] < 0)
		timeout[c.x+1][c.y][c.z] = 0;
		
	if(timeout[c.x+1][c.y][c.z] > 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	switch(data[c.x+1][c.y][c.z]) {
	case 0:	 // floor
	case 16:	// invisible wall
		glColor3f(0.05f, 0.1f, 0.4f);
		glDrawfloor(xtr, ytr, ztr);
		break;
	case 1:	// wall
		glColor3f(0.3f, 0.5f, 1.0f);
		glDrawcube(xtr, ytr, ztr);
		break;
	case 2:	// prism upper left
		glColor3f(0.5f, 0.5f, 0.5f);
		glDrawprism(xtr, ytr, ztr, 0);
		break;
	case 3:	// prism upper right
		glColor3f(0.5f, 0.5f, 0.5f);
		glDrawprism(xtr, ytr, ztr, 1);
		break;
	case 4:	// prism lower left
		glColor3f(0.5f, 0.5f, 0.5f);
		glDrawprism(xtr, ytr, ztr, 2);
		break;
	case 5:	// prism lower right
		glColor3f(0.5f, 0.5f, 0.5f);
		glDrawprism(xtr, ytr, ztr, 3);
		break;
	case 6:	// yellow prism upper left
		glColor3f(1.0f, 1.0f, 0.0f);
		glDrawprism(xtr, ytr, ztr, 0);
		break;
	case 7:	// yellow prism upper right
		glColor3f(1.0f, 1.0f, 0.0f);
		glDrawprism(xtr, ytr, ztr, 1);
		break;
	case 8:	// yellow prism lower left
		glColor3f(1.0f, 1.0f, 0.0f);	
		glDrawprism(xtr, ytr, ztr, 2);
		break;
	case 9:	// yellow prism lower right
		glColor3f(1.0f, 1.0f, 0.0f);	
		glDrawprism(xtr, ytr, ztr, 3);
		break;
	case 10:	// finish
		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawcube(xtr, ytr, ztr);
		break;
	case 11:	// red switch floor
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1.0f, 0.1f, 0.1f);
		glDrawfloor(xtr, ytr, ztr);
		break;
	case 12:	// red prism upper left
		glColor3f(1.0f, 0.1f, 0.1f);
		glDrawprism(xtr, ytr, ztr, 0);
		break;
	case 13:	// red prism upper right
		glColor3f(1.0f, 0.1f, 0.1f);
		glDrawprism(xtr, ytr, ztr, 1);
		break;
	case 14:	// red prism lower left
		glColor3f(1.0f, 0.1f, 0.1f);
		glDrawprism(xtr, ytr, ztr, 2);
		break;
	case 15:	// red prism lower right
		glColor3f(1.0f, 0.1f, 0.1f);
		glDrawprism(xtr, ytr, ztr, 3);
		break;
	case 17:	// teleporter orange
		glColor3f(1.0f, 0.58f, 0.0f);
		glDrawpyramid(xtr, ytr, ztr);
		break;
	case 18:	// teleporter cyan
		glColor3f(0.37f, 0.97f, 0.84f);
		glDrawpyramid(xtr, ytr, ztr);
		break;
	case 19:	// teleporter magenta
		glColor3f(0.78f, 0.15f, 0.51f);
		glDrawpyramid(xtr, ytr, ztr);
		break;
	case 20:	// teleporter dark green
		glColor3f(0.37f, 0.55f, 0.33f);
		glDrawpyramid(xtr, ytr, ztr);
		break;
	case 21:	// teleporter light blue
		glColor3f(0.0f, 0.56f, 1.0f);
		glDrawpyramid(xtr, ytr, ztr);
		break;
	case 22:	// violet switch floor
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0.6f, 0.46f, 0.89f);
		glDrawfloor(xtr, ytr, ztr);
		break;
	case 23:	// violet floor
		glColor3f(0.6f, 0.46f, 0.89f);
		glDrawfloor(xtr, ytr, ztr);
		break;
	case 24:	// violet wall
		glColor3f(0.6f, 0.46f, 0.89f);
		glDrawcube(xtr, ytr, ztr);
		break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Level::switchDoors() {
	int here;
	for(int y = 0; y < Level::ysize; y++) {
		for(int x = 0; x < Level::xsize; x++) {
			for(int z = 0; z < Level::zsize; z++) {
				here = data[x][y][z];
				if(here == 23)
					here = 24;
				else if(here == 24)
					here = 23;
				data[x][y][z] = here;
			}
		}
	}
}

void Level::switchBlocks() {
	int here;
	for(int y = 0; y < Level::ysize; y++) {
		for(int x = 0; x < Level::xsize; x++) {
			for(int z = 0; z < Level::zsize; z++) {
				here = data[x][y][z];
				// rotate the prisms clockwise
				if(here >= 12 && here <= 15) {
					if(here == 12)
						here = 13;
					else if(here == 13)
						here = 15;
					else if(here == 14)
						here = 12;
					else if(here == 15)
						here = 14;
					data[x][y][z] = here;
				}
			}
		}
	}
}

Level::~Level() { }
