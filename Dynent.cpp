#include "Dynent.h"

void Dynent::setMovement(Coord3d c) {
	this->xmove = c.x;
	this->ymove = c.y;
	this->zmove = c.z;
	setX((int)(getX()+0.5));
	setZ((int)(getZ()+0.5));
}

bool Dynent::move(Level &l, Sound &s) {
	double eps = 0.10;
	
	x += ((double)xmove)/(10.0*speed);
	y += ((double)ymove)/(10.0*speed);
	z += ((double)zmove)/(10.0*speed);
	
	int checkx = 0;
	int checky = 0;
	int checkz = 0;

	if(zmove == 1) {
		checkx = (int)x;
		checky = (int)y;
		checkz = (int)(z+zmove);
	} else if(zmove == -1) {
		checkx = (int)x;
		checky = (int)y;
		checkz = (int)(z+zmove+1);
	}
	
	if(xmove == 1) {
		checkx = (int)(x+xmove);
		checky = (int)y;
		checkz = (int)z;
	} else if(xmove == -1) {
		checkx = (int)(x+xmove+1);
		checky = (int)y;
		checkz = (int)z;
	}
	
	// check for blocks that are "here", like prisms
	Coord3d herec((int)(getX()+0.5), 0, (int)(getZ()+0.5));

	int here = l.getBlock(herec);
	if((fabs(herec.x - getX()) <= eps) && (fabs(herec.z - getZ()) <= eps))
	switch(here) {
		case 2:		// prism upper left
		case 12:	// red prism upper left
			if(xmove == -1) {
				setMovement(Coord3d(0,0,-1));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == 1) {
				setMovement(Coord3d(1,0,0));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			return true;
			break;
		case 3:		// prism upper right
		case 13:	// red prism upper right
			if(xmove == 1) {
				setMovement(Coord3d(0,0,-1));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
				return true;
			}
			if(zmove == 1) {
				setMovement(Coord3d(-1,0,0));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
				return true;
			}
			return true;
			break;
		case 4:		// prism lower left
		case 14:	// red prism lower left
			if(xmove == -1) {
				setMovement(Coord3d(0,0,1));
				if(!l.getTimeout(herec))				
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == -1) {
				setMovement(Coord3d(1,0,0));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			return true;
			break;
		case 5:		// prism lower right
		case 15:	// red prism lower right
			if(xmove == 1) {
				setMovement(Coord3d(0,0,1));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == -1) {
				setMovement(Coord3d(-1,0,0));
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			return true;
			break;
		case 17:	// teleporter
		case 18:
		case 19:
		case 20:
		case 21:
			if(!l.getTimeout(herec)) {
				s.playSound(S_SWITCH);
				teleport(herec, l);
			}
			l.setTimeout(herec);
			return true;
			break;
		default:
			return inWallCollision(l, herec, s);
			break;
	}

	herec.set(checkx+1, checky, checkz);
	
	// check for blocks that are one block away, like walls	
	switch(l.getBlock(herec)) {
		case 1:		// wall
		case 16:	// invsible wall
			xmove = -xmove;
			ymove = -ymove;
			zmove = -zmove;
			if(!l.getTimeout(herec)) {
				s.playSound(S_HIT);
				l.setBlock(herec, 1);
			}
			l.setTimeout(herec);
			return true;
			break;
		case 24:	// violet wall
			xmove = -xmove;
			ymove = -ymove;
			zmove = -zmove;
			if(!l.getTimeout(herec)) {
				s.playSound(S_HIT);
			}
			l.setTimeout(herec);
			return true;
			break;
		case 2:		// prism upper left
		case 12:
			if(xmove == 1) {
				xmove = -xmove;
				if(!l.getTimeout(herec))				
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == -1) {
				zmove = -zmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			return true;
			break;
		case 3:		// prism upper right
		case 13:
			if(xmove == -1) {
				xmove = -xmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == -1) {
				zmove = -zmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			return true;
			break;
		case 4:		// prism lower left
		case 14:
			if(xmove == 1) {
				xmove = -xmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == 1) {
				zmove = -zmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}				
			return true;
			break;
		case 5:		// prism lower right
		case 15:
			if(xmove == -1) {
				xmove = -xmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(zmove == 1) {
				zmove = -zmove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			if(ymove != 0) {
				ymove = -ymove;
				if(!l.getTimeout(herec))
					s.playSound(S_HIT);
				l.setTimeout(herec);
			}
			return true;
			break;
		default:
			return beforeWallCollision(l, herec, s);
			break;
	}
	return true;
}

void Dynent::teleport(Coord3d from, Level &l) {
	// find corresponding teleporter
	Coord3d dest(from);
	int whichTeleporter = l.getBlock(from);
	for(int ay = 0; ay < l.getYSize(); ay++) {
		for(int ax = 0; ax < l.getXSize(); ax++) {
			for(int az = 0; az < l.getZSize(); az++) {
				if(l.getBlock(Coord3d(ax,ay,az)) == whichTeleporter &&
					((ax != from.x) || (az != from.z))) {
					dest.set(ax, ay, az);
				}
			}
		}
	}
	l.setTimeout(dest);
	this->x = (double)dest.x;
	this->z = (double)dest.z;
}

void Dynent::setX(double x) {
	this->x = x;
}

void Dynent::setY(double y) {
	this->y = y;
}

void Dynent::setZ(double z) {
	this->z = z;
}

double Dynent::getX() {
	return x;
}

double Dynent::getY() {
	return y;
}

double Dynent::getZ() {
	return z;
}

Dynent::Dynent(double x, double y, double z, double speed) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->speed = speed;
	xmove = ymove = zmove = 0;
}

Dynent::~Dynent() { }
