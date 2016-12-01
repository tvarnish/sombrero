#include "misc.h"
#include "body.h"
#include "units.h"

#include <fstream>
#include <sstream>

using namespace std;

void LoadParametersFromFile(string filename, int & bodyCount, int & width, int & height, double & scale) {
	ifstream inputFile(filename);
	string fileLine;

	// Read in setup parameters
	getline(inputFile, fileLine);
	stringstream setup(fileLine);
	string parameter;
	string parameterArray [4];
	int i = 0;

	while (getline(setup, parameter, ','))
	{
		parameterArray[i] = parameter;
		i++;
	}

	bodyCount = stoi(parameterArray[0]);
	width = stoi(parameterArray[1]);
	height = stoi(parameterArray[2]);
	scale = stod(parameterArray[3]);
}

void LoadBodiesFromFile(string filename, Body * bodyArray []) {
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
		string detailArray [8];
		i = 0;

		while (getline(bodyDetails, parameter, ','))
		{
			detailArray[i] = parameter;
			i++;
		}

		double x = stod(detailArray[0]);
		double y = stod(detailArray[1]);
		double z = stod(detailArray[2]);
		double mass = stod(detailArray[3]);
		double radius = stod(detailArray[4]);
		double xVelocity = stod(detailArray[5]);
		double yVelocity = stod(detailArray[6]);
		double zVelocity = stod(detailArray[7]);

		bodyArray[bodyIndex] = new Body(x, y, z, mass, radius, xVelocity, yVelocity, zVelocity);
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
