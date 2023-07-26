#include "Player.h"

bool Player::beforeWallCollision(Level &l, Coord3d c, Sound &s) {
	int here = l.getBlock(c);
	switch(here) {
		case 6:		// prism upper left
			if(xmove == 1) {
				xmove = -xmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(zmove == -1) {
				zmove = -zmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			return true;
			break;
		case 7:		// prism upper right
			if(xmove == -1) {
				xmove = -xmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(zmove == -1) {
				zmove = -zmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			return true;
			break;
		case 8:		// prism lower left
			if(xmove == 1) {
				xmove = -xmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(zmove == 1) {
				zmove = -zmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			return true;
			break;
		case 9:		// prism lower right
			if(xmove == -1) {
				xmove = -xmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(zmove == 1) {
				zmove = -zmove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
			}
			return true;
			break;
	}
	return true;
}

bool Player::inWallCollision(Level &l, Coord3d c, Sound &s) {
	int here = l.getBlock(c);
	switch(here) {
		case 6:		// yellow prism upper left
			if(xmove == -1)
				setMovement(Coord3d(0,0,-1));
			if(zmove == 1)
				setMovement(Coord3d(1,0,0));
			if(!l.getTimeout(c))
				s.playSound(S_HIT);
			l.setTimeout(c);
			return true;
			break;
		case 7:		// yellow prism upper right
			if(xmove == 1) {
				setMovement(Coord3d(0,0,-1));
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
				return true;
			}
			if(zmove == 1) {
				setMovement(Coord3d(-1,0,0));
				if(!l.getTimeout(c))
					s.playSound(S_HIT);
				l.setTimeout(c);
				return true;
			}
			return true;
			break;
		case 8:		// yellow prism lower left
			if(xmove == -1)
				setMovement(Coord3d(0,0,1));
			if(zmove == -1)
				setMovement(Coord3d(1,0,0));
			if(!l.getTimeout(c))
				s.playSound(S_HIT);
			l.setTimeout(c);
			return true;
			break;
		case 9:		// yellow prism lower right
			if(xmove == 1)
				setMovement(Coord3d(0,0,1));
			if(zmove == -1)
				setMovement(Coord3d(-1,0,0));
			if(!l.getTimeout(c))
				s.playSound(S_HIT);
			l.setTimeout(c);
			return true;
			break;
		case 10:	// finish
			return false;
			break;
		case 11:	// red switch floor
			if(!l.getTimeout(c)) {
				s.playSound(S_SWITCH);
				l.switchBlocks();
			}
			l.setTimeout(c);
			return true;
			break;
		case 22:	// violet switch floor
			if(!l.getTimeout(c)) {
				s.playSound(S_SWITCH);
				l.switchDoors();
			}
			l.setTimeout(c);
			return true;
			break;
	}
	return true;
}

double Player::getPitch() {
	return pitch;
}

void Player::setPitch(double pitch) {
	if(pitch <= 80.0 && pitch >= 20.0)
		this->pitch = pitch;
}

double Player::getYaw() {
	return yaw;
}

void Player::setYaw(double yaw) {
	this->yaw = yaw;
	if(this->yaw > 360.0)
		this->yaw -= 360.0;
	if(this->yaw < 0.0)
		this->yaw += 360.0;
}

bool Player::isMoving() {
	return xmove!=0 || zmove!=0;
}

void Player::shoot(Sound &s) {
	int bdir = 0;
	if(zmove == 1)
		bdir = 0;
	if(xmove == 1)
		bdir = 1;
	if(zmove == -1)
		bdir = 2;
	if(xmove == -1)
		bdir = 3;
	s.playSound(S_SHOOT);
	bullets.push_back(new Bullet(x, y, z, bdir));
}

void Player::doBullets(Level &l, Sound &s) {
	// move and render all of the player's bullets
	list<Bullet*>::iterator it = bullets.begin();
	while(it != bullets.end()) {
		(*it)->render(l, s);
		// if bullet's move says false, it ran into something,
		// or it has died, remove it
		if(!(*it)->isAlive() ||  !((*it)->move(l, s))) {
			delete (*it);
			bullets.erase(it);	
			return;
		}
		it++;
	}
}

void Player::render(Level &l, Sound &s) {
	// take care of bullets
	doBullets(l, s);
	
	float xtr = (float)x;
	float ytr = Level::getYSize()-(float)y;
	float ztr = Level::getZSize()-(float)z;
	GLfloat sz = 0.4f;
	
	//glColor3f(1.0f, 0.5f, 0.3f);
	glColor3f(0.3f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
	
	if(xmove == 0 && zmove == -1) {		// down
		// back face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
		// \ face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f(xtr, -sz+ytr,  sz+ztr);
		glVertex3f(xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
		// / face
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( xtr,  sz+ytr,  sz+ztr);
		glVertex3f( xtr, -sz+ytr,  sz+ztr);
	} else if(xmove == -1 && zmove == 0) {		// left
		// right face
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
		// \ face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(-sz+xtr, -sz+ytr,  ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  ztr);
		// / face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glVertex3f(-sz+xtr, -sz+ytr, ztr);
		glVertex3f(-sz+xtr,  sz+ytr, ztr);
		glVertex3f( sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr, -sz+ztr);
	} else if(xmove == 0 && zmove == 1) {		// up
		// front face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
		// / face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(xtr, -sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(xtr,  sz+ytr, -sz+ztr);
		// \ face
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( xtr, -sz+ytr, -sz+ztr);
		glVertex3f( xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  sz+ztr);
	} else {		// right
		// left face
		glNormal3f(-1.0, 0.0f, 0.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
		// / face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(-sz+xtr, -sz+ytr,  sz+ztr);
		glVertex3f( sz+xtr, -sz+ytr,  ztr);
		glVertex3f( sz+xtr,  sz+ytr,  ztr);
		glVertex3f(-sz+xtr,  sz+ytr,  sz+ztr);
		// \ face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glVertex3f(-sz+xtr, -sz+ytr, -sz+ztr);
		glVertex3f(-sz+xtr,  sz+ytr, -sz+ztr);
		glVertex3f( sz+xtr,  sz+ytr, ztr);
		glVertex3f( sz+xtr, -sz+ytr, ztr);
	}
    glEnd();

}

void Player::clearBullets() {
	for(list<Bullet*>::iterator it = bullets.begin(); 
		bullets.size() > 0; it = bullets.begin()) {
		delete (*it);
		bullets.erase(it);
	}
}

Player::Player(double x, double y, double z, double yaw, double pitch):Dynent(x,y,z,2.0) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->pitch = pitch;
	this->yaw = yaw;
	bullets.clear();
}

Player::~Player() {
	clearBullets();
}
