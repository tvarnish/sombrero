#include "misc.h"
#include "body.h"
#include "units.h"

#include <fstream>
#include <sstream>

using namespace std;

void LoadParametersFromFile(string filename, int & bodyCount, int & width, int & height, string & positionUnits, double & scale, string & massUnits) {
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
}

void LoadBodiesFromFile(string filename, Body * bodyArray [], string positionUnits, string massUnits) {
	ifstream inputFile(filename);
	string fileLine;
	getline(inputFile, fileLine); // Skip first line

	// Read in body details
	string parameter;
	int bodyIndex = 0;
	int i;

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

		/*
		// Handle Position Units
		if (positionUnits == "AU")
		{
			x *= AU;
			y *= AU;
		}
		else if (positionUnits == "LYR")
		{
			x *= LYR;
			y *= LYR;
		}
		else if (positionUnits == "PC")
		{
			x *= PC;
			y *= PC;
		}
		else if (positionUnits == "MPC")
		{
			x *= MPC;
			y *= MPC;
		}

		// Handle Mass Units
		if (massUnits == "M")
		{
			mass *= M;
		}
		*/

		bodyArray[bodyIndex] = new Body(x, y, 0.0, mass, xVelocity, yVelocity, 0.0);
		bodyIndex++;
	}
}

void CleanUpBodyArray(Body * bodyArray [], int bodyCount) {
	for (int i = 0; i < bodyCount; i++)
	{
		delete bodyArray[i];
	}
}

int NumberLength(int number) {
	string numberString = to_string(number);
	return numberString.length();
}

string PadWithZeroes(int frameNumber, int maxCount) {
	// Get Lengths of numbers
	int maxCountLength = NumberLength(maxCount - 1);
	int frameNumberLength = NumberLength(frameNumber);

	string paddedValue = to_string(frameNumber);

	if (frameNumberLength < maxCountLength)
	{
		int difference = maxCountLength - frameNumberLength;
		paddedValue = "";
		for (int i = 0; i < difference; i++)
		{
			paddedValue += "0";
		}
		paddedValue += to_string(frameNumber);
	}

	return paddedValue;
}
