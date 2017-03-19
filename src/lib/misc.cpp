// misc.cpp
// A collection of miscellaneous functions required by a number of other functions/objects

#include "misc.h"
#include "body.h"
#include "units.h"
#include "linkedlist.h"

#include <fstream>
#include <sstream>

using namespace std;

int GetLengthOfNumber(int number) {
	// Get the length of a number (for padding)
	// e.g. 100 has length 3, 37 has length 2

	string numberString = to_string(number);
	return numberString.length();
}

string PadWithZeroes(int frameNumber, int maxFrames) {
	// Pad a value with zeroes to match a final length
	// Used for image filenames, e.g. image_001.png, image_042.png, etc.

	// Get Lengths of numbers
	int maxFramesLength = GetLengthOfNumber(maxFrames - 1);
	int frameGetLengthOfNumber = GetLengthOfNumber(frameNumber);

	string paddedValue = to_string(frameNumber);

	// Only add zeroes to the number if it is required
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
	// Remove zeroes from a number (converted to a string)
	// e.g. 1.4000000000 -> 1.4

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
	// Convert a double to a standard form string
	// e.g. 12857 -> 1.2857e4
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

	// Create standard form string
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
		// Doesn't need to be put into standard form if number is e0
		// e.g. 1.8793e0 = 1.8793, so standard form not required
		standardForm = valueString;
	}

	return standardForm;
}

double Random(double min, double max) {
	// Generate a random double, from min up to (and including) max
	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	double rnd = min + r * (max - min);

	return rnd;
}

bool FileExists(string fileName) {
	// Determine whether the file at fileName exists
	// Try to read file, return .good()
	ifstream file(fileName.c_str());
	return file.good();
}

double ToRadians(double angle) {
	// Convert an angle in degrees to radians
	return angle * (PI / 180.0);
}