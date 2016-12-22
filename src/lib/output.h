#ifndef __OUTPUT_H_INCLUDED__
#define __OUTPUT_H_INCLUDED__

#include <iostream>
#include "linkedlist.h"

class Body;

class Output {
		std::string fileName;
		std::string fileContents;

		//std::string ToStandardForm(double value);

	public:
		Output(std::string filename, int width, int height, double scale);

		void AddBody(Body * body);
		void AddAllBodies(List bodyList);
		void Save();
};

std::string ToStandardForm(double value);

#endif
