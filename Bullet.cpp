#include "Bullet.h"

Bullet::Bullet(double x, double y, double z, int direction):Dynent(x,y,z,0.5) {
	this->x = x;
	this->y = y;
	this->z = z;
	xmove = ymove = zmove = 0;
	timeToLive = 500;
	switch(direction) {
		case 0:
			zmove = 1;
			break;
		case 1:
			xmove = 1;
			break;
		case 2:
			zmove = -1;
			break;
		case 3:
			xmove = -1;
			break;
	}
}

bool Bullet::beforeWallCollision(Level &l, Coord3d c, Sound &s) {
	return inWallCollision(l, c, s);
}

bool Bullet::inWallCollision(Level &l, Coord3d c, Sound &s) {
	int here = l.getBlock(c);
	if(here >= 6 && here <= 9) {
		if(here == 6)
			here = 7;
		else if(here == 7)
			here = 9;
		else if(here == 8)
			here = 6;
		else if(here == 9)
			here = 8;
		l.setBlock(c, here);
		s.playSound(S_SWITCH);
		return false;
	}
	
	return true;
}

bool Bullet::isAlive() {
	timeToLive--;
	return timeToLive > 0;	
}

void Bullet::render(Level &l, Sound &s) {
	float xtr = (float)x;
	float ytr = Level::getYSize()-(float)y;
	float ztr = Level::getZSize()-(float)z;
	GLfloat sz = 0.4f;
	GLfloat ss = 0.2f;
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	
	if(xmove == 0) {	// down or up
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-ss+xtr,  ytr, -sz+ztr);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-ss+xtr,  ytr,  sz+ztr);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( ss+xtr,  ytr,  sz+ztr);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( ss+xtr,  ytr, -sz+ztr);
	} else {			// left or right
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-sz+xtr,  ytr, -ss+ztr);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-sz+xtr,  ytr,  ss+ztr);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( sz+xtr,  ytr,  ss+ztr);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( sz+xtr,  ytr, -ss+ztr);
	}
    glEnd();

}

Bullet::~Bullet() { }
