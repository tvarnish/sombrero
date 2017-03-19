#include <iostream>
#include "sombrero.h"

using namespace std;

int main() {
	// Create simulation object
	Simulation sim = Simulation();

	// Set simulation parameters
	sim.SetSimulationName("HelloWorld");
	sim.SetTimestep(YR / 365);
	sim.SetFramerate(60);
	sim.SetScale(AU, 100);

	// Simulate setup
	if (sim.LoadBodiesFromFile("init/realsolarsystem.csv")) {
		sim.Run(0, 365, "Building Video...");
	}
	else {
		cout << "Error in reading body file." << endl;
	}

	return 0;
}