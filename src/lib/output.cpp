#include "output.h"
#include "body.h"
#include "units.h"
#include "linkedlist.h"

#include <fstream>

using namespace std;

Output::Output(string filename) {
	fileName = filename;
	fileContents = "";
}

void Output::AddBody(Body * body) {
	fileContents += ToStandardForm( body->GetX() ) + ",";
	fileContents += ToStandardForm( body->GetY() ) + ",";
	fileContents += ToStandardForm( body->GetZ() ) + ",";
	fileContents += ToStandardForm( body->GetMass() ) + ",";
	fileContents += ToStandardForm( body->GetRadius() ) + ",";
	fileContents += ToStandardForm( body->GetXVelocity() ) + ",";
	fileContents += ToStandardForm( body->GetYVelocity() ) + ",";
	fileContents += ToStandardForm( body->GetZVelocity() ) + "\n";
}

void Output::Save() {
	ofstream outputFile(fileName, ios::out);
	outputFile << fileContents;
	outputFile.close();
}

void Output::AddAllBodies(List bodyList) {
	Body * body = bodyList.GetHead();

	while (body != NULL) {
		AddBody(body);
		body = body->next;
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
