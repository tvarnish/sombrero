#include "body.h"

#include <cmath>

Body::Body(double xPos, double yPos, double bodyMass, double xVel, double yVel) {
	x = xPos;
	y = yPos;
	mass = bodyMass;
	xVelocity = xVel;
	yVelocity = yVel;
	xForce = 0.0;
	yForce = 0.0;
}

void Body::Update(double dt) {
	// Update velocities
	xVelocity += xForce / mass * dt;
	yVelocity += yForce / mass * dt;

	// Update position
	x += xVelocity * dt;
	y += yVelocity * dt;
}

void Body::AddForce(double force, double angle) {
	xForce += -force * cos(angle);
	yForce += -force * sin(angle);
}

void Body::ResetForce() {
	xForce = 0.0;
	yForce = 0.0;
}
