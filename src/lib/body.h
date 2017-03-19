// include guard
#ifndef __BODY_H_INCLUDED__
#define __BODY_H_INCLUDED__

#include "position_vector.h"
#include <iostream>

class Body {
		//double x, y, z;
		Vector position;
		//double xVelocity, yVelocity, zVelocity;
		Vector velocity;
		double mass;
		double radius;

		//double nextX, nextY, nextZ;
		Vector nextPosition;
		//double xForce, yForce, zForce;
		Vector force;

	public:
		Body * next = NULL;
		int id;

		Body(double _x, double _y, double _z, double _mass, double _radius, double _xVelocity, double _yVelocity, double _zVelocity);
		Body(Vector position, double _mass, double _radius, Vector velocity);

		double GetMass() { return mass; };
		double GetRadius() { return radius; };

		/*
		double GetX() { return x; };
		double GetY() { return y; };
		double GetZ() { return z; };
		*/
		double GetX() { return position.GetX(); };
		double GetY() { return position.GetY(); };
		double GetZ() { return position.GetZ(); };
		Vector GetPosition() { return position; };

		/*
		double GetNextX() { return nextX; };
		double GetNextY() { return nextY; };
		double GetNextZ() { return nextZ; };
		*/
		double GetNextX() { return nextPosition.GetX(); };
		double GetNextY() { return nextPosition.GetY(); };
		double GetNextZ() { return nextPosition.GetZ(); };
		Vector GetNextPosition() { return nextPosition; };

		//void SetPosition(double _x, double _y, double _z) { x = _x; y = _y; z = _z; };
		void SetPosition(double _x, double _y, double _z) { position.Set(_x, _y, _z); };
		void SetPosition(Vector _position) { position = _position; };

		/*
		double GetXVelocity() { return xVelocity; };
		double GetYVelocity() { return yVelocity; };
		double GetZVelocity() { return zVelocity; };
		*/
		double GetXVelocity() { return velocity.GetX(); };
		double GetYVelocity() { return velocity.GetY(); };
		double GetZVelocity() { return velocity.GetZ(); };
		Vector GetVelocity() { return velocity; };

		//void SetVelocity(double _xVelocity, double _yVelocity, double _zVelocity) { xVelocity = _xVelocity; yVelocity = _yVelocity; zVelocity = _zVelocity; };
		void SetVelocity(double _xVelocity, double _yVelocity, double _zVelocity) { velocity.Set(_xVelocity, _yVelocity, _zVelocity); };
		void SetVelocity(Vector _velocity) { velocity = _velocity; };

		void AddForce(double _force, double phi, double theta);
		void ResetForce();

		void Update(double dt);
		void Step();
};

#endif
