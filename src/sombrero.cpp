#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <regex>
#include <map>

#include "lib/font.h"
#include "lib/body.h"
#include "lib/image.h"
#include "lib/video.h"
#include "lib/output.h"
#include "lib/misc.h"
#include "lib/matrix.h"
#include "lib/linkedlist.h"
#include "lib/units.h"

#include "sombrero.h"

#include <pngwriter.h>

using namespace std;

string Simulation::GetTimeUnits(double time) {
	regex validInteger("[1-9][0-9]*");
	string timeUnits;

	if (time < DAY) {
			if (regex_match(to_string(1 / (time / DAY)), validInteger)) {
				// if it is in the form DAY / n where n is an integer
				timeUnits = "DAYS ";
			}
			else {
				timeUnits = "SECS ";
			}
	}
	else if (time > DAY) {
		if (time < 2 * YR) {
			timeUnits = "DAYS ";
		}
		else {
			timeUnits = "YEARS";
		}
	}
	else if (time == DAY) {
		timeUnits = "DAYS ";
	}

	return timeUnits;
}

Simulation::Simulation() {
	bodyList = List();
}

Simulation::Simulation(int _width, int _height, double _scale, double _dt, int _framerate) {
	bodyList = List();

	width = _width;
	height = _height;
	scale = _scale;
	dt = _dt;
	framerate = _framerate;
}

Simulation::Simulation(string _name, int _width, int _height, double _scale, double _dt, int _framerate) {
	bodyList = List();

	name = _name;
	width = _width;
	height = _height;
	scale = _scale;
	dt = _dt;
	framerate = _framerate;
}

bool Simulation::LoadBodiesFromFile(string _filename) {
	regex validLine("(\\s*([+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?)\\s*,){7}\\s*[+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?\\s*(//(\\s*\\S*)*)?\r?");
	regex validCommentLine("\\s*//(\\s*\\S*)*\r?");

	ifstream inputFile(_filename);
	string fileLine;

	// Read in body details
	string parameter;
	int i;

	if (!FileExists(_filename)) {
		return false;
	}

	while (getline(inputFile, fileLine))
	{
		if (regex_match(fileLine, validCommentLine)) {
			continue;
		}
		else if (!regex_match(fileLine, validLine)) {
			return false;
		}
		else {
			stringstream bodyDetails(fileLine);
			string detailArray [8];
			i = 0;

			while (getline(bodyDetails, parameter, ','))
			{
				detailArray[i] = parameter;
				i++;
			}

			double x = stod(detailArray[0]);
			double y = stod(detailArray[1]);
			double z = stod(detailArray[2]);
			double mass = stod(detailArray[3]);
			double radius = stod(detailArray[4]);
			double xVelocity = stod(detailArray[5]);
			double yVelocity = stod(detailArray[6]);
			double zVelocity = stod(detailArray[7]);

			bodyList.Append(new Body(x, y, z, mass, radius, xVelocity, yVelocity, zVelocity));
		}
	}

	return true;
}

void Simulation::GenerateRandomShell(int _bodyCount) {
	srand(time(NULL));
	bodyList = List();

	for (int i = 0; i < _bodyCount - 1; i++) {
		double r = 0.5e11;
		double theta = Random(0, 2 * PI);
		double phi = Random(0, 2 * PI);

		double x = r * cos(theta) * cos(phi);
		double y = r * sin(theta);
		double z = r * cos(theta) * sin(phi);

		double mass = Random(1e23, 1e24);

		bodyList.Append(new Body(x, y, z, mass, Random(1e6, 9e6), 0, 0, 0));
	}

	bodyList.Append(new Body(0.0, 0.0, 0.0, 1e30, 1e8, 0.0, 0.0, 0.0));

	// Save bodies to output.txt
	Output output(outputFolder + name + ".csv");
	output.AddAllBodies(bodyList);
	output.Save();
}

void Simulation::GenerateRandomDistribution(int _bodyCount) {
	srand(time(NULL));
	bodyList = List();

	for (int i = 0; i < _bodyCount; i++) {
		double x = Random(-3e11, 3e11);
		double y = Random(-3e11, 3e11);
		double z = Random(-3e11, 3e11);

		double mass = Random(1e23, 1e27);
		double radius = Random(1e6, 1e8);

		bodyList.Append(new Body(x, y, z, mass, radius, 0, 0, 0));
	}

	// Save bodies to output.txt
	Output output(outputFolder + name + ".csv");
	output.AddAllBodies(bodyList);
	output.Save();
}

void Simulation::Rotate() {
	Video video = Video("images/", "image_", width, height, framerate);
	video.ClearImageFolder();

	// Rotate bodies about the y-axis
	for (double angle = 0.0; angle < 360.0; angle ++) {
		string imageFileName = "images/image_" + PadWithZeroes(angle, 360) + ".png";
		Image image = Image(imageFileName, width, height, scale);

		body = bodyList.GetHead();
		while (body != NULL) {
			// Rotate body
			Vector p;
			p.Set(body->GetX(), body->GetY(), body->GetZ());

			Vector t;
			t = p.RotateY(angle);
			t = t.RoundValues();

			image.DrawBody(t.GetX(), t.GetY(), body->GetRadius(), 255, 255, 255);

			body = body->next;
		}

		// Add details to image
		image.DrawText("ROTATION", 10, 10, 155, 155, 155);
		image.DrawText("A: " + to_string((int)angle), 10, 20, 155, 155, 155);
		image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

		image.DrawScale(scale, 10, height - 15, 55, 55, 55);

		image.Save();
	}

	// Build video from images
	video.Build(outputFolder + name + ".mp4", 360);
}

void Simulation::Scale(double finalScale, bool updateScale) {
	Video video = Video("images/", "image_", width, height, framerate);
	video.ClearImageFolder();

	double scaleStep;

	if (finalScale < scale) {
		scaleStep = -1;
	}
	else {
		scaleStep = 1;
	}

	int frameCount = abs(finalScale - scale);

	double currentScale = scale;
	while (abs(finalScale - currentScale) != 0) {
		string imageFileName = "images/image_" + PadWithZeroes(abs(currentScale - scale), frameCount) + ".png";
		Image image = Image(imageFileName, width, height, currentScale);

		image.DrawAllBodies(bodyList, 255, 255, 0);

		// Add details to image
		image.DrawText("ZOOM", 10, 10, 155, 155, 155);
		image.DrawText("S: " + RemoveTrailingZeroes(to_string(currentScale)), 10, 20, 155, 155, 155);
		image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

		image.DrawScale(currentScale, 10, height - 15, 55, 55, 55);

		image.Save();

		currentScale += scaleStep;
	}

	if (updateScale) {
		scale = finalScale;
	}

	// Build video from images
	video.Build(outputFolder + name + "_zoom.mp4", abs(finalScale - scale));
}

void Simulation::Run(int startingFrame, int framesToSimulate) {
	int currentFrames = 0;
	int initialFrameCounter = startingFrame;

	Video video = Video("images/", "image_", width, height, framerate);
	video.ClearImageFolder();

	double elapsedTime = (startingFrame - 1) * dt;

	if (startingFrame == 0) {
		// Produce frame showing initial setup of bodies (if starting from frame 0)
		string minimumTimeUnits = GetTimeUnits(dt);

		string imageFileName = "images/image_" + PadWithZeroes(0, framesToSimulate) + ".png";
		Image image = Image(imageFileName, width, height, scale);

		image.DrawAllBodies(bodyList, 255, 255, 255);

		// Draw information on frame
		image.DrawText("SIMULATION  " + minimumTimeUnits + " " + "0.0", 10, 10, 155, 155, 155);
		image.DrawText("F: 0", 10, 20, 155, 155, 155);
		image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

		image.DrawScale(scale, 10, height - 15, 55, 55, 55);

		image.Save();

		initialFrameCounter++;
		elapsedTime = 0;
	}

	// Simulate the next frames
	// <=, as it simulates 0->1, 1->2 therefore needs to simulate -> n.	
	for (int f = initialFrameCounter; f <= framesToSimulate + startingFrame; f++) {
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
				double force = gravConst * ((bodyA->GetMass() * bodyB->GetMass()) / (pow(totalDistance, 2)));

				// Add forces to totals
				bodyA->AddForce(force, phiAngle, thetaAngle);
				bodyB->AddForce(-force, phiAngle, thetaAngle);

				// Advance pointer
				bodyB = bodyB->next;
			}

			bodyA = bodyA->next;
		}

		string imageFileName = "images/image_" + PadWithZeroes(f - startingFrame, framesToSimulate) + ".png";
		Image image = Image(imageFileName, width, height, scale);

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
			image.DrawBody(body->GetX(), body->GetY(), body->GetRadius(), 255, 255, 255);

			body = body->next;
		}

		elapsedTime += dt;

		// Format elapsed time
		string elapsedTimeString;
		string timeUnits = GetTimeUnits(elapsedTime);

		if (timeUnits == "YEARS") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime / (double)YR));
		}
		else if (timeUnits == "DAYS ") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime / (double)DAY));
		}
		else if (timeUnits == "SECS ") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime));
		}

		// Draw information on frame
		image.DrawText("SIMULATION  " + timeUnits + " " + elapsedTimeString, 10, 10, 155, 155, 155);
		image.DrawText("F: " + to_string(f + currentFrames), 10, 20, 155, 155, 155);
		image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

		image.DrawScale(scale, 10, height - 15, 55, 55, 55);

		image.Save();
	}

	video.Build(outputFolder + name + "_run.mp4", framesToSimulate);

	// Create _output.csv
	Output output(outputFolder + name + "_output" + ".csv");
	output.AddAllBodies(bodyList);
	output.Save();
}

/*
int main() {
	Simulation sim = Simulation();

	sim.SetSimulationName("shell");
	sim.SetScale(AU, 150);
	sim.SetFramerate(60);
	sim.SetTimestep(3600);

	sim.GenerateRandomShell(100);

	sim.Run(0, 100);

	Simulation sim = Simulation();

	sim.SetSimulationName("PlutoCharon");
	sim.SetScale(1.3e7, 100);
	sim.SetFramerate(120);
	sim.SetTimestep(DAY / 16);

	sim.LoadBodiesFromFile("plutocharon.csv");

	sim.Run(0, 800);

	Simulation sim = Simulation();

	if (sim.LoadBodiesFromFile("test.csv")) {
		cout << "Read successfully!" << endl;
	}
	else {
		cout << "Failed to load bodies!" << endl;
	}

	cout << sim.GetNumberOfBodies() << endl;

	sim.SetTimestep(YR);
	sim.Run(0, 2);
}
*/