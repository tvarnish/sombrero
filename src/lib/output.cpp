#include "output.h"
#include "body.h"
#include "units.h"
#include "misc.h"
#include "linkedlist.h"

#include <fstream>

using namespace std;

Output::Output(string _fileName) {
	fileName = _fileName;
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
