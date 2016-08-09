#include <iostream>

// Real World Units (https://www.iau.org/public/themes/measuring/)
#define AU 149597870700 // Astronomical Unit - Average Earth-Sun Distance
#define LYR 9.4607305e15 // Lightyear
#define PC 3.0857e12 // Parsec
#define MPC 1.0e6 * PC // Megaparsec
// A Parsec is the distance to a star that subtends 1 arcsecond, at an arc
// length of 1 AU.
#define DAY 86400 // Day in seconds
#define YR 365.25 * DAY // Year in Seconds
#define M 1.9891e30 // Mass of the Sun

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
		bodyArray[i] = new Body(AU, i, 5.0e24, 1.0e3, 0.0);
	}

	cout << bodyArray[0]->GetX() << endl;
	cout << bodyArray[1]->GetY() << endl;

	return 0;
}
