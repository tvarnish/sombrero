#include "misc.h"
#include "body.h"
#include "units.h"
#include "linkedlist.h"

#include <fstream>
#include <sstream>

using namespace std;

int GetLengthOfNumber(int number) {
	string numberString = to_string(number);
	return numberString.length();
}

string PadWithZeroes(int frameNumber, int maxFrames) {
	// Get Lengths of numbers
	int maxFramesLength = GetLengthOfNumber(maxFrames - 1);
	int frameGetLengthOfNumber = GetLengthOfNumber(frameNumber);

	string paddedValue = to_string(frameNumber);

	if (frameGetLengthOfNumber < maxFramesLength)
	{
		int difference = maxFramesLength - frameGetLengthOfNumber;
		paddedValue = "";
		for (int i = 0; i < difference; i++)
		{
			paddedValue += "0";
		}
		paddedValue += to_string(frameNumber);
	}

	return paddedValue;
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