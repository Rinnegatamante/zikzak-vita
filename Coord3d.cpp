#include "Coord3d.h"

void Coord3d::set(int a, int b, int c) {
	x = a;
	y = b;
	z = c;
}

Coord3d::Coord3d(int a, int b, int c):x(a),y(b),z(c) {};
Coord3d::~Coord3d() {}
