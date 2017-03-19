#include <iostream>
#include <cmath>

#include "matrix.h"
#include "units.h"

using namespace std;

Matrix::Matrix(int _rows, int _columns) {
	rows = _rows;
	columns = _columns;

	data.assign(rows, vector<double>(columns, 0));
}

void Matrix::Display() {
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < columns; y++) {
			cout << data.at(x).at(y) << " ";
		}
		cout << endl;
	}
}

double Matrix::GetAt(int r, int c) {
	return data.at(r).at(c);
}

void Matrix::SetAt(int r, int c, double value) {
	data.at(r).at(c) = value;
}
