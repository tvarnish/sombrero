#include <iostream>
#include <cmath>

#include "matrix.h"
#include "units.h"

using namespace std;

void Matrix::Initialise(int _rows, int _columns) {
	rows = _rows;
	columns = _columns;

	this->data.assign(rows, vector<double>(columns, 0));
}

void Matrix::Display() {
	for (int x = 0; x < this->rows; x++) {
		for (int y = 0; y < this->columns; y++) {
			cout << this->data.at(x).at(y) << " ";
		}
		cout << endl;
	}
}

double Matrix::GetAt(int r, int c) {
	return this->data.at(r).at(c);
}

void Matrix::SetAt(int r, int c, double value) {
	this->data.at(r).at(c) = value;
}

void PositionMatrix::Initialise() {
  this->rows = 3;
  this->columns = 1;
	this->data.assign(3, vector<double>(1, 0.0));
}

void PositionMatrix::Set(double x, double y, double z) {
  this->SetAt(0, 0, x);
  this->SetAt(1, 0, y);
  this->SetAt(2, 0, z);
}

double PositionMatrix::GetX() {
  return this->GetAt(0, 0);
}

double PositionMatrix::GetY() {
  return this->GetAt(1, 0);
}

double PositionMatrix::GetZ() {
  return this->GetAt(2, 0);
}

PositionMatrix PositionMatrix::RotateY(double angle) {
	Matrix rotateMatrix;
	rotateMatrix.Initialise(3, 3);

	// TODO: Create MISC function for converting to radians
	double angleRadians = angle * (3.1415926 / 180.0);

	rotateMatrix.SetAt(0, 0, cos(angleRadians));
	rotateMatrix.SetAt(0, 2, sin(angleRadians));
	rotateMatrix.SetAt(1, 1, 1.0);
	rotateMatrix.SetAt(2, 0, -1.0 * sin(angleRadians));
	rotateMatrix.SetAt(2, 2, cos(angleRadians));

	PositionMatrix newMatrix;
	newMatrix.Initialise();

	double value;
  for (int i = 0; i < 3; i ++) {
    value = 0.0;
    for (int j = 0; j < 3; j++) {
      value += rotateMatrix.GetAt(i, j) * this->GetAt(j, 0);
    }
    newMatrix.SetAt(i, 0, value);
  }

	return newMatrix;
}

PositionMatrix PositionMatrix::Round() {
  PositionMatrix roundedMatrix;
  roundedMatrix.Initialise();

  double roundedValue;
  for (int i = 0; i < 3; i++) {
    roundedValue = round(this->GetAt(i, 0));
    if (roundedValue == -0.0) {
      roundedValue = 0.0;
    }
    roundedMatrix.SetAt(i, 0, roundedValue);
  }

  return roundedMatrix;
}
