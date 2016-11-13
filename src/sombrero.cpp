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

using namespace std;

int main(int argc, char * argv[]) {
	// position example:
	// (a,b) -> m.GetAt(a,b)
	// (0,0) (0,1)
	// (1,0) (1,1)

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

	return 0;
}
