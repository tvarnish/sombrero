// include guard
#ifndef __BODY_H_INCLUDED__
#define __BODY_H_INCLUDED__

#include <iostream>

class Body {
		double x, y, z;
		double xVelocity, yVelocity, zVelocity;
		double mass;
		double radius;

		double nextX, nextY, nextZ;
		double xForce, yForce, zForce;

	public:
		Body * next = NULL;
		int id;

		Body(double _x, double _y, double _z, double _mass, double _radius, double _xVelocity, double _yVelocity, double _zVelocity);

		double GetMass() { return mass; };
		double GetX() { return x; };
		double GetY() { return y; };
		double GetZ() { return z; };
		double GetNextX() { return nextX; };
		double GetNextY() { return nextY; };
		double GetNextZ() { return nextZ; };
		double GetRadius() { return radius; };
		void SetPosition(double _x, double _y, double _z) { x = _x; y = _y; z = _z; };
		double GetXVelocity() { return xVelocity; };
		double GetYVelocity() { return yVelocity; };
		double GetZVelocity() { return zVelocity; };
		void SetVelocity(double _xVelocity, double _yVelocity, double _zVelocity) { xVelocity = _xVelocity; yVelocity = _yVelocity; zVelocity = _zVelocity; };

		void AddForce(double force, double phi, double theta);
		void ResetForce();

		void Update(double dt);
		void Step();
};

#endif
