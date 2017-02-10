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

int main(int argc, char * argv[]) {
	string usageStatement = "Usage: " + (string)argv[0] + " -c configfile.scfg (-g --generate (random|rotate|zoom) )|(-r --run framecount)";

	string simulationName;
	int bodyCount;
	int width, height;
	double scale;
	double dt;
	int framerate;

	List bodyList = List();
	Body * body;
	Body * bodyA;
	Body * bodyB;

	////////////////// TESTING /////////////////////

	LoadFromFile("init/testing.txt", simulationName, width, height, scale, framerate, dt, bodyList);

	string imageFileName = simulationName + ".png";
	Image image = Image(imageFileName, width, height, scale);

	// Add details to image
	image.DrawText(simulationName.c_str(), 10, 10, 155, 155, 155);
	image.DrawText("W: " + to_string(width), 10, 20, 155, 155, 155);
	image.DrawText("H: " + to_string(height), 10, 30, 155, 155, 155);
	image.DrawText("S: " + to_string(scale), 10, 40, 155, 155, 155);
	image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 50, 155, 155, 155);
	image.DrawText("F: " + to_string(framerate), 10, 60, 155, 155, 155);
	image.DrawText("T: " + to_string(dt), 10, 70, 155, 155, 155);
	image.DrawScale(scale, 10, height - 15, 55, 55, 55);
	image.DrawAllBodies(bodyList, 255, 255, 255);
	image.Save();

	return 0;

	///////////////////////////////////////////////

	// Check command is valid using regex
	regex validCommand("-c [\\w]+.scfg (((\\-g|\\-\\-generate) ([a-z]+)( [1-9][0-9]*))|((\\-r|\\-\\-run) [0-9]* [0-9]*))");

	// Join argv into command string (ignoring program name)
	string command = "";
	for (int i = 1; i < argc; i++) {
		command += (string)argv[i];
		if (i != argc - 1) {
			command += " ";
		}
	}

	if (!regex_match(command, validCommand)) {
		cout << "Invalid command. See usage statement." << endl;
		cout << usageStatement << endl;
		return 1;
	}

	// Load settings (parameter values) from config file
	ifstream configFile((string)argv[2]);
	string value;

	regex integer("[1-9][0-9]*");
	regex decimal("([1-9][0-9]*)(\\.[0-9]*)");
	regex stdform("([1-9][0-9]*)(\\.)*[0-9]*e(-*[1-9][0-9]*)");

	// Width
	getline(configFile, value);
	if (regex_match(value, integer)) {
		width = stoi(value);
	}
	else {
		cout << "Width parameter is not a valid integer (line 1) of " << (string)argv[2] << endl;
		return 1;
	}

	// Height
	getline(configFile, value);
	if (regex_match(value, integer)) {
		height = stoi(value);
	}
	else {
		cout << "Height parameter is not a valid integer (line 2) of " << (string)argv[2] << endl;
		return 1;
	}

	// Body Count - How many bodies to generate when using generate commands
	getline(configFile, value);
	if (regex_match(value, integer)) {
		bodyCount = stoi(value);
	}
	else {
		cout << "Body count parameter is not a valid integer (line 3) of " << (string)argv[2] << endl;
		return 1;
	}

	// Scale
	getline(configFile, value);
	if (regex_match(value, integer) or regex_match(value, decimal) or regex_match(value, stdform)) {
		scale = stod(value.c_str());
	}
	else {
		cout << "Scale parameter is not a valid double (line 4) of " << (string)argv[2] << endl;
	}

	// dt
	string dtErrorMessage = "dt parameter is not a valid double or time value (DAY, etc.) (line 5) of " + (string)argv[2];
	getline(configFile, value);
	if (regex_match(value, decimal) or regex_match(value, integer)) {
		dt = atof(value.c_str());
	}
	else {
		regex division("\\/");
		regex multiplication("\\*");

		if (regex_search(value, division)) {
			stringstream valueDiv(value);
			getline(valueDiv, value, '/');

			if (value == "DAY") {
				getline(valueDiv, value, '/');
				if (regex_match(value, integer) or regex_match(value, decimal) or regex_match(value, stdform)) {
					dt = DAY / atof(value.c_str());
				}
				else {
					cout << dtErrorMessage << endl;
					return 1;
				}
			}
			else if (value == "YR" or value == "YEAR") {
				getline(valueDiv, value, '/');
				if (regex_match(value, integer) or regex_match(value, decimal) or regex_match(value, stdform)) {
					dt = YR / atof(value.c_str());
				}
				else {
					cout << dtErrorMessage << endl;
					return 1;
				}
			}
			else {
				cout << dtErrorMessage << endl;
				return 1;
			}
		}
		else if (regex_search(value, multiplication)) {
			stringstream valueMult(value);
			getline(valueMult, value, '*');
			
			if (value == "DAY") {
				getline(valueMult, value, '*');
				if (regex_match(value, integer) or regex_match(value, decimal) or regex_match(value, stdform)) {
					dt = DAY * atof(value.c_str());
				}
				else {
					cout << dtErrorMessage << endl;
					return 1;
				}
			}
			else if (value == "YR" or value == "YEAR") {
				getline(valueMult, value, '*');
				if (regex_match(value, integer) or regex_match(value, decimal) or regex_match(value, stdform)) {
					dt = YR * atof(value.c_str());
				}
				else {
					cout << dtErrorMessage << endl;
					return 1;
				}
			}
			else {
				cout << dtErrorMessage << endl;
				return 1;
			}
		}
		else {
			if (value == "DAY") {
				dt = DAY;
			}
			else if (value == "YR" or value == "YEAR") {
				dt = YR;
			}
			else {
				cout << dtErrorMessage << endl;
				return 1;
			}
		}
	}

	// framerate
	getline(configFile, value);
	if (regex_match(value, integer)) {
		framerate = atof(value.c_str());
	}
	else {
		cout << "Framerate parameter is not a valid integer (line 6) of " << (string)argv[2] << endl;
		return 1;
	}

	// Generate Body arrangement
	string mainCommand = (string)argv[3];
	
	// For the generate commands - random seed
	srand(time(NULL));

	if (mainCommand == "-g" or mainCommand == "--generate") {
		string generateCommand = (string)argv[4];
		string zoomScale = (string)argv[5];

		// Random - generate random arrangement of bodies
		if (generateCommand == "shell") {
			for (int i = 0; i < bodyCount - 1; i++) {
				double r = 1e11;
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
			Output output("init/output.txt");
			output.AddAllBodies(bodyList);
			output.Save();

			return 0;
		}

		// Random arrangement of stationary particles
		else if (generateCommand == "random") {
			for (int i = 0; i < bodyCount; i++) {
				double x = Random(-3e11, 3e11);
				double y = Random(-3e11, 3e11);
				double z = Random(-3e11, 3e11);

				double mass = Random(1e23, 1e27);
				double radius = Random(1e6, 1e8);

				bodyList.Append(new Body(x, y, z, mass, radius, 0, 0, 0));
			}

			// Save bodies to output.txt
			Output output("init/output.txt");
			output.AddAllBodies(bodyList);
			output.Save();

			return 0;
		}

		// Generate rotate video around bodies
		else if (generateCommand == "rotate") {
			//LoadBodiesFromFile("init/output.txt", bodyList);

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
			video.Build("result.mp4", 360);

			return 0;
		}

		// Generate zoom (set new scale) video
		else if (generateCommand == "zoom") {
			//LoadBodiesFromFile("init/output.txt", bodyList);

			Video video = Video("images/", "image_", width, height, framerate);
			video.ClearImageFolder();

			double finalScale = stod(zoomScale);
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

			// Build video from images
			video.Build("zoom_result.mp4", abs(finalScale - scale));

			return 0;
		}

		else {
			cout << "Not a valid generate command. Please specify a valid command." << endl;
			cout << usageStatement << endl;

			return 1;
		}
	}

	// Run simulation
	else if (mainCommand == "-r" or mainCommand == "-run") {
		int frames = stoi((string)argv[4]);
		int currentFrames = stoi((string)argv[5]);
		
		if (frames == 0) {
			cout << "Simulation must run for at least 1 frame." << endl;
			cout << usageStatement << endl;
			return 1;
		}

		//LoadBodiesFromFile("init/output.txt", bodyList);

		Video video = Video("images/", "image_", width, height, framerate);
		video.ClearImageFolder();

		double elapsedTime = 0;

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
			string timeUnits;

			if (elapsedTime >= 2 * YR) {
				timeUnits = " YEARS";
				elapsedTimeString =  RemoveTrailingZeroes(to_string(elapsedTime / ((double)YR))); 
			}
			else {
				if (elapsedTime == DAY) {
					timeUnits = " DAY";
				}
				else {
					timeUnits = " DAYS";
				}
				elapsedTimeString =  RemoveTrailingZeroes(to_string(elapsedTime / (double)DAY));
			}

			// Draw information on frame
			image.DrawText("SIMULATION  (" + timeUnits + ") " + elapsedTimeString, 10, 10, 155, 155, 155);
			image.DrawText("F: " + to_string(f + currentFrames), 10, 20, 155, 155, 155);
			image.DrawText("N: " + to_string(bodyList.GetLength()), 10, 30, 155, 155, 155);

			image.DrawScale(scale, 10, height - 15, 55, 55, 55);

			image.Save();
		}

		video.Build("result_run.mp4", frames);

		// Create output.txt
		Output output("init/output.txt");
		output.AddAllBodies(bodyList);
		output.Save();

		return 0;
	}
}
