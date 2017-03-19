// matrix.cpp
// Matrix object used to describe and perform transformations

#include <iostream>
#include <cmath>

#include "matrix.h"
#include "units.h"

using namespace std;

Matrix::Matrix(int _rows, int _columns) {
	// Constructor - set the size of the matrix
	rows = _rows;
	columns = _columns;

	data.assign(rows, vector<double>(columns, 0));
}

void Matrix::Display() {
	// Used for debugging - display the contents of a Matrix (console)
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < columns; y++) {
			cout << data.at(x).at(y) << " ";
		}
		cout << endl;
	}
}

double Matrix::GetAt(int r, int c) {
	// Return the value stored in the matrix at the specified position
	return data.at(r).at(c);
}

void Matrix::SetAt(int r, int c, double value) {
	// Set the value stored in the matrix at the specified position
	data.at(r).at(c) = value;
}
