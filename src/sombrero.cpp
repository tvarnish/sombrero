#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <regex>
#include <map>

#include "lib/body.h"
#include "lib/output.h"
#include "lib/misc.h"
#include "lib/matrix.h"
#include "lib/position_vector.h"
#include "lib/linkedlist.h"
#include "lib/units.h"

#include "sombrero.h"

using namespace std;

int main(int argc, char *argv[]) {
	Simulation sim = Simulation();

	string bodyFile;
	double timestep;
	int stepCount;

	int requiredFlagCount = 3;
	int validFlagCount = 0;

	if (argc > 1) {
		// Parse command line arguments
		string currentFlag = "";
    
		for (int argumentIndex = 1; argumentIndex < argc; argumentIndex++) {
			string currentArgument = argv[argumentIndex];

			if (currentArgument[0] == '-') {
				currentFlag = currentArgument.substr(1);
        
        // Show usage message
        if (currentFlag == "h") {
          cout << "Usage: ./sombrero -i init_filename.csv -s step_count -dt time_step" << endl;
          return 0;
        }
			}
			else if (currentFlag != "") { 
				if (currentFlag == "i") {         
					bodyFile = currentArgument;
					validFlagCount++;
				}
				else if (currentFlag == "s") {
					try {
						stepCount = stoi(currentArgument);
						validFlagCount++;
					}
					catch (...) {
						cout << "ERROR: Simulation step count value is invalid." << endl;
						return 1;
					}
				}
				else if (currentFlag == "dt") {
					try {
						timestep = stod(currentArgument);
						validFlagCount++;
					}
					catch (...) {
						cout << "ERROR: Simulation time step value (dt) is invalid." << endl;
						return 1;
					}
				}
			}
		}
	}

	// Check all required flags set
	if (validFlagCount != requiredFlagCount) {
		cout << "ERROR: Not all required flags have been set." << endl;
    cout << "Usage: ./sombrero -i init_filename.csv -s step_count -dt time_step" << endl;
		return 1;
	}
	
	if (sim.LoadBodiesFromFile(bodyFile) == false) {
		cout << "ERROR: Bodies count not be loaded from supplied filepath." << endl;
		return 1;
	}

	sim.SetTimestep(timestep);
	sim.Run(0, stepCount);

	return 0;
}

Simulation::Simulation() {
	// Constructor for Simulation object with default parameters
	bodyList = List();
}

Simulation::Simulation(int _width, int _height, double _dt) {
	// Constructor for Simulation object - key parameters defined
	bodyList = List();

	width = _width;
	height = _height;
	dt = _dt;
}

Simulation::Simulation(string _name, int _width, int _height, double _dt) {
	// Constructor for Simulation object - key parameters defined (except simulation name)
	bodyList = List();

	name = _name;
	width = _width;
	height = _height;
	dt = _dt;
}

bool Simulation::LoadBodiesFromFile(string _fileName) {
	// Load a simulation set-up from a body .csv file, into the bodyList
	regex validParameterLine("\\s*[0-9]*,(\\s*([+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?)\\s*,){2}\\s*[0-9]*\r?");
	regex validLine("(\\s*([+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?)\\s*,){7}\\s*[+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?,?([0-9a-zA-Z]*)?\\s*(//(\\s*\\S*)*)?\r?");
	regex validCommentLine("\\s*//(\\s*\\S*)*\r?");

	ifstream inputFile(_fileName);
	string fileLine, parameter;
	int i;

	// Return false if the file does not exist (raise error)
	if (!FileExists(_fileName)) {
		return false;
	}

	bool bodyLineReached = false;

	// Iterate through lines in the file
	while (getline(inputFile, fileLine))
	{
		if (regex_match(fileLine, validCommentLine)) {
			// Ignore line if it is a comment line
			continue;
		}
		else if (regex_match(fileLine, validParameterLine)) {
			// Parameters must be specified before any bodies
			if (bodyLineReached) {
				return false;
			}
			else {
				stringstream parameters(fileLine);
				string parameterArray [4];
				i = 0;

				while (getline(parameters, parameter, ','))
				{
					parameterArray[i] = parameter;
					i++;
				}


				// TODO: Set simulation parameters from entered parameters
				// int lastFrame = stoi(parameterArray[0]);
				// double timestep = stod(parameterArray[1]);

			}
		}
		else if (!regex_match(fileLine, validLine)) {
			// Return false if the line is not valid (raise error)
			return false;
		}
		else {
			bodyLineReached = true;
			stringstream bodyDetails(fileLine);
			string detailArray [9];
			i = 0;

			// Iterate through each comma separated value, and store each in the detailArray (for easy access later)
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
			string bodyName = detailArray[8];
			// string bodyName = "";

			bodyList.Append(new Body(x, y, z, mass, radius, xVelocity, yVelocity, zVelocity, bodyName));
			bodyCount += 1;
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

	// Save bodies to file
	SaveOutputFile(outputFolder + name + ".csv");
}

void Simulation::GenerateRandomDistribution(int _bodyCount) {
	srand(time(NULL));
	bodyList = List();

	for (int i = 0; i < _bodyCount; i++) {
		double x = Random(-3e11, 3e11);
		double y = Random(-3e11, 3e11);
		double z = Random(-3e11, 3e11);

		double mass = Random(1e26, 1e27);
		double radius = Random(1e6, 1e8);

		bodyList.Append(new Body(x, y, z, mass, radius, 0, 0, 0));
	}

	// Save bodies to file
	SaveOutputFile(outputFolder + name + ".csv");
}

void Simulation::GenerateRandomDistribution(int _bodyCount, double _width, double _bodyMassMin, double _bodyMassMax, double _radiusMin, double _radiusMax) {
	srand(time(NULL));
	bodyList = List();

	for (int i = 0; i < _bodyCount; i++) {
		double x = Random(-_width / 2.0, _width / 2.0);
		double y = Random(-_width / 2.0, _width / 2.0);
		double z = Random(-_width / 2.0, _width / 2.0);

		double mass = Random(_bodyMassMin, _bodyMassMax);
		double radius = Random(_radiusMin, _radiusMax);

		bodyList.Append(new Body(x, y, z, mass, radius, 0, 0, 0));
	}

	// Save bodies to file
	SaveOutputFile(outputFolder + name + ".csv");
}

void Simulation::Run(int startingFrame, int framesToSimulate) {
	// TODO: Enable starting from a different frame number
	// int currentFrames = 0;

	double elapsedTime = startingFrame * dt;

	// Simulate the next frames
	// <=, as it simulates 0->1, 1->2 therefore needs to simulate up to n.
	for (int f = startingFrame; f <= framesToSimulate + startingFrame; f++) {
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
				// Check for collision
				Vector bodyAPosition = bodyA->GetPosition();
				Vector bodyBPosition = bodyB->GetPosition();
				Vector bodyANextPosition = bodyA->GetNextPosition();
				Vector bodyBNextPosition = bodyB->GetNextPosition();

				Vector lambda = bodyBPosition.Subtract(bodyAPosition);
				Vector mu = bodyBNextPosition.Subtract(bodyANextPosition).Add(bodyAPosition).Subtract(bodyBPosition);

				// Quadratic coefficients => at^2 + bt + c = 0
				double a = pow(mu.Magnitude(), 2);
				double b = 2 * lambda.DotProduct(mu);
				double c = pow(lambda.Magnitude(), 2) - pow(bodyA->GetRadius() + bodyB->GetRadius(), 2);

				double t1 = ((-1 * b) + (sqrt(pow(b, 2) - (4 * a * c))))/(2 * a);
				double t2 = ((-1 * b) - (sqrt(pow(b, 2) - (4 * a * c))))/(2 * a);

				bool t1Valid = (t1 >= 0 && t1 <= 1);
				bool t2Valid = (t2 >= 0 && t2 <= 1);

				double separation = bodyBPosition.Subtract(bodyAPosition).Magnitude();
				double radiiSum = bodyA->GetRadius() + bodyB->GetRadius();
				
				// Select the first valid (0 <= t <= 1) collision time (smallest t)
				if (separation <= radiiSum) {
					// TODO: Determine if bodies should combine or bounce.
					//		 Currently, default: combine (true), (e = 1.0)
					HandleCollision(bodyA, bodyB, 0);

					// Update the body pointers 
					// 		(as both bodyA and bodyB have been deleted 
					//		from the list of bodies)
					bodyA = bodyList.GetHead();
					bodyB = bodyA->GetNext();
				}
				else if ((t1 <= t2 && t1Valid)) {
					// TODO: Determine if bodies should combine or bounce.
					//		 Currently, default: combine (true), (e = 1.0)
					HandleCollision(bodyA, bodyB, t1);

					// Update the body pointers
					bodyA = bodyList.GetHead();
					bodyB = bodyA->GetNext();
				}
				else if (t2Valid) {
					// TODO: Determine if bodies should combine or bounce.
					//		 Currently, default: combine (true), (e = 1.0)
					HandleCollision(bodyA, bodyB, t2);
					
					// Update the body pointers
					bodyA = bodyList.GetHead();
					bodyB = bodyA->GetNext();
				}
				else {
					bodyB = bodyB->GetNext();
				}
			}

			bodyA = bodyA->GetNext();
		}

		/*
		// Format elapsed time (for image output)
		string elapsedTimeString;
		string timeUnits = GetTimeUnits(elapsedTime);

		if (timeUnits == "YEARS") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime / (double)YR));
		}
		else if (timeUnits == "DAYS ") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime / (double)DAY));
		}
		else if (timeUnits == "HOURS") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime / (double)HOUR));
		}
		else if (timeUnits == "MINS ") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime / (double)MINUTE));
		}
		else if (timeUnits == "SECS ") {
			elapsedTimeString = RemoveTrailingZeroes(to_string(elapsedTime));
		}
		*/

		// Create _output.csv
		// TODO: This only creates an output file if the folder structure exists!
		string dataFileName = outputFolder + "data/bodyData_" + PadWithZeroes(f - startingFrame, framesToSimulate) + ".csv";
		SaveOutputFile(dataFileName, f, dt, f * dt, bodyCount);

		// Move each body to their new positions
		body = bodyList.GetHead();
		while (body != NULL) {
			body->Step();

			body = body->GetNext();
		}

		// Update the elapsed time
		elapsedTime += dt;
	}
}

void Simulation::SaveOutputFile(string _fileName, int _stepNumber, double _dt, double _timeElapsed, int _bodyCount) {
	// Save bodies to output.csv
	string outputFileName = outputFolder + name + "_output.csv";

	if (_fileName != "") {
		outputFileName = _fileName;
	}

	Output output(outputFileName);
	output.AddParameters(_stepNumber, _dt, _timeElapsed, _bodyCount);
	output.AddAllBodies(bodyList);
	output.Save();
}

void Simulation::AddBody(Body * body) {
	bodyList.Append(body);
}

void Simulation::ClearBodyList() {
	bodyList = List();
}

void Simulation::SetOutputDirectory(string _outputFolder) {
	outputFolder = _outputFolder;
	string command = "mkdir -p " + outputFolder;
	system(command.c_str());

	// Deal with final slash
	if (outputFolder[outputFolder.length() - 1] != '/') {
		outputFolder += '/';
	}
}

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
			else if (time >= HOUR) {
				timeUnits = "HOURS";
			}
			else if (time >= 2 * MINUTE) {
				timeUnits = "MINS ";
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

void Simulation::HandleCollision(Body * bodyA, Body * bodyB, double t, double e, bool combine) {
	// TODO: Improve this function to include a dynamic combine/collide 
	//		 functionality (depending on certain conditions).

	// Currently decides between combining (2 -> 1) or colliding 
	// ("bouncing" off one another) bodies, depending on arguments.
	
	if (combine == true) {
		CombineBodies(bodyA, bodyB, t);
	} else {
		CollideBodies(bodyA, bodyB, t, e);
	}
}

void Simulation::CombineBodies(Body * bodyA, Body * bodyB, double t) {
	// Calculate new Mass (sum of original bodies)
	double newMass = bodyA->GetMass() + bodyB->GetMass();

	// Conservation of linear momentum (assuming bodies will combine)
	Vector aMomentum = bodyA->GetVelocity().Multiply(bodyA->GetMass());
	Vector bMomentum = bodyB->GetVelocity().Multiply(bodyB->GetMass());
	Vector newVelocity = (aMomentum.Add(bMomentum)).Divide(newMass);

	// Calculate position of new body
	// Average the centres of the bodies at their collision positions
	bodyA->Update(t * dt);
	bodyB->Update(t * dt);

	Vector bodyAPosition = bodyA->GetNextPosition();
	Vector bodyBPosition = bodyB->GetNextPosition();

	Vector newPosition = (bodyAPosition.Add(bodyBPosition)).Divide(2.0);

	// Calculate new radius - use volumes of the two materials
	double volumeA = (4.0/3.0) * PI * pow(bodyA->GetRadius(), 3);
	double volumeB = (4.0/3.0) * PI * pow(bodyB->GetRadius(), 3);
	double newVolume = volumeA + volumeB;

	double newRadius = pow(newVolume / ((4.0/3.0) * PI), (1.0/3.0));

	// Create a new (combined) body, and remove A and B
	Body * newBody = new Body(newPosition, newMass, newRadius, newVelocity);
	newBody->Update((1 - t) * dt);

	bodyList.Append(newBody);
	
	bodyList.Remove(bodyA->GetID());
	bodyList.Remove(bodyB->GetID());

	// Update body count
	bodyCount -= 1;
}

void Simulation::CollideBodies(Body * bodyA, Body * bodyB, double t, double e) {
	// Move bodies to point of collision
	bodyA->Update(t * dt);
	bodyB->Update(t * dt);

	double mA = bodyA->GetMass();
	double mB = bodyB->GetMass();
	double mT = mA + mB;

	// Calculate direction vector connecting centers of the bodies
	Vector centerLineAB = bodyB->GetPosition().Subtract(bodyA->GetPosition());
	centerLineAB = centerLineAB.Divide(centerLineAB.Magnitude());

	// Calculate velocities along center line (problem is 1D)
	double uA = bodyA->GetVelocity().DotProduct(centerLineAB);
	double uB = bodyB->GetVelocity().DotProduct(centerLineAB);

	double totalMomentum = (mA * uA) + (mB * uB);
	double vA = (totalMomentum + (mB * e * (uB - uA))) / mT;
	double vB = (totalMomentum + (mA * e * (uA - uB))) / mT;
	
	bodyA->AddVelocity(centerLineAB.Multiply(vA - uA));
	bodyB->AddVelocity(centerLineAB.Multiply(vB - uB));

	// Move the bodies to the end of the time-step
	bodyA->Update((1 - t) * dt);
	bodyB->Update((1 - t) * dt);
}