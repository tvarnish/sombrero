#include <iostream>
#include <cmath>

#include "misc.h"
#include "matrix.h"
#include "position_vector.h"

using namespace std;

Vector::Vector() {
	Set(0.0, 0.0, 0.0);
}

Vector::Vector(double _x, double _y, double _z) {
	Set(_x, _y, _z);
}

Vector Vector::Add(Vector other) {
	Vector result = Vector(x + other.GetX(), y + other.GetY(), z + other.GetZ());
	return result;
}

Vector Vector::Subtract(Vector other) {
	Vector result = Vector(x - other.GetX(), y - other.GetY(), z - other.GetZ());
	return result;
}

Vector Vector::Multiply(double scalar) {
	Vector result = Vector(x * scalar, y * scalar, z * scalar);
	return result;
}

Vector Vector::Divide(double scalar) {
	Vector result = Vector(x / scalar, y / scalar, z / scalar);
	return result;
}

double Vector::DotProduct(Vector other) {
	double result = (x * other.GetX()) + (y * other.GetY()) + (z * other.GetZ());
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
	Matrix rotateMatrix = Matrix(3, 3);

	double angleRadians = ToRadians(angle);

	rotateMatrix.SetAt(0, 0, cos(angleRadians));
	rotateMatrix.SetAt(0, 2, sin(angleRadians));
	rotateMatrix.SetAt(1, 1, 1.0);
	rotateMatrix.SetAt(2, 0, -1.0 * sin(angleRadians));
	rotateMatrix.SetAt(2, 2, cos(angleRadians));

	Vector transformed = Transform(rotateMatrix);

	return transformed;
}

Vector Vector::RoundValues() {
	Vector rounded;

	rounded.SetX(round(x));
	rounded.SetY(round(y));
	rounded.SetZ(round(z));

	if (rounded.GetX() == -0.0) rounded.SetX(0.0);
	if (rounded.GetY() == -0.0) rounded.SetY(0.0);
	if (rounded.GetZ() == -0.0) rounded.SetZ(0.0);

	return rounded;
}