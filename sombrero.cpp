#include <iostream>
#include <fstream>
#include <sstream>

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

// Output Class
class Output {
		string fileName;
		string fileContents;

	public:
		Output(string filename, int bodyCount, int width, int height, string positionUnits, double scale, string massUnits);

		void AddBody(Body * body);
		void AddAllBodies(int bodyCount, Body * bodyArray []);
		void Save();
};

Output::Output(string filename, int bodyCount, int width, int height, string positionUnits, double scale, string massUnits) {
	fileName = filename;
	fileContents = "";

	// Add details about the simulation
	// bodyCount, width, height, units (xy - m,au,lyr,pc,mpc), scale, units (mass - kg,m)
	fileContents += to_string(bodyCount) + ",";
	fileContents += to_string(width) + ",";
	fileContents += to_string(height) + ",";
	fileContents += positionUnits + ",";
	fileContents += to_string(scale) + ",";
	fileContents += massUnits + "\n";
}

void Output::AddBody(Body * body) {
	// x,y,mass,xV,yV
	fileContents += to_string(body->GetX()) + ",";
	fileContents += to_string(body->GetY()) + ",";
	fileContents += to_string(body->GetMass()) + ",";
	fileContents += to_string(body->GetXVelocity()) + ",";
	fileContents += to_string(body->GetYVelocity()) + "\n";
}

void Output::Save() {
	ofstream outputFile(fileName, ios::out);
	outputFile << fileContents;
	outputFile.close();
}

void Output::AddAllBodies(int bodyCount, Body * bodyArray []) {
	for (int i = 0; i < bodyCount; i++)
	{
		AddBody(bodyArray[i]);
	}
}

void LoadFromFile(string filename, int & bodyCount, int & width, int & height, string & positionUnits, double & scale, string & massUnits, Body * bodyArray []) {
	ifstream inputFile(filename);
	string fileLine;

	// Read in setup parameters
	getline(inputFile, fileLine);
	stringstream setup(fileLine);
	string parameter;
	string parameterArray [6];
	int i = 0;

	while (getline(setup, parameter, ','))
	{
		parameterArray[i] = parameter;
		i++;
	}

	bodyCount = stoi(parameterArray[0]);
	width = stoi(parameterArray[1]);
	height = stoi(parameterArray[2]);
	positionUnits = parameterArray[3];
	scale = stod(parameterArray[4]);
	massUnits = parameterArray[5];

	// Read in body details
	int bodyIndex = 0;

	while (getline(inputFile, fileLine))
	{
		stringstream bodyDetails(fileLine);
		string detailArray [5];
		i = 0;

		while (getline(bodyDetails, parameter, ','))
		{
			detailArray[i] = parameter;
			i++;
		}

		double x = stod(detailArray[0]);
		double y = stod(detailArray[1]);
		double mass = stod(detailArray[2]);
		double xVelocity = stod(detailArray[3]);
		double yVelocity = stod(detailArray[4]);

		bodyArray[bodyIndex] = new Body(x, y, mass, xVelocity, yVelocity);
		bodyIndex++;
	}
}

int main() {
	int bodyCount = 3;
	int width = 100, height = 100;
	string positionUnits, massUnits;
	double scale;

	Body * bodyArray [bodyCount];

	LoadFromFile("save.txt", bodyCount, width, height, positionUnits, scale, massUnits, bodyArray);
	cout << bodyCount << endl;
	cout << width << endl;
	cout << height << endl;
	cout << positionUnits << endl;
	cout << scale << endl;
	cout << massUnits << endl;

	Output dataOutput = Output("save1.txt", bodyCount, width, height, "m", 1.0, "kg");
	dataOutput.AddAllBodies(bodyCount, bodyArray);
	dataOutput.Save();

	/*
	for (int i = 0; i < bodyCount; i++)
	{
		bodyArray[i] = new Body(AU * i, i, 5.0e24, 1.0e3, 0.0);
		dataOutput.AddBody(bodyArray[i]);
	}
	*/

	return 0;
}
