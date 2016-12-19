#include "body.h"

#include <cmath>

Body::Body(double xPos, double yPos, double zPos, double bodyMass, double r, double xVel, double yVel, double zVel) {
	x = xPos;
	y = yPos;
	z = zPos;

	mass = bodyMass;
	radius = r;
	
	xVelocity = xVel;
	yVelocity = yVel;
	zVelocity = zVel;
	
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
