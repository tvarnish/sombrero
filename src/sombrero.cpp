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
	// position example:
	// (a,b) -> m.GetAt(a,b)
	// (0,0) (0,1)
	// (1,0) (1,1)

	/*
	PositionMatrix p;
	p.Initialise();
	p.Set(0.0, 4.0, -2.0);
	cout << p.GetX() << endl;
	cout << p.GetY() << endl;
	cout << p.GetZ() << endl << endl;

	PositionMatrix t;
	t.Initialise();

	for (double i = 0; i < 90; i += 10) {
		cout << "i = " << i << endl;
		t = p.RotateY(i);
		t = t.Round();
		t.Display();
		cout << endl;
	}
	*/
	
	string usageStatement = "Usage: ./sombrero [-g --generate] [-r --run]";
	
	int bodyCount = 401;
	int width = 640;
	int height = 480;

	Body * bodyArray [bodyCount];
	
	if (argc == 1) {
		// No arguments supplied. Exit.
		cout << "No arguments supplied. Must choose an option." << endl;
		cout << usageStatement << endl;
		return 1;
	}
	
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

			bodyArray[i] = new Body(x, y, z, mass, 0.0, 0.0, 0.0);
		}

		bodyArray[400] = new Body(0.0, 0.0, 0.0, 2e31, 0.0, 0.0, 0.0);

		// Save bodies to output.txt
		Output output("init/output.txt", bodyCount, width, height, "", 100, "");
		output.AddAllBodies(bodyArray);
		output.Save();

		Video video = Video("images/", "image_", width, height);
		video.ClearImageFolder();

		// Do the thing.
		// Rotate bodies about the y-axis
		for (double angle = 0.0; angle < 360.0; angle ++) {
			string imageFileName = "images/image_" + PadWithZeroes(angle, 360) + ".png";
		
			pngwriter png(width, height, 0.0, imageFileName.c_str());
			int midX = width / 2;
			int midY = height / 2;

			for (int i = 0; i < bodyCount; i++) {
				// Rotate body
				PositionMatrix p;
				p.Initialise();
				p.Set(bodyArray[i]->GetX(), bodyArray[i]->GetY(), bodyArray[i]->GetZ());

				PositionMatrix t;
				t.Initialise();
				t = p.RotateY(angle);
				t = t.Round();

				// Draw on image
				int x = (int)(t.GetX() / AU * 100) + midX;
				int y = (int)(t.GetY() / AU * 100) + midY;
				bool xValid = x < width && y >= 0;
				bool yValid = y < height && y >= 0;

				if (xValid && yValid) {
					png.plot(x, y, 1.0, 1.0, 1.0);
				}
			}

			png.close();
		}
	
		// string imageFileName = "images/image_" + PadWithZeroes(angle, 360) + ".ppm";
		// Image image = Image(imageFileName, width, height);
		// image.DrawAllBodies(bodyCount, rotatedBodies, 0, 255, 0, "", 100, 0);
		// image.Save();
		// image.CleanUp();
		
		// Build video from images
		video.Build("result.mp4", 360);
		
		CleanUpBodyArray(bodyArray, bodyCount);
	}

	/////////////////////////////////////////////////////////////////
	// N-Body Code

	if (strcmp(argv[1], "-r") == 0 or strcmp(argv[1], "--run") == 0) {
		LoadBodiesFromFile("init/output.txt", bodyArray, "", "");

		Video video = Video("images/", "image_", width, height);
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
			pngwriter png(width, height, 0.0, imageFileName.c_str());
			int midX = width / 2;
			int midY = height / 2;

			for (int i = 0; i < bodyCount; i++) {
				bodyArray[i]->Update(dt);
				
				PositionMatrix p;
				p.Initialise();
				p.Set(bodyArray[i]->GetX(), bodyArray[i]->GetY(), bodyArray[i]->GetZ());

				int x = (int)(p.GetX() / AU * 100) + midX;
				int y = (int)(p.GetY() / AU * 100) + midY;
				bool xValid = x < width && x >= 0;
				bool yValid = y < height && y >= 0;

				if (xValid && yValid) {
					png.plot(x, y, 1.0, 1.0, 1.0);
				}
			}

			png.close();
		}

		cout << "Done! Building video..." << endl;
		video.Build("result_run.mp4", frames);

		// Create output.txt
		Output output("init/output.txt", bodyCount, width, height, "", 100, "");
		output.AddAllBodies(bodyArray);
		output.Save();

		CleanUpBodyArray(bodyArray, bodyCount);
	}

	else {
		cout << "No valid arguments provided." << endl;
		cout << usageStatement << endl;
		return 1;
	}

	// Demo Image
	/*
	pngwriter png(300, 300, 0, "test.png");
	png.plot(100, 100, 1.0, 0.0, 0.0);
	png.plot(200, 100, 0.0, 1.0, 0.0);
	png.plot(200, 200, 0.0, 0.0, 1.0);
	png.plot(100, 200, 1.0, 1.0, 1.0);
	png.close();
	*/

	return 0;
}
