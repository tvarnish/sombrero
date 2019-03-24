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
		Vector nextPosition;
		Vector force;

		// Used for linked list
		Body * next = NULL;
		int id;

	public:
		// Constructors
		Body(double _x, double _y, double _z, double _mass, double _radius, double _xVelocity, double _yVelocity, double _zVelocity, string _name = "");
		Body(Vector position, double _mass, double _radius, Vector velocity, string _name = "");

		// Get Methods
		double GetMass() { return mass; };
		double GetRadius() { return radius; };

		double GetX() { return position.GetX(); };
		double GetY() { return position.GetY(); };
		double GetZ() { return position.GetZ(); };
		Vector GetPosition() { return position; };

		double GetNextX() { return nextPosition.GetX(); };
		double GetNextY() { return nextPosition.GetY(); };
		double GetNextZ() { return nextPosition.GetZ(); };
		Vector GetNextPosition() { return nextPosition; };

		double GetXVelocity() { return velocity.GetX(); };
		double GetYVelocity() { return velocity.GetY(); };
		double GetZVelocity() { return velocity.GetZ(); };
		Vector GetVelocity() { return velocity; };

		Body * GetNext() { return next; };
		int GetID() { return id; };

		string GetName() { return name; }

		// Set Methods
		void SetPosition(double _x, double _y, double _z) { position.Set(_x, _y, _z); };
		void SetPosition(Vector _position) { position = _position; };
		void SetNextPosition(Vector _nextPosition) { nextPosition = _nextPosition; };

		void SetVelocity(double _xVelocity, double _yVelocity, double _zVelocity) { velocity.Set(_xVelocity, _yVelocity, _zVelocity); };
		void SetVelocity(Vector _velocity) { velocity = _velocity; };
		void AddVelocity(Vector _velocity) { velocity = velocity.Add(_velocity); };

		void SetNext(Body * _next) { next = _next; };
		void SetID(int _id) { id = _id; };

		void SetName(string _name) { name = _name; }

		// Calculation Methods
		void AddForce(Vector _force) { force = force.Add(_force); };
		void ResetForce();

		void Update(double dt);
		void Step();
};

#endif
