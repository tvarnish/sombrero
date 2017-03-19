// position_vector.cpp
// Vector object for velocity, position, force etc. of the body object

#include <iostream>
#include <cmath>

#include "misc.h"
#include "matrix.h"
#include "position_vector.h"

using namespace std;

Vector::Vector() {
	// Constructor - initialise values as zero
	Set(0.0, 0.0, 0.0);
}

Vector::Vector(double _x, double _y, double _z) {
	// Constructor - initialise vector with values (x,y,z)
	Set(_x, _y, _z);
}

Vector Vector::Add(Vector other) {
	// Add two vectors together -> self + other
	Vector result = Vector(x + other.GetX(), y + other.GetY(), z + other.GetZ());
	return result;
}

Vector Vector::Subtract(Vector other) {
	// Subtract other from self
	Vector result = Vector(x - other.GetX(), y - other.GetY(), z - other.GetZ());
	return result;
}

Vector Vector::Multiply(double scalar) {
	// Multiply self by a scalar value
	Vector result = Vector(x * scalar, y * scalar, z * scalar);
	return result;
}

Vector Vector::Divide(double scalar) {
	// Divide self by a scalar value
	Vector result = Vector(x / scalar, y / scalar, z / scalar);
	return result;
}

double Vector::DotProduct(Vector other) {
	// Calculate the dot product of two vectors (self and other)
	double result = (x * other.GetX()) + (y * other.GetY()) + (z * other.GetZ());
	return result;
}

double Vector::Magnitude() {
	// Calculate the magnitude of the vector
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector Vector::Transform(Matrix transformationMatrix) {
	// Transform the vector with transformationMatrix
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
	// Rotate the vector about the y-axis by angle (using rotation matrix)
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
	// Round the vector values (for scaling the coordinates to draw on the image)
	Vector rounded;

	rounded.SetX(round(x));
	rounded.SetY(round(y));
	rounded.SetZ(round(z));

	if (rounded.GetX() == -0.0) rounded.SetX(0.0);
	if (rounded.GetY() == -0.0) rounded.SetY(0.0);
	if (rounded.GetZ() == -0.0) rounded.SetZ(0.0);

	return rounded;
}