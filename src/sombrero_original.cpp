#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>

#include "lib/font.h"
#include "lib/body.h"
#include "lib/image.h"
#include "lib/video.h"
#include "lib/output.h"
#include "lib/misc.h"

#include "lib/units.h"

using namespace std;

int main(int argc, char * argv[]) {
	int bodyCount = 2;
	int width = 640;
	int height = 480;
	double scale = 150;

	double dt = DAY;
	double elapsedTime = 0;
	int totalFrames = 200;

	string positionUnits = "";
	string massUnits = "";

	Body * bodyArray [bodyCount];
	bodyArray[0] = new Body(0.0, 0.0, 0.0, 1.9891e30, 0.0, 0.0, 0.0);
	bodyArray[1] = new Body(0.0, -1.49597871e11, 0.0, 5.972e24, 0.0, 0.0, 30e3);

	Video video = Video("images/", "image_", width, height);
	video.ClearImageFolder();

	cout << "Running N-Body Simulation..." << endl;
	cout << "Total Frames: " << totalFrames << endl << endl;

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
					double zDistance = bodyArray[a]->GetZ() - bodyArray[b]->GetZ();
					double totalDistance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));

					// Calculate angles
					double phiAngle = atan2(zDistance, sqrt(pow(xDistance, 2) + pow(yDistance, 2)));
					double thetaAngle = atan2(yDistance, xDistance);

					// Calculate force
					double force = GR * ((bodyArray[a]->GetMass() * bodyArray[b]->GetMass()) / (pow(totalDistance, 2)));

					// Add force to total
					bodyArray[a]->AddForce(force, phiAngle, thetaAngle);
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

		/*
		// Save the velocity data
		double velX = bodyArray[7]->GetXVelocity();
		double velY = bodyArray[7]->GetYVelocity();
		double vel = sqrt((velX * velX) +(velY * velY));
		velocityString += to_string(frameNumber) + "," + to_string(vel) + "\n";
		*/

		// Generate Image
		string imageFileName = "images/image_" + PadWithZeroes(frameNumber, totalFrames) + ".ppm";
		Image image = Image(imageFileName, width, height);
		image.DrawAllBodies(bodyCount, bodyArray, 255, 255, 255, positionUnits, scale, 0);
		image.DrawText("Frame: " + to_string(frameNumber + 1), 10, 10, 0, 255, 0);
		image.DrawText("Time: " + to_string((int)elapsedTime), 10, 16, 0, 255, 0);
		image.DrawText("FRONT", 10, 22, 255, 0, 0);
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

	/*
	// Save velocityString
	ofstream o("init/velocity.csv", ios::out);
	o << velocityString;
	o.close();
	*/

	// Rotate the view
	for (int i = 0; i < 100; i++) {
		string imageFileName = "images/image_" + PadWithZeroes(totalFrames + i, totalFrames * 2 + 100) + ".ppm";
		Image image = Image(imageFileName, width, height);
		image.DrawAllBodies(bodyCount, bodyArray, 255, 255, 255, positionUnits, scale, 90.0);
		image.DrawText("Frame: " + to_string(totalFrames + i + 1), 10, 10, 0, 255, 0);
		image.DrawText("Time: " + to_string((int)elapsedTime), 10, 16, 0, 255, 0);
		image.DrawText("LEFT", 10, 22, 255, 0, 0);
		image.Save();
		image.CleanUp();
	}

	for (int frameNumber = totalFrames + 100; frameNumber < totalFrames * 2 + 100; frameNumber++)
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
					double zDistance = bodyArray[a]->GetZ() - bodyArray[b]->GetZ();
					double totalDistance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));

					// Calculate angles
					double phiAngle = atan2(zDistance, sqrt(pow(xDistance, 2) + pow(yDistance, 2)));
					double thetaAngle = atan2(yDistance, xDistance);

					// Calculate force
					double force = GR * ((bodyArray[a]->GetMass() * bodyArray[b]->GetMass()) / (pow(totalDistance, 2)));

					// Add force to total
					bodyArray[a]->AddForce(force, phiAngle, thetaAngle);
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

		/*
		// Save the velocity data
		double velX = bodyArray[7]->GetXVelocity();
		double velY = bodyArray[7]->GetYVelocity();
		double vel = sqrt((velX * velX) +(velY * velY));
		velocityString += to_string(frameNumber) + "," + to_string(vel) + "\n";
		*/

		// Generate Image
		string imageFileName = "images/image_" + PadWithZeroes(frameNumber, totalFrames * 2 + 100) + ".ppm";
		Image image = Image(imageFileName, width, height);
		image.DrawAllBodies(bodyCount, bodyArray, 255, 255, 255, positionUnits, scale, 90.0);
		image.DrawText("Frame: " + to_string(frameNumber + 1), 10, 10, 0, 255, 0);
		image.DrawText("Time: " + to_string((int)elapsedTime), 10, 16, 0, 255, 0);
		image.DrawText("LEFT", 10, 22, 255, 0, 0);
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

	// Create output.txt
	Output output("init/output.txt", bodyCount, width, height, positionUnits, scale, massUnits);
	output.AddAllBodies(bodyArray);
	output.Save();

	cout << "\n\nBuilding Video..." << endl;
	video.Build("result.mp4", totalFrames * 2 + 100);

	CleanUpBodyArray(bodyArray, bodyCount);

	cout << "Simulation Complete!" << endl;

	return 0;
}
