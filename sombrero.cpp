#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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

class Pixel {
	public:
		int R;
		int G;
		int B;
};

class Image {
		int width;
		int height;
		string fileName;

	public:
		vector< vector<Pixel> > pixels;

		Image(string filename, int w, int h);
		void DrawBody(Body * body, int r, int g, int b);
		void DrawAllBodies(int bodyCount, Body * bodyArray [], int r, int g, int b);
		void Save();
};

Image::Image(string filename, int w, int h) {
	fileName = filename;
	width = w;
	height = h;

	// Initialize image (set all pixels to black (0,0,0))
	for (int y = 0; y < height; y++)
	{
		vector<Pixel> row(width);
		pixels.push_back(row);

		Pixel p;
		p.R = 0;
		p.G = 0;
		p.B = 0;

		for (int x = 0; x < width; x++)
		{
			pixels[y].push_back(p);
		}
	}
}

void Image::DrawBody(Body * body, int r, int g, int b) {
	pixels[body->GetY()][body->GetX()].R = r;
	pixels[body->GetY()][body->GetX()].G = g;
	pixels[body->GetY()][body->GetX()].B = b;
}

void Image::DrawAllBodies(int bodyCount, Body * bodyArray [], int r, int g, int b) {
	for (int i = 0; i < bodyCount; i++)
	{
		DrawBody(bodyArray[i], r, g, b);
	}
}

void Image::Save() {
	ofstream imageFile(fileName, ios::out);
	imageFile << "P3\n";
	imageFile << to_string(width) << " " << to_string(height) << "\n";
	imageFile << "255\n";

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			imageFile << pixels[y][x].R << " ";
			imageFile << pixels[y][x].G << " ";
			imageFile << pixels[y][x].B << "\n";
		}
	}

	imageFile.close();
}

int main() {
	int bodyCount = 3;
	int width = 100, height = 100;
	string positionUnits, massUnits;
	double scale;

	Body * bodyArray [bodyCount];

	LoadFromFile("save1.txt", bodyCount, width, height, positionUnits, scale, massUnits, bodyArray);

	Image img = Image("demo.ppm", width, height);
	img.DrawAllBodies(bodyCount, bodyArray, 255, 255, 255);
	// OUTPUT:
	// R G
	// B *
	// so, img[y][x]
	img.Save();

	/*
	for (int i = 0; i < bodyCount; i++)
	{
		bodyArray[i] = new Body(AU * i, i, 5.0e24, 1.0e3, 0.0);
		dataOutput.AddBody(bodyArray[i]);
	}
	*/

	// TODO :: Must remember to delete the bodyArray, as it is created using the * new * keyword.

	return 0;
}
