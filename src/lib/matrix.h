#ifndef __UNITS_H_INCLUDED__
#define __UNITS_H_INCLUDED__

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
	void Set(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
	void SetX(double x) { this->x = x; };
	void SetY(double y) { this->y = y; };
	void SetZ(double z) { this->z = z; };

	double GetX() { return this->x; };
	double GetY() { return this->y; };
	double GetZ() { return this->z; };

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
