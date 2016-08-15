#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "libs/font.h"
#include "libs/body.h"
#include "libs/image.h"
#include "libs/video.h"
#include "libs/output.h"
#include "libs/misc.h"

#include "libs/units.h"

using namespace std;

int main() {
	int bodyCount;
	int width, height;
	string positionUnits, massUnits;
	double scale;

	double dt = DAY;
	double elapsedTime = 0;
	int totalFrames = 365;

	LoadParametersFromFile("savefiles/save.txt", bodyCount, width, height, positionUnits, scale, massUnits);
	Body * bodyArray [bodyCount];
	LoadBodiesFromFile("savefiles/save.txt", bodyArray, positionUnits, massUnits);

	// Debug

	Video video = Video("images/", "image_", width, height);
	video.ClearImageFolder();

	cout << "Running simulation..." << endl;

	for (int frameNumber = 0; frameNumber < totalFrames; frameNumber++)
	{
		for (int a = 0; a < bodyCount; a++)
		{
			bodyArray[a]->ResetForce();

			for (int b = 0; b < bodyCount; b++)
			{
				if (a != b)
				{
					// N-Body Code
					// Calculate distance
					double xDistance = bodyArray[a]->GetX() - bodyArray[b]->GetX();
					double yDistance = bodyArray[a]->GetY() - bodyArray[b]->GetY();
					double totalDistance = sqrt(pow(xDistance, 2) + pow(yDistance, 2));

					// Calculate angle
					double angle = atan2(yDistance, xDistance);

					// Calculate force
					double force = GR * ((bodyArray[a]->GetMass() * bodyArray[b]->GetMass()) / (pow(totalDistance, 2)));

					// Add force to total
					bodyArray[a]->AddForce(force, angle);
				}
			}
		}

		// Update all the bodies
		for (int i = 0; i < bodyCount; i++)
		{
			bodyArray[i]->Update(dt);
		}

		// Update elapsedTime
		elapsedTime += dt;

		// Generate Image
		string imageFileName = "images/image_" + PadWithZeroes(frameNumber, totalFrames) + ".ppm";
		Image image = Image(imageFileName, width, height);
		image.DrawAllBodies(bodyCount, bodyArray, 255, 255, 255, positionUnits, scale);
		image.DrawText("Frame: " + to_string(frameNumber + 1), 10, 10, 0, 255, 0);
		image.DrawText("Time: " + to_string((int)elapsedTime), 10, 16, 0, 255, 0);
		image.Save();
		image.CleanUp();
	}

	cout << "Building Video..." << endl;
	video.Build("result.mp4", totalFrames);

	CleanUpBodyArray(bodyArray, bodyCount);

	cout << "Done!" << endl;

	return 0;
}
