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

#include "lib/units.h"

using namespace std;

class Matrix {
	public:
		int rows, columns;
		vector<vector<double>> data;

		void Initialise(int rows, int columns);
		void Display();
};

void Matrix::Initialise(int _rows, int _columns) {
	rows = _rows;
	columns = _columns;
	
	data.assign(rows, vector<double>(columns, 0));
}

void Matrix::Display() {
	for (int x = 0; x < this->rows; x++) {
		for (int y = 0; y < this->columns; y++) {
			cout << this->data.at(x).at(y) << " ";
		}
		cout << endl;
	}
}

int main(int argc, char * argv[]) {
	int i = 2;
	int j = 2;
	
	Matrix m;
	m.Initialise(i, j);

	// position example:
	// (a,b) -> m.at(a).at(b)
	// (0,0) (0,1)
	// (1,0) (1,1)

	m.data.at(0).at(0) = 2;
	m.data.at(1).at(0) = 4;

	m.Display();

	/*
	Matrix m1;
	m1.Initialise(3, 1);
	m1.data.at(0).at(0) = 2.0;
	m1.data.at(2).at(0) = 1.0;
	m1.Display();

	cout << endl;

	Matrix m2;
	m2.Initialise(1, 3);
	m1.data.at(0).at(1) = 5.0;
	m2.Display();
	*/
	return 0;
}
