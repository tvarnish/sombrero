#include <iostream>

using namespace std;

class Body {
	double x, y;
	double xVelocity, yVelocity;
	double mass;
public:
	Body(double xPos, double yPos, double bodyMass, double xVel, double yVel);

	double GetMass() { return mass; };
	double GetX() { return x; };
	double GetY() { return y; };
	void SetPosition(double xPos, double yPos) { x = xPos; y = yPos; };
	double GetXVelocity() { return xVelocity; };
	double GetYVelocity() { return yVelocity; };
	void SetVelocity(double xVel, double yVel) { xVelocity = xVel; yVelocity = yVel; };
};

Body::Body(double xPos, double yPos, double bodyMass, double xVel, double yVel) {
	x = xPos;
	y = yPos;
	mass = bodyMass;
	xVelocity = xVel;
	yVelocity = yVel;
}

int main() {
	Body body = Body(1.0, 2.0, 30.0, 0.0, 0.0);

	cout << "Mass: " << body.GetMass() << endl;
	cout << "x: " << body.GetX() << ", y: " << body.GetY() << endl;
	cout << "xVelocity: " << body.GetXVelocity() << ", yVelocity: " << body.GetYVelocity() << endl;

	cout << "Setting Position to (10.0, -3.0)" << endl;

	body.SetPosition(10.0, -3.0);

	cout << "x: " << body.GetX() << ", y: " << body.GetY() << endl;

	return 0;
}
