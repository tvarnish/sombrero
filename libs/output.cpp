#include "output.h"
#include "body.h"

#include <fstream>

using namespace std;

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
