// body.cpp
// Body class - represents a single planet/star in the simulation

#include "position_vector.h"
#include "body.h"

#include <cmath>

Body::Body(double _x, double _y, double _z, double _mass, double _radius, double _xVelocity, double _yVelocity, double _zVelocity, string _name) {
	// Constructor (when cartesian components are supplied)
	position = Vector(_x, _y, _z);

	mass = _mass;
	radius = _radius;

	velocity = Vector(_xVelocity, _yVelocity, _zVelocity);

	force = Vector();

	name = _name;
}

Body::Body(Vector _position, double _mass, double _radius, Vector _velocity, string _name) {
	// Constructor (when Vectors are supplied)
	position = _position;

	mass = _mass;
	radius = _radius;
	
	velocity = _velocity;

	force = Vector();

	name = _name;
}

void Body::Update(double dt) {
	// Euler Method - Calculate the next position and velocity of the body
	// Update velocities (with acceleration)
	velocity = velocity.Add(force.Divide(mass).Multiply(dt));

	// Calculate next position
	nextPosition = position.Add(velocity.Multiply(dt));
}

void Body::Step() {
	// Move to the calculated position
	position = nextPosition;
}

void Body::AddForce(double _force, double phi, double theta) {
	// Add the supplied force to the net force acting on the body
	force = force.Add(Vector(_force * cos(phi) * cos(theta), _force * cos(phi) * sin(theta), _force * sin(phi)));
}

void Body::ResetForce() {
	// Reset the "net force" on the body (after each "frame")
	force.Set(0.0, 0.0, 0.0);
}
