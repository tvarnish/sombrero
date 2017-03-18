#include <iostream>

#include "sombrero.h"

using namespace std;

int main() {
	Simulation sim = Simulation();

	sim.SetSimulationName("HelloWorld");
	sim.SetTimestep(DAY);
	sim.SetFramerate(60);
	sim.SetScale(AU, 100);

	if (sim.LoadBodiesFromFile("_realsolarsystem.csv")) {
		sim.Run(0, 100);
	}
	else {
		cout << "Error in reading body file." << endl;
	}

	return 0;
}