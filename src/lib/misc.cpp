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

	string number_string = to_string(number);
	return number_string.length();
}

string PadWithZeroes(int frame_number, int max_frames) {
	// Pad a value with zeroes to match a final length
	// Used for image filenames, e.g. image_001.png, image_042.png, etc.

	// Get Lengths of numbers
	int max_frame_num_length = GetLengthOfNumber(max_frames - 1);
	int frame_num_length = GetLengthOfNumber(frame_number);

	string padded_value = to_string(frame_number);

	// Only add zeroes to the number if it is required
	if (frame_num_length < max_frame_num_length)
	{
		int difference = max_frame_num_length - frame_num_length;
		padded_value = "";
		for (int i = 0; i < difference; i++)
		{
			padded_value += "0";
		}
		padded_value += to_string(frame_number);
	}

	return padded_value;
}

string RemoveTrailingZeroes(string input_string) {
	// Remove zeroes from a number (converted to a string)
	// e.g. 1.4000000000 -> 1.4

	int i = input_string.length() - 1;
	int last_not_zero;

	while (i >= 0) {
		if (input_string[i] != '0') {
			last_not_zero = i;
			break;
		}

		i--;
	}

	if (input_string[last_not_zero] == '.') last_not_zero++;

	string outputString = input_string.substr(0, last_not_zero + 1);
	return outputString;
}

string ToStandardForm(double value) {
	// Convert a double to a standard form string
	// e.g. 12857 -> 1.2857e4
	string standard_form = "";
	bool negative = false;

	// Calculate the power
	int power = 0;
	double v = value;

	// Determine if value is negative
	if (value < 0.0) {
		negative = true;
		v *= -1;
	}

	// Decrease value until it is below 10.0 (and increase power counter)
	while (v >= 10.0) {
		v /= 10;
		power ++;
	}

	// Increase value until it is above 1.0 (and decrease power counter)
	while (v < 1.0 && v != 0.0) {
		v *= 10;
		power --;
	}

	string value_string = to_string(v);

	// Create standard form string
	if (power != 0) {
		// Deal with the negative sign at the front of negative numbers
		if (negative) {
			standard_form = "-";
		}

		standard_form += RemoveTrailingZeroes(value_string);

		// Append exponent
		standard_form += "e";
		standard_form += to_string(power);
	}
	else {
		// Doesn't need to be put into standard form if number is e0
		// e.g. 1.8793e0 = 1.8793, so standard form not required
		if (negative) {
			standard_form = "-";
		}

		standard_form += RemoveTrailingZeroes(value_string);
	}

	return standard_form;
}

double Random(double min, double max) {
	// Generate a random double, from min up to (and including) max
	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	double rnd = min + r * (max - min);

	return rnd;
}

bool FileExists(string file_name) {
	// Determine whether the file at fileName exists
	// Try to read file, return .good()
	ifstream file(file_name.c_str());
	return file.good();
}

double ToRadians(double angle) {
	// Convert an angle in degrees to radians
	return angle * (PI / 180.0);
}