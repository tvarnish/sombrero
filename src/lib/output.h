#ifndef __OUTPUT_H_INCLUDED__
#define __OUTPUT_H_INCLUDED__

#include <iostream>

class Body;

class Output {
		std::string fileName;
		std::string fileContents;

		int bodyCount;

		//std::string ToStandardForm(double value);

	public:
		Output(std::string filename, int bodyCount, int width, int height, double scale);

		void AddBody(Body * body);
		void AddAllBodies(Body * bodyArray []);
		void Save();
};

std::string ToStandardForm(double value);

#endif
