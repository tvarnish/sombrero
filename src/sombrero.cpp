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
#include "lib/matrix.h"
#include "lib/units.h"

#include <pngwriter.h>

using namespace std;

int RandomInteger(int min, int max) {
	srand((unsigned)clock());;

	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	int rnd = (int)(min + r * (max - min));

	return rnd;
}

double Random(double min, double max) {
	srand((unsigned)clock());
	max ++;
	double r = (double)rand() / (double)RAND_MAX;
	double rnd = min + r * (max - min);

	return rnd;
}

int main(int argc, char * argv[]) {
	string usageStatement = "Usage: ./sombrero [-g --generate] [-r --run]";

	// Need to make these "settable" by the user
	int bodyCount = 201;
	int width = 640;
	int height = 480;

	int framerate = 45;

	Body * bodyArray [bodyCount];

	// No arguments supplied
	if (argc == 1) {
		// No arguments supplied. Exit.
		cout << "No arguments supplied. Must choose an option." << endl;
		cout << usageStatement << endl;
		return 1;
	}

	// Generate Body arrangement
	if (strcmp(argv[1], "-g") == 0 or strcmp(argv[1], "--generate") == 0) {
		// Randomly generate bodies
		for (int i = 0; i < bodyCount - 1; i++) {
			double r = Random(1e11, 2e11);
			double theta = Random(0, 2 * PI);
			double phi = Random(0, 2 * PI);

			double x = r * cos(theta) * cos(phi);
			double y = r * sin(theta);
			double z = r * cos(theta) * sin(phi);

			double mass = Random(1e23, 1e25);

			bodyArray[i] = new Body(x, y, z, mass, Random(1e6, 9e6), Random(0, 1e4), Random(0, 1e4), Random(0, 1e4));
		}

		bodyArray[400] = new Body(0.0, 0.0, 0.0, 2e31, 1e8, 0.0, 0.0, 0.0);

		// Save bodies to output.txt
		Output output("init/output.txt", bodyCount, width, height, 100);
		output.AddAllBodies(bodyArray);
		output.Save();

		Video video = Video("images/", "image_", width, height, framerate);
		video.ClearImageFolder();

		// Rotate bodies about the y-axis
		for (double angle = 0.0; angle < 360.0; angle ++) {
			string imageFileName = "images/image_" + PadWithZeroes(angle, 360) + ".png";
			Image image = Image(imageFileName, width, height, 100);

			for (int i = 0; i < bodyCount; i++) {
				// Rotate body
				PositionMatrix p;
				p.Initialise();
				p.Set(bodyArray[i]->GetX(), bodyArray[i]->GetY(), bodyArray[i]->GetZ());

				PositionMatrix t;
				t.Initialise();
				t = p.RotateY(angle);
				t = t.Round();

				image.DrawBody(t.GetX(), t.GetY(), 255, 255, 255);
			}

			image.Save();
		}

		// Build video from images
		video.Build("result.mp4", 360);

		CleanUpBodyArray(bodyArray, bodyCount);

		return 0;
	}

	// Run simulation
	if (strcmp(argv[1], "-r") == 0 or strcmp(argv[1], "--run") == 0) {
		LoadBodiesFromFile("init/output.txt", bodyArray);

		Video video = Video("images/", "image_", width, height, framerate);
		video.ClearImageFolder();

		int frames = 500;
		double dt = DAY;

		for (int f = 0; f < frames; f++) {
			for (int a = 0; a < bodyCount; a++) {
				bodyArray[a]->ResetForce();

				for (int b = 0; b < bodyCount; b++) {
					if (a != b) {
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

			string imageFileName = "images/image_" + PadWithZeroes(f, frames) + ".png";
			Image image = Image(imageFileName, width, height, 100);

			for (int i = 0; i < bodyCount; i++) {
				bodyArray[i]->Update(dt);
				bodyArray[i]->Step();

				image.DrawBody(bodyArray[i]->GetX(), bodyArray[i]->GetY(), 255, 255, 255);
			}

			image.Save();
		}

		cout << "Done! Building video..." << endl;
		video.Build("result_run.mp4", frames);

		// Create output.txt
		Output output("init/output.txt", bodyCount, width, height, 100);
		output.AddAllBodies(bodyArray);
		output.Save();

		CleanUpBodyArray(bodyArray, bodyCount);

		return 0;
	}

	// No *valid* arguments supplied
	else {
		cout << "No valid arguments provided." << endl;
		cout << usageStatement << endl;
		return 1;
	}

	return 0;
}
