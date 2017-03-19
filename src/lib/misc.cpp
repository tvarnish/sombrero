#include "misc.h"
#include "body.h"
#include "units.h"
#include "linkedlist.h"

#include <fstream>
#include <sstream>

using namespace std;

void LoadFromFile(string filename, string & simulationName, int & width, int & height, double & scale, int & frameRate, double & timeStep, List & bodyList) {
	ifstream inputFile(filename);
	string fileLine;

	string parameter;
	int i = 0;

	// Read in parameter details
	getline(inputFile, fileLine);
	stringstream simulationParameters(fileLine);
	string parameterArray [6];

	while (getline(simulationParameters, parameter, ',')) {
		parameterArray[i] = parameter;
		i++;
	}

	simulationName = parameterArray[0];
	width = stoi(parameterArray[1]);
	height = stoi(parameterArray[2]);
	scale = stod(parameterArray[3]);
	frameRate = stoi(parameterArray[4]);
	timeStep = stod(parameterArray[5]);

	// Read in body details
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
	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	int rnd = (int)(min + r * (max - min));

	return rnd;
}

double Random(double min, double max) {
	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	double rnd = min + r * (max - min);

	return rnd;
}

bool FileExists(string filename) {
	ifstream file(filename.c_str());
	return file.good();
}

string RemoveTrailingZeroes(string inputString) {
	int i = inputString.length() - 1;
	int lastNotZero;

	while (i >= 0) {
		if (inputString[i] != '0') {
			lastNotZero = i;
			break;
		}

		i--;
	}

	if (inputString[lastNotZero] == '.') lastNotZero++;

	string outputString = inputString.substr(0, lastNotZero + 1);
	return outputString;
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