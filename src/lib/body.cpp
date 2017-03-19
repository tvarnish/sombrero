#include "position_vector.h"
#include "body.h"

#include <cmath>

Body::Body(double _x, double _y, double _z, double _mass, double _radius, double _xVelocity, double _yVelocity, double _zVelocity) {
	/*
	x = _x;
	y = _y;
	z = _z;
	*/
	position = Vector(_x, _y, _z);

	mass = _mass;
	radius = _radius;

	/*
	xVelocity = _xVelocity;
	yVelocity = _yVelocity;
	zVelocity = _zVelocity;
	*/
	velocity = Vector(_xVelocity, _yVelocity, _zVelocity);

	/*
	xForce = 0.0;
	yForce = 0.0;
	zForce = 0.0;
	*/
	force = Vector();
}

Body::Body(Vector _position, double _mass, double _radius, Vector _velocity) {
	position = _position;
	mass = _mass;
	radius = _radius;
	velocity = _velocity;
}

void Body::Update(double dt) {
	// Update velocities
	/*
	xVelocity += xForce / mass * dt;
	yVelocity += yForce / mass * dt;
	zVelocity += zForce / mass * dt;
	*/

	velocity = velocity.Add(force.Divide(mass).Multiply(dt));

	// Calculate next position
	/*
	nextX = x + xVelocity * dt;
	nextY = y + yVelocity * dt;
	nextZ = z + zVelocity * dt;
	*/

	nextPosition = position.Add(velocity.Multiply(dt));
}

void Body::Step() {
	/*
	x = nextX;
	y = nextY;
	z = nextZ;
	*/

	position = nextPosition;
}

void Body::AddForce(double _force, double phi, double theta) {
	/*
	xForce += -force * cos(phi) * cos(theta);
	yForce += -force * cos(phi) * sin(theta);
	zForce += -force * sin(phi);
	*/

	_force *= -1;

	force = force.Add(Vector(_force * cos(phi) * cos(theta), _force * cos(phi) * sin(theta), _force * sin(phi)));
}

void Body::ResetForce() {
	/*
	xForce = 0.0;
	yForce = 0.0;
	zForce = 0.0;
	*/

	force.Set(0.0, 0.0, 0.0);
}
