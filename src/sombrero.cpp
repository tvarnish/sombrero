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
#include "lib/position_vector.h"
#include "lib/linkedlist.h"
#include "lib/units.h"

#include "sombrero.h"

#include <pngwriter.h>

using namespace std;

string Simulation::GetTimeUnits(double time) {
	// Determine, and return, the time units (Seconds, Days, Years) for the given time.
	regex validInteger("[1-9][0-9]*");
	string timeUnits;

	// Note: space included for "DAYS " purely for spacing when output
	if (time < DAY) {
			if (regex_match(to_string(1 / (time / DAY)), validInteger)) {
				// Check if the time is in the form DAY / n, where n is an integer
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
	// Constructor for Simulation object with default parameters
	bodyList = List();
}

Simulation::Simulation(int _width, int _height, double _scale, double _dt, int _framerate) {
	// Constructor for Simulation object - key parameters defined
	bodyList = List();

	width = _width;
	height = _height;
	scale = _scale;
	dt = _dt;
	framerate = _framerate;
}

Simulation::Simulation(string _name, int _width, int _height, double _scale, double _dt, int _framerate) {
	// Constructor for Simulation object - key parameters defined (except simulation name)
	bodyList = List();

	name = _name;
	width = _width;
	height = _height;
	scale = _scale;
	dt = _dt;
	framerate = _framerate;
}

bool Simulation::LoadBodiesFromFile(string _fileName) {
	// Load a simulation set-up from a body .csv file, into the bodyList
	regex validLine("(\\s*([+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?)\\s*,){7}\\s*[+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?\\s*(//(\\s*\\S*)*)?\r?");
	regex validCommentLine("\\s*//(\\s*\\S*)*\r?");

	ifstream inputFile(_fileName);
	string fileLine, parameter;
	int i;

	// Return false if the file does not exist (raise error)
	if (!FileExists(_fileName)) {
		return false;
	}

	// Iterate through lines in the file
	while (getline(inputFile, fileLine))
	{
		if (regex_match(fileLine, validCommentLine)) {
			// Ignore line if it is a comment line
			continue;
		}
		else if (!regex_match(fileLine, validLine)) {
			// Return false if the line is not valid (raise error)
			return false;
		}
		else {
			stringstream bodyDetails(fileLine);
			double detailArray [8];
			i = 0;

			// Iterate through each comma separated value, and store each in the detailArray (for easy access later)
			while (getline(bodyDetails, parameter, ','))
			{
				detailArray[i] = stod(parameter);
				i++;
			}

			double x = detailArray[0];
			double y = detailArray[1];
			double z = detailArray[2];
			double mass = detailArray[3];
			double radius = detailArray[4];
			double xVelocity = detailArray[5];
			double yVelocity = detailArray[6];
			double zVelocity = detailArray[7];

			bodyList.Append(new Body(x, y, z, mass, radius, xVelocity, yVelocity, zVelocity));
		}
	}

	return true;
}

void Simulation::GenerateRandomShell(int _bodyCount) {
	// Generate a set up of bodies, in a randomly spaced shell (equidistant to the centre) around a central heavy body
	srand(time(NULL));
	bodyList = List();

	for (int i = 0; i < _bodyCount - 1; i++) {
		// Generate the body's location with spherical coordinates
		double r = 0.5e11;
		double theta = Random(0, 2 * PI);
		double phi = Random(0, 2 * PI);

		// Convert spherical coordinates to cartesian coordinates
		double x = r * cos(theta) * cos(phi);
		double y = r * sin(theta);
		double z = r * cos(theta) * sin(phi);

		double mass = Random(1e23, 1e24);

		bodyList.Append(new Body(x, y, z, mass, Random(1e6, 9e6), 0, 0, 0));
	}

	// Add the central heavy body to the body list
	bodyList.Append(new Body(0.0, 0.0, 0.0, 1e30, 1e8, 0.0, 0.0, 0.0));

	// Save bodies to _output.csv
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

	// Save bodies to output.csv
	Output output(outputFolder + name + ".csv");
	output.AddAllBodies(bodyList);
	output.Save();
}

void Simulation::Rotate(string buildingMessage) {
	// Generate a video, rotating the three-dimensional set up around the y-axis
	Video video = Video("images/", "image_", width, height, framerate);
	video.ClearImageFolder();

	// Iterate through each frame of the video (to be generated) / angle
	for (double angle = 0.0; angle < 360.0; angle ++) {
		string imageFileName = "images/image_" + PadWithZeroes(angle, 360) + ".png";
		Image image = Image(imageFileName, width, height, scale);

		// Iterate through the bodies and transform their positions
		body = bodyList.GetHead();
		while (body != NULL) {
			// Rotate body
			Vector position = body->GetPosition();
			Vector transformedPosition = position.RotateY(angle);
			transformedPosition = transformedPosition.RoundValues();

			image.DrawBody(transformedPosition.GetX(), transformedPosition.GetY(), body->GetRadius(), 255, 255, 255);

			body = body->GetNext();
		}

		// Draw details of simulation to frame
		image.DrawText("ROTATION", 10, 10, 155, 155, 155);
		image.DrawText("A: " + to_string((int)angle), 10, 20, 155, 155, 155);
		image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

		image.DrawScale(scale, 10, height - 15, 55, 55, 55);

		image.Save();
	}

	// Build video from images
	video.Build(outputFolder + name + ".mp4", 360, buildingMessage);
}

void Simulation::Scale(double finalScale, bool updateScale, string buildingMessage) {
	// Create a video, zooming in or out of (scaling) the simulation
	Video video = Video("images/", "image_", width, height, framerate);
	video.ClearImageFolder();

	double scaleStep;

	// Determine the "direction" of the scaling (in or out)
	if (finalScale < scale) {
		scaleStep = -1;
	}
	else {
		scaleStep = 1;
	}

	int frameCount = abs(finalScale - scale);

	// Generate the frames of the video
	double currentScale = scale;
	while (abs(finalScale - currentScale) != 0) {
		// Image is scaled by passing in a modified scale to the image object (simulation scale is unaffected unless updateScale is true)
		string imageFileName = "images/image_" + PadWithZeroes(abs(currentScale - scale), frameCount) + ".png";
		Image image = Image(imageFileName, width, height, currentScale);

		image.DrawAllBodies(bodyList, 255, 255, 0);

		// Draw details of simulation to frame
		image.DrawText("ZOOM", 10, 10, 155, 155, 155);
		image.DrawText("S: " + RemoveTrailingZeroes(to_string(currentScale)), 10, 20, 155, 155, 155);
		image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

		image.DrawScale(currentScale, 10, height - 15, 55, 55, 55);

		image.Save();

		currentScale += scaleStep;
	}

	// If updateScale is true, change the simulation's scale (update the scale for run, rotate, etc.)
	if (updateScale) {
		scale = finalScale;
	}

	// Build video from images
	video.Build(outputFolder + name + "_zoom.mp4", abs(finalScale - scale), buildingMessage);
}

void Simulation::Run(int startingFrame, int framesToSimulate, string buildingMessage) {
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
	// <=, as it simulates 0->1, 1->2 therefore needs to simulate up to n.	
	for (int f = initialFrameCounter; f <= framesToSimulate + startingFrame; f++) {
		// Reset force counter on each body
		// (as force is used to count the net force on the body for that frame)
		body = bodyList.GetHead();
		while (body != NULL) {
			body->ResetForce();
			body = body->GetNext();
		}

		// n-body algorithm (optimised)
		// i.e. (1: 2,3,4,5; 2: 3,4,5; etc)
		bodyA = bodyList.GetHead();
		bodyB = NULL;

		while (bodyA != NULL) {
			bodyB = bodyA->GetNext();

			while (bodyB != NULL) {
				// Calculate distance between objects
				Vector distance = bodyA->GetPosition().Subtract(bodyB->GetPosition());
				double totalDistance = distance.Magnitude();

				// Calculate angles
				double phiAngle = atan2(distance.GetZ(), sqrt(pow(distance.GetX(), 2) + pow(distance.GetY(), 2)));
				double thetaAngle = atan2(distance.GetY(), distance.GetX());

				// Calculate force
				double force = -1 * gravConst * ((bodyA->GetMass() * bodyB->GetMass()) / (pow(totalDistance, 2)));

				// Add forces to totals
				bodyA->AddForce(force, phiAngle, thetaAngle);
				bodyB->AddForce(-force, phiAngle, thetaAngle);

				// Advance pointer
				bodyB = bodyB->GetNext();
			}

			bodyA = bodyA->GetNext();
		}

		string imageFileName = "images/image_" + PadWithZeroes(f - startingFrame, framesToSimulate) + ".png";
		Image image = Image(imageFileName, width, height, scale);

		// Calculate next position for each body
		body = bodyList.GetHead();
		while (body != NULL) {
			body->Update(dt);
			body = body->GetNext();
		}

		// Collision Physics
		bodyA = bodyList.GetHead();
		bodyB = NULL;

		while (bodyA != NULL) {
			bodyB = bodyA->GetNext();

			while (bodyB != NULL) {
				double collisionTime = -1;

				// Set up position vectors
				Vector initialA = bodyA->GetPosition();
				Vector finalA = bodyA->GetNextPosition();
				
				Vector initialB = bodyB->GetPosition();
				Vector finalB = bodyB->GetNextPosition();
				
				// Check the two lines are not parallel (using direction vectors)
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

					// Check if the collision occurs within the timestep
					if (timeValid1 && timeValid2) {
						if (time1 <= time2) {
							collisionTime = time1 * dt;
						}
						else {
							collisionTime = time2 * dt;
						}
					}
				}

				// Collide particles if collision will occur
				if (collisionTime != -1) {
					double newMass = bodyA->GetMass() + bodyB->GetMass();

					// Conservation of linear momentum (assuming bodies will combine)
					Vector aMomentum = bodyA->GetVelocity().Multiply(bodyA->GetMass());
					Vector bMomentum = bodyB->GetVelocity().Multiply(bodyB->GetMass());
					Vector newVelocity = (aMomentum.Add(bMomentum)).Divide(newMass);

					// Calculate position of new body
					// Use midpoint of the two "collision positions"
					bodyA->Update(collisionTime);
					bodyB->Update(collisionTime);

					// Average the centres of the bodies at their collision positions
					Vector newPosition = (bodyA->GetNextPosition().Add(bodyB->GetNextPosition())).Divide(2);

					// Calculate new radius - use volumes of the two materials
					double volumeA = (4/3) * PI * pow(bodyA->GetRadius(), 3);
					double volumeB = (4/3) * PI * pow(bodyB->GetRadius(), 3);
					double newVolume = volumeA + volumeB;

					double newRadius = pow(newVolume / ((4/3) * PI), (1/3));

					// Create a new (combined) body, and remove A and B;
					bodyList.Remove(bodyA->GetID());
					bodyList.Remove(bodyB->GetID());

					Body * newBody = new Body(newPosition, newMass, newRadius, newVelocity);

					newBody->Update(dt - collisionTime);
					bodyList.Append(newBody);
				}

				bodyB = bodyB->GetNext();
			}

			bodyA = bodyA->GetNext();
		}

		// Move each body to their new positions
		body = bodyList.GetHead();
		while (body != NULL) {
			body->Step();

			image.DrawBody(body->GetX(), body->GetY(), body->GetRadius(), 255, 255, 255);

			body = body->GetNext();
		}

		// Update the elapsed time
		elapsedTime += dt;

		// Format elapsed time (for image output)
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

	video.Build(outputFolder + name + "_run.mp4", framesToSimulate, buildingMessage);

	// Create _output.csv
	Output output(outputFolder + name + "_output" + ".csv");
	output.AddAllBodies(bodyList);
	output.Save();
}

void Simulation::SaveOutputFile() {
	// Save bodies to output.csv
	Output output(outputFolder + name + ".csv");
	output.AddAllBodies(bodyList);
	output.Save();
}

void Simulation::AddBody(Body * body) {
	bodyList.Append(body);
}

void Simulation::ClearBodyList() {
	bodyList = List();
}