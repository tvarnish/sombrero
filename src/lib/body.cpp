#include "body.h"

#include <cmath>

Body::Body(double _x, double _y, double _z, double _mass, double _radius, double _xVelocity, double _yVelocity, double _zVelocity) {
	x = _x;
	y = _y;
	z = _z;

	mass = _mass;
	radius = _radius;

	xVelocity = _xVelocity;
	yVelocity = _yVelocity;
	zVelocity = _zVelocity;

	xForce = 0.0;
	yForce = 0.0;
	zForce = 0.0;
}

void Body::Update(double dt) {
	// Update velocities
	xVelocity += xForce / mass * dt;
	yVelocity += yForce / mass * dt;
	zVelocity += zForce / mass * dt;

	// Calculate next position
	nextX = x + xVelocity * dt;
	nextY = y + yVelocity * dt;
	nextZ = z + zVelocity * dt;
}

void Body::Step() {
	x = nextX;
	y = nextY;
	z = nextZ;
}

void Body::AddForce(double force, double phi, double theta) {
	xForce += -force * cos(phi) * cos(theta);
	yForce += -force * cos(phi) * sin(theta);
	zForce += -force * sin(phi);
}

void Body::ResetForce() {
	xForce = 0.0;
	yForce = 0.0;
	zForce = 0.0;
}
