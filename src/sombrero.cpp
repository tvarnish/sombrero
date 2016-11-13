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
	int i = 3;
	int j = 1;

	Matrix m;
	m.Initialise(i, j);

	// position example:
	// (a,b) -> m.at(a).at(b)
	// (0,0) (0,1)
	// (1,0) (1,1)

	m.SetAt(0, 0, 0.0);
	m.SetAt(1, 0, 4.0);
	m.SetAt(2, 0, -2.0);

	m.Display();

	Matrix t;
	t.Initialise(i, j);
	cout << endl;

	for (int i = 0; i < 90.0; i += 5) {
		cout << "i = " << i << endl;
		t = m.Rotate(i);
		/*
		t.SetAt(0, 0, round(t.GetAt(0, 0)));
		t.SetAt(1, 0, round(t.GetAt(1, 0)));
		t.SetAt(2, 0, round(t.GetAt(2, 0)));
		*/
		t = t.Round();
		t.Display();
		cout << endl;
	}

	return 0;
}
