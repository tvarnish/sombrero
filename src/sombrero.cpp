#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "lib/font.h"
#include "lib/body.h"
#include "lib/image.h"
#include "lib/video.h"
#include "lib/output.h"
#include "lib/misc.h"

#include "lib/units.h"

using namespace std;

int main(int argc, char * argv[]) {
	int bodyCount;
	int width, height;
	string positionUnits, massUnits;
	double scale;

	string initFileName; // = "init/solarsystem.txt";

	double dt = DAY;
	double elapsedTime = 0;
	int totalFrames; // = 365;
	
	// Parsing Command Line Arguments
	if (argc != 3)
	{
		cout << "No arguments supplied! Please supply an initialisation file name, then a frame count.\n Usage: [time] ./sombrero initFileName frameCount" << endl;
		return 1;
	}
	else
	{
		initFileName = argv[1];
		totalFrames = atoi(argv[2]);
	}

	LoadParametersFromFile(initFileName, bodyCount, width, height, positionUnits, scale, massUnits);
	Body * bodyArray [bodyCount];
	LoadBodiesFromFile(initFileName, bodyArray, positionUnits, massUnits);

	// Debug

	Video video = Video("images/", "image_", width, height);
	video.ClearImageFolder();

	cout << "Running N-Body Simulation...\n Using init file: " << initFileName << endl;
	cout << " Total Frames: " << totalFrames << endl << endl;
	
	// Calculating values for progress bar
	string progressBar = "\rProgress -> [";
	int progressCounter = 0;
	int nextProgress = 0;
	int fivePercent = totalFrames / 20;

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
		
		// Update Progress Bar
		if (frameNumber == nextProgress)
		{
			if (frameNumber != 0)
			{
				progressCounter++;
			}
			
			cout << progressBar;
			for (int c = 0; c < 20; c++)
			{
				if (c < progressCounter)
				{
					cout << "=";	
				}
				else
				{
					cout << " ";
				}
			}
			cout << "] -> (" << nextProgress << ")";
			
			nextProgress += fivePercent;
			fflush(stdout);
		}
	}

	cout << "\n\nBuilding Video..." << endl;
	video.Build("result.mp4", totalFrames);

	CleanUpBodyArray(bodyArray, bodyCount);

	cout << "Simulation Complete!" << endl;

	return 0;
}
