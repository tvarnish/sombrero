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

Vector Vector::Add(Vector other) {
	Vector result;
	result.Set(this->x + other.x, this->y + other.y, this->z + other.z);
	return result;
}

Vector Vector::Subtract(Vector other) {
	Vector result;
	result.Set(this->x - other.x, this->y - other.y, this->z - other.z);
	return result;
}

Vector Vector::Multiply(double scalar) {
	Vector result;
	result.Set(this->x * scalar, this->y * scalar, this->z * scalar);
	return result;
}

Vector Vector::Divide(double scalar) {
	Vector result;
	result.Set(this->x / scalar, this->y / scalar, this->z / scalar);
	return result;
}

double Vector::DotProduct(Vector other) {
	double result;
	result = (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
	return result;
}

double Vector::Magnitude() {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector Vector::Transform(Matrix transformationMatrix) {
	Vector transformed;

	// X
	transformed.SetX(x * transformationMatrix.GetAt(0, 0) +
					 y * transformationMatrix.GetAt(0, 1) +
					 z * transformationMatrix.GetAt(0, 2));
	// Y
	transformed.SetY(x * transformationMatrix.GetAt(1, 0) +
					 y * transformationMatrix.GetAt(1, 1) +
					 z * transformationMatrix.GetAt(1, 2));
	// Z
	transformed.SetZ(x * transformationMatrix.GetAt(2, 0) +
					 y * transformationMatrix.GetAt(2, 1) +
					 z * transformationMatrix.GetAt(2, 2));

	return transformed;
}

Vector Vector::RotateY(double angle) {
	Matrix rotateMatrix;
	rotateMatrix.Initialise(3, 3);

	// TODO: Create MISC function for converting to radians
	double angleRadians = angle * (3.1415926 / 180.0);

	rotateMatrix.SetAt(0, 0, cos(angleRadians));
	rotateMatrix.SetAt(0, 2, sin(angleRadians));
	rotateMatrix.SetAt(1, 1, 1.0);
	rotateMatrix.SetAt(2, 0, -1.0 * sin(angleRadians));
	rotateMatrix.SetAt(2, 2, cos(angleRadians));

	Vector transformed;
	transformed = this->Transform(rotateMatrix);

	return transformed;
}

Vector Vector::RoundValues() {
	Vector rounded;

	rounded.SetX(round(this->x));
	rounded.SetY(round(this->y));
	rounded.SetZ(round(this->z));

	if (rounded.GetX() == -0.0) rounded.SetX(0.0);
	if (rounded.GetY() == -0.0) rounded.SetY(0.0);
	if (rounded.GetZ() == -0.0) rounded.SetZ(0.0);

	return rounded;
}
