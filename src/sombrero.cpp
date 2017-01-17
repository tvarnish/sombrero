#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <regex>

#include "lib/font.h"
#include "lib/body.h"
#include "lib/image.h"
#include "lib/video.h"
#include "lib/output.h"
#include "lib/misc.h"
#include "lib/matrix.h"
#include "lib/linkedlist.h"
#include "lib/units.h"

#include <pngwriter.h>

using namespace std;

// Function to return the index of an argument in argv
int FindFlag(char * argv[], int argc, string argumentShort, string argumentLong = "") {
	int shortIndex = -1;
	int longIndex = -1;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], argumentShort.c_str()) == 0 and shortIndex == -1) {
				shortIndex = i;
			}
		if (argumentLong != "") {
			if (strcmp(argv[i], argumentLong.c_str()) == 0 and longIndex == -1) {
				longIndex = i;
			}
		}
	}

	if (shortIndex != -1 and longIndex == -1) {
		return shortIndex;
	}
	else if (shortIndex == -1 and longIndex != -1) {
		return longIndex;
	}
	else {
		return -1;
	}

}

int main(int argc, char * argv[]) {
	string usageStatement = "Usage: ./sombrero [-g --generate] [-r --run] [-s --settings]";

	// Need to make these "settable" by the user
	// Defaults
	int bodyCount = 200;
	int width = 640;
	int height = 480;

	// "run" settings
	double dt = DAY;
	int frames = 200;

	int framerate = 45;

	List bodyList = List();
	Body * body;
	Body * bodyA;
	Body * bodyB;

	// No arguments supplied
	if (argc == 1) {
		cout << "No arguments supplied." << endl;
		cout << usageStatement << endl;
		return 1;
	}
	else {
		// Get indexes of main flags
		// May be more efficient to generate a dictionary of all of the flags and their indexes...
		int generateFlag = FindFlag(argv, argc, "-g", "--generate");
		int runFlag = FindFlag(argv, argc, "-r", "--run");
		int settingsFlag = FindFlag(argv, argc, "-s", "--settings");

		string flagString = "";
		bool generate = generateFlag != -1;
		bool run = runFlag != -1;
		bool settings = settingsFlag != -1;

		flagString = to_string(generate) + to_string(run) + to_string(settings);
		
		// Valid if flag string matches one of the following patterns:
		//	10*
		//	01*
		// REGUlAR EXPRESSIONS!

		regex validFlags("(01|10)(0|1)");
		if (regex_match(flagString, validFlags)) {
			cout << "Valid!" << endl;
		}
		else {
			cout << "Not Valid!" << endl;
		}

		return 0;
	}

	///////////////////////

	// Generate Body arrangement
	if (strcmp(argv[1], "-g") == 0 or strcmp(argv[1], "--generate") == 0) {
		if (argc >= 3) {
			// Generate random arrangement of bodies (+ rotation video)
			// i.e. create new bodyList
			if (strcmp(argv[2], "random") == 0) {
				for (int i = 0; i < bodyCount - 1; i++) {
					double r = Random(1e11, 1e11);
					double theta = Random(0, 2 * PI);
					double phi = Random(0, 2 * PI);

					double x = r * cos(theta) * cos(phi);
					double y = r * sin(theta);
					double z = r * cos(theta) * sin(phi);

					double mass = Random(1e23, 1e24);

					bodyList.Append(new Body(x, y, z, mass, Random(1e6, 9e6), 0, 0, 0));
				}

				bodyList.Append(new Body(0.0, 0.0, 0.0, 2e30, 1e8, 0.0, 0.0, 0.0));

				// Save bodies to output.txt
				Output output("init/output.txt", width, height, 100);
				output.AddAllBodies(bodyList);
				output.Save();
			}

			// Generate rotate video around bodies
			// i.e. Load bodies from file
			else if (strcmp(argv[2], "rotate") == 0) {
				string filename = "init/output.txt";

				if (argc == 4) {
					// Assume that the final argument is the filename
					filename = argv[3];
				}

				if (FileExists(filename)) {
					LoadBodiesFromFile(filename.c_str(), bodyList);
				}
				else {
					cout << "./sombrero --generate rotate [filename]" << endl;
					cout << "When using rotate, must include valid filename: e.g. init/output.txt" << endl;
					return 1;
				}
			}

			// No *valid* options supplied
			else {
				cout << usageStatement << endl;
				cout << "Must supply generate argument: [random, rotate]" << endl;
				return 1;
			}
		}

		// No options supplied
		else {
			cout << usageStatement << endl;
			cout << "Must supply generate argument: [random, rotate]" << endl;
			return 1;
		}

		// Generate rotate video around bodies
		Video video = Video("images/", "image_", width, height, framerate);
		video.ClearImageFolder();

		// Rotate bodies about the y-axis
		for (double angle = 0.0; angle < 360.0; angle ++) {
			string imageFileName = "images/image_" + PadWithZeroes(angle, 360) + ".png";
			Image image = Image(imageFileName, width, height, 100);

			body = bodyList.GetHead();
			while (body != NULL) {
				// Rotate body
				Vector p;
				p.Set(body->GetX(), body->GetY(), body->GetZ());

				Vector t;
				t = p.RotateY(angle);
				t = t.RoundValues();

				image.DrawBody(t.GetX(), t.GetY(), 255, 255, 255);

				body = body->next;
			}

			// Add details to image
			image.DrawText("ROTATION", 10, 10, 255, 255, 255);
			image.DrawText("A: " + to_string((int)angle), 10, 20, 255, 255, 255);
			image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 255, 255, 255);

			image.Save();
		}

		// Build video from images
		video.Build("result.mp4", 360);

		return 0;
	}

	// Run simulation
	if (strcmp(argv[1], "-r") == 0 or strcmp(argv[1], "--run") == 0) {
		LoadBodiesFromFile("init/output.txt", bodyList);

		Video video = Video("images/", "image_", width, height, framerate);
		video.ClearImageFolder();

		for (int f = 0; f < frames; f++) {
			// Reset force counter on each body
			body = bodyList.GetHead();
			while (body != NULL) {
				body->ResetForce();
				body = body->next;
			}

			// n-body Algorithm (optimised)
			bodyA = bodyList.GetHead();
			bodyB = NULL;

			while (bodyA != NULL) {
				bodyB = bodyA->next;

				while (bodyB != NULL) {
					// Calculate distance
					double xDistance = bodyA->GetX() - bodyB->GetX();
					double yDistance = bodyA->GetY() - bodyB->GetY();
					double zDistance = bodyA->GetZ() - bodyB->GetZ();
					double totalDistance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));

					// Calculate angles
					double phiAngle = atan2(zDistance, sqrt(pow(xDistance, 2) + pow(yDistance, 2)));
					double thetaAngle = atan2(yDistance, xDistance);

					// Calculate force
					double force = GR * ((bodyA->GetMass() * bodyB->GetMass()) / (pow(totalDistance, 2)));

					// Add forces to totals
					bodyA->AddForce(force, phiAngle, thetaAngle);
					bodyB->AddForce(-force, phiAngle, thetaAngle);

					// Advance pointer
					bodyB = bodyB->next;
				}

				bodyA = bodyA->next;
			}

			string imageFileName = "images/image_" + PadWithZeroes(f, frames) + ".png";
			Image image = Image(imageFileName, width, height, 100);

			// Calculate next position for each body
			body = bodyList.GetHead();
			while (body != NULL) {
				body->Update(dt);
				body = body->next;
			}

			// Collision Physics
			bodyA = bodyList.GetHead();
			bodyB = NULL;

			while (bodyA != NULL) {
				bodyB = bodyA->next;

				while (bodyB != NULL) {
					double collisionTime = -1;

					// Set up position vectors
					Vector initialA;
					initialA.Set(bodyA->GetX(), bodyA->GetY(), bodyA->GetZ());

					Vector finalA;
					finalA.Set(bodyA->GetNextX(), bodyA->GetNextY(), bodyA->GetNextZ());

					Vector initialB;
					initialB.Set(bodyB->GetX(), bodyB->GetY(), bodyB->GetZ());

					Vector finalB;
					finalB.Set(bodyB->GetNextX(), bodyB->GetNextY(), bodyB->GetNextZ());

					// Check the two lines are not parallel
					Vector lineA = finalA.Subtract(initialA);
					Vector lineB = finalB.Subtract(initialB);

					if (lineA.DotProduct(lineB) != 0) {
						// Calculate collision times
						Vector vectorA = initialB.Subtract(initialA);
						Vector vectorB = (finalB.Subtract(finalA)).Add(initialA.Subtract(initialB));

						double dotProduct = vectorA.DotProduct(vectorB);
						double radiiSum = bodyA->GetRadius() + bodyB->GetRadius();

						double determinant = (4 * pow(dotProduct, 2)) - (4 * pow(vectorB.Magnitude(), 2) * (pow(vectorA.Magnitude(), 2) - pow(radiiSum, 2)));
						double time1 = (-2 * dotProduct + sqrt(determinant)) / (2 * pow(vectorB.Magnitude(), 2));
						double time2 = (-2 * dotProduct - sqrt(determinant)) / (2 * pow(vectorB.Magnitude(), 2));

						bool timeValid1 = time1 >= 0 && time1 <= 1;
						bool timeValid2 = time2 >= 0 && time2 <= 1;

						// Check validity
						if (timeValid1 && timeValid2) {
							if (time1 <= time2) {
								collisionTime = time1 * dt;
							}
							else {
								collisionTime = time2 * dt;
							}
						}
					}

					// Collide particles if collision has occured
					if (collisionTime != -1) {
						double newMass = bodyA->GetMass() + bodyB->GetMass();

						// Conservation of linear momentum
						// X
						double aXVelocity = bodyA->GetXVelocity();
						double bXVelocity = bodyB->GetXVelocity();
						double newXVelocity = ((bodyA->GetMass() * aXVelocity) + (bodyB->GetMass() * bXVelocity)) / (newMass);

						// Y
						double aYVelocity = bodyA->GetYVelocity();
						double bYVelocity = bodyB->GetYVelocity();
						double newYVelocity = ((bodyA->GetMass() * aYVelocity) + (bodyB->GetMass() * bYVelocity)) / (newMass);

						// Z
						double aZVelocity = bodyA->GetZVelocity();
						double bZVelocity = bodyB->GetZVelocity();
						double newZVelocity = ((bodyA->GetMass() * aZVelocity) + (bodyB->GetMass() * bZVelocity)) / (newMass);

						// Calculate new position
						// Use midpoint of the two "new positions"
						bodyA->Update(collisionTime);
						bodyB->Update(collisionTime);

						double newX = (bodyA->GetNextX() + bodyB->GetNextX()) / 2;
						double newY = (bodyA->GetNextY() + bodyB->GetNextY()) / 2;
						double newZ = (bodyA->GetNextZ() + bodyB->GetNextZ()) / 2;

						// Calculate new radius
						// TODO: Figure out way of calculating a new radius given constant density?
						// At the moment, use larger radius of the two bodies
						double newRadius;
						if (bodyA->GetRadius() >= bodyB->GetRadius()) {
							newRadius = bodyA->GetRadius();
						}
						else {
							newRadius = bodyB->GetRadius();
						}

						// Create a new (combined) body, and remove A and B;
						bodyList.Remove(bodyA->id);
						bodyList.Remove(bodyB->id);

						Body * newBody = new Body(newX, newY, newZ, newMass, newRadius, newXVelocity, newYVelocity, newZVelocity);
						newBody->Update(dt - collisionTime);
						bodyList.Append(newBody);
					}

					bodyB = bodyB->next;
				}

				bodyA = bodyA->next;
			}

			// Move each body to their new positions
			body = bodyList.GetHead();
			while (body != NULL) {
				body->Step();
				image.DrawBody(body->GetX(), body->GetY(), 255, 255, 255);

				body = body->next;
			}

			// Draw information on frame
			image.DrawText("SIMULATION", 10, 10, 255, 255, 255);
			image.DrawText("F: " + to_string(f), 10, 20, 255, 255, 255);
			image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 255, 255, 255);

			image.Save();
		}

		video.Build("result_run.mp4", frames);

		// Create output.txt
		Output output("init/output.txt", width, height, 100);
		output.AddAllBodies(bodyList);
		output.Save();

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
