#include "output.h"
#include "body.h"
#include "units.h"

#include <fstream>

using namespace std;

Output::Output(string filename, int totalBodyCount, int width, int height, string positionUnits, double scale, string massUnits) {
	fileName = filename;
	fileContents = "";
	bodyCount = totalBodyCount;
	
	string massUnitString = massUnits;
	string positionUnitString = positionUnits;

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
	double x = body->GetX();
	double y = body->GetY();
	double mass = body->GetMass();
	
	/*
	// Handle Position Units
	if (positionUnitString == "AU")
	{
		x /= AU;
		y /= AU;
	}
	else if (positionUnitString == "LYR")
	{
		x /= LYR;
		y /= LYR;
	}
	else if (positionUnitString == "PC")
	{
		x /= PC;
		y /= PC;
	}
	else if (positionUnitString == "MPC")
	{
		x /= MPC;
		y /= MPC;
	}

	// Handle Mass Units
	if (massUnitString == "M")
	{
		mass /= M;
	}
	*/
	
	// x,y,mass,xV,yV
	fileContents += ToStandardForm( x ) + ",";
	fileContents += ToStandardForm( y ) + ",";
	fileContents += ToStandardForm( mass ) + ",";
	fileContents += ToStandardForm( body->GetXVelocity() ) + ",";
	fileContents += ToStandardForm( body->GetYVelocity() ) + "\n";
}

void Output::Save() {
	ofstream outputFile(fileName, ios::out);
	outputFile << fileContents;
	outputFile.close();
}

void Output::AddAllBodies(Body * bodyArray []) {
	for (int i = 0; i < bodyCount; i++)
	{
		AddBody(bodyArray[i]);
	}
}

string ToStandardForm(double value) {
	string standardForm = "";
	string valueString = to_string(value);
	bool negative = false;
	
	// Calculate the power
	int power = 0;
	double v = value;
	
	// Determine if value is negative
	if (value < 0.0) {
		negative = true;
		v *= -1;
	}
	
	if ((value < 1.0) && (value > 0.0)) {
		while (v < 1.0) {
			v *= 10;
			power --;
		}
	}
	else {
		while (v >= 10.0) {
			v /= 10;
			power ++;
		}
	}
	
	// Compile into standard form
	if (power != 0) {	
		// Deal with the negative sign at the front of negative numbers
		if (negative) {
			standardForm += "-";
			// Remove "-" from the value string
			valueString = valueString.substr(1, valueString.length() - 1);
		}
		
		// Append significant figures
		standardForm += valueString[0];
		standardForm += ".";
		for (size_t i = 1; i < valueString.length() - 1; i++) {
			if (valueString[i] == '.') break;
			standardForm += valueString[i];
		}
		
		// Append exponent
		standardForm += "e";
		standardForm += to_string(power);
	}
	else {
		standardForm = valueString;
	}
	
	return standardForm;
}
