#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

#include <vector>

using namespace std;

class Matrix {
  protected:
	   vector<vector<double>> data;

	public:
		int rows, columns;

		void Initialise(int _rows, int _columns);
		void Display();
		double GetAt(int r, int c);
		void SetAt(int r, int c, double value);
};

class Vector {
	double x;
	double y;
	double z;

  public:
	void Set(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
	void SetX(double _x) { x = _x; };
	void SetY(double _y) { y = _y; };
	void SetZ(double _z) { z = _z; };

	double GetX() { return x; };
	double GetY() { return y; };
	double GetZ() { return z; };

	Vector Add(Vector other);
	Vector Subtract(Vector other);
	Vector Multiply(double scalar);
	Vector Divide(double scalar);
	double DotProduct(Vector other);
	double Magnitude();

	Vector Transform(Matrix transformationMatrix);
	Vector RotateY(double angle);
	Vector RoundValues();
};

#endif
