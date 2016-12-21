// include guard
#ifndef __BODY_H_INCLUDED__
#define __BODY_H_INCLUDED__

class Body {
		double x, y, z;
		double nextX, nextY, nextZ;
		double xVelocity, yVelocity, zVelocity;
		double mass;
		double radius;
		double xForce, yForce, zForce;

	public:
		Body(double xPos, double yPos, double zPos, double bodyMass, double r, double xVel, double yVel, double zVel);

		double GetMass() { return mass; };
		double GetX() { return x; };
		double GetY() { return y; };
		double GetZ() { return z; };
		double GetNextX() { return nextX; };
		double GetNextY() { return nextY; };
		double GetNextZ() { return nextZ; };
		double GetRadius() { return radius; };
		void SetPosition(double xPos, double yPos, double zPos) { x = xPos; y = yPos; z = zPos; };
		double GetXVelocity() { return xVelocity; };
		double GetYVelocity() { return yVelocity; };
		double GetZVelocity() { return zVelocity; };
		void SetVelocity(double xVel, double yVel, double zVel) { xVelocity = xVel; yVelocity = yVel; zVelocity = zVel; };

		void AddForce(double force, double phi, double theta);
		void ResetForce();

		void Update(double dt);
		void Step();
};

#endif
