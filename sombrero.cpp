class Body {
	double x, y;
	double xVelocity, yVelocity;
	double mass;
public:
	double GetMass();
	double * GetPosition();
	void SetPosition(double xPos, double yPos);
	double * GetVelocity();
	void SetVelocity(double xVel, double yVel);
};

double Body::GetMass() {
	// GetMass - returns mass of Body (double).
	return mass;
}

double * Body::GetPosition() {
	// GetPosition - returns array of xy position in the form [x,y].
	static double position [2] = {x, y};
	return position;
}

void Body::SetPosition(double xPos, double yPos) {
	// SetPosition - Takes doubles xPos and yPos as parameters.
	x = xPos;
	y = yPos;
}

double * Body::GetVelocity() {
	// GetVelocity - returns array of xVelocity and yVelocity.
	static double velocity [2] = {xVelocity, yVelocity};
	return velocity;
}

void Body::SetVelocity(double xVel, double yVel) {
	// SetVelocity - Takes doubles xVel, and yVel.
	xVelocity = xVel;
	yVelocity = yVel;
}
