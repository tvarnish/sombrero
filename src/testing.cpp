#include <iostream>
#include "sombrero.h"

using namespace std;

int main() {
	Simulation sim = Simulation();

	sim.SetSimulationName("HelloWorld");
	sim.SetTimestep(YR / 365);
	sim.SetFramerate(60);
	sim.SetScale(AU, 100);

	if (sim.LoadBodiesFromFile("init/realsolarsystem.csv")) {
		sim.Run(0, 365);
	}
	else {
		cout << "Error in reading body file." << endl;
	}

	return 0;
}