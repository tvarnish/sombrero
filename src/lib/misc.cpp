#include "misc.h"
#include "body.h"
#include "units.h"
#include "linkedlist.h"

#include <fstream>
#include <sstream>

using namespace std;

void LoadParametersFromFile(string filename, int & width, int & height, double & scale) {
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

	width = stoi(parameterArray[0]);
	height = stoi(parameterArray[1]);
	scale = stod(parameterArray[2]);
}

void LoadBodiesFromFile(string filename, List & bodyList) {
	ifstream inputFile(filename);
	string fileLine;
	getline(inputFile, fileLine); // Skip first line

	// Read in body details
	string parameter;
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

		bodyList.Append(new Body(x, y, z, mass, radius, xVelocity, yVelocity, zVelocity));
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

int RandomInteger(int min, int max) {
	srand((unsigned)clock());;

	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	int rnd = (int)(min + r * (max - min));

	return rnd;
}

double Random(double min, double max) {
	srand((unsigned)clock());
	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	double rnd = min + r * (max - min);

	return rnd;
}

bool FileExists(string filename) {
	ifstream file(filename.c_str());
	return file.good();
}
