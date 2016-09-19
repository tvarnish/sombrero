#ifndef __OUTPUT_H_INCLUDED__
#define __OUTPUT_H_INCLUDED__

#include <iostream>

class Body;

class Output {
		std::string fileName;
		std::string fileContents;
		int bodyCount;

	public:
		Output(std::string filename, int bodyCount, int width, int height, std::string positionUnits, double scale, std::string massUnits);

		void AddBody(Body * body);
		void AddAllBodies(Body * bodyArray []);
		void Save();
};

#endif
