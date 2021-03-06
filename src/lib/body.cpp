// body.cpp
// Body class - represents a single planet/star in the simulation

#include "position_vector.h"
#include "body.h"

#include <cmath>

Body::Body(double _x, double _y, double _z, double _mass, double _radius, 
		   double _x_velocity, double _y_velocity, double _z_velocity, string _name) {
	// Constructor (when cartesian components are supplied)
	position = Vector(_x, _y, _z);

	mass = _mass;
	radius = _radius;

	velocity = Vector(_x_velocity, _y_velocity, _z_velocity);

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
	next_position = position.Add(velocity.Multiply(dt));
}

void Body::Step() {
	// Move to the calculated position
	position = next_position;
}

void Body::ResetForce() {
	// Reset the "net force" on the body (after each "frame")
	force.Set(0.0, 0.0, 0.0);
}
