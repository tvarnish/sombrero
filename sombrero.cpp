#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "libs/font.h"
#include "libs/body.h"
#include "libs/image.h"
#include "libs/video.h"
#include "libs/misc.h"

#include "libs/units.h"

using namespace std;

int main() {
	//int bodyCount;
	int width = 100, height = 100;
	string positionUnits, massUnits;
	//double scale;

	//LoadParametersFromFile("save1.txt", bodyCount, width, height, positionUnits, scale, massUnits);
	//Body * bodyArray [bodyCount];
	//LoadBodiesFromFile("save1.txt", bodyArray);

	Video video = Video("images/", "frame_", width, height);
	video.ClearImageFolder();

	for (int i = 0; i < 100; i++)
	{
		string filename = "images/frame_" + PadWithZeroes(i, 100) + ".ppm";

		Image img = Image(filename, width, height);
		img.DrawText("Hello World", 10, 10, 0, 255, 0);
		img.pixels[50][i].SetColour(255, 255, 255);
		img.Save();
	}

	video.Build("output.mp4", 100);

	//CleanUpBodyArray(bodyArray, bodyCount);

	return 0;
}
