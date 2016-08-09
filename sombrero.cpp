#include <iostream>

using namespace std;

// Body Class
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
	int bodyCount = 2;
	Body * bodyArray [bodyCount];

	for (int i = 0; i < bodyCount; i++)
	{
		bodyArray[i] = new Body(0.0, i, 10.0, 0.0, 0.0);
	}

	cout << bodyArray[0]->GetX() << endl;
	cout << bodyArray[1]->GetY() << endl;

	return 0;
}
