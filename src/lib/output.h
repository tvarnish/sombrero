#ifndef __OUTPUT_H_INCLUDED__
#define __OUTPUT_H_INCLUDED__

#include <iostream>

#include "body.h"
#include "linkedlist.h"

using namespace std;

class Output {
		string fileName;
		string fileContents;

		//std::string ToStandardForm(double value);

	public:
		Output(string _fileName);

		void AddBody(Body * body);
		void AddAllBodies(List bodyList);
		void Save();
};

//std::string ToStandardForm(double value);

#endif
