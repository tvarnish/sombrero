// body.h
// Body class - represents a single planet/star in the simulation

#ifndef __BODY_H_INCLUDED__
#define __BODY_H_INCLUDED__

#include "position_vector.h"
#include <iostream>

class Body {
		// Body attributes
		Vector position;
		Vector velocity;
		double mass;
		double radius;
		string name;

		// Used for calculations
		Vector next_position;
		Vector force;

		// Used for linked list
		Body * next = NULL;
		int id;

	public:
		// Constructors
		Body(double _x, double _y, double _z, double _mass, double _radius, double _x_velocity, 
			 double _y_velocity, double _z_velocity, string _name = "");
		Body(Vector position, double _mass, double _radius, Vector velocity,
			 string _name = "");

		// Get Methods
		double GetMass() { return mass; };
		double GetRadius() { return radius; };

		double GetX() { return position.GetX(); };
		double GetY() { return position.GetY(); };
		double GetZ() { return position.GetZ(); };
		Vector GetPosition() { return position; };

		double GetNextX() { return next_position.GetX(); };
		double GetNextY() { return next_position.GetY(); };
		double GetNextZ() { return next_position.GetZ(); };
		Vector GetNextPosition() { return next_position; };

		double GetXVelocity() { return velocity.GetX(); };
		double GetYVelocity() { return velocity.GetY(); };
		double GetZVelocity() { return velocity.GetZ(); };
		Vector GetVelocity() { return velocity; };

		Vector GetMomentum() { return velocity.Multiply(mass); }

		Body * GetNext() { return next; };
		int GetID() { return id; };

		string GetName() { return name; }

		// Set Methods
		void SetPosition(double _x, double _y, double _z) { position.Set(_x, _y, _z); };
		void SetPosition(Vector _position) { position = _position; };
		void SetNextPosition(Vector _next_position) { next_position = _next_position; };

		void SetVelocity(double _x_velocity, double _y_velocity, double _z_velocity) 
						{ velocity.Set(_x_velocity, _y_velocity, _z_velocity); };
		void SetVelocity(Vector _velocity) { velocity = _velocity; };
		void AddVelocity(Vector _velocity) { velocity += _velocity; };

		void SetNext(Body * _next) { next = _next; };
		void SetID(int _id) { id = _id; };

		void SetName(string _name) { name = _name; };

		// Calculation Methods
		void AddForce(Vector _force) { force += _force; };
		void ResetForce();

		void Update(double dt);
		void Step();
};

#endif
