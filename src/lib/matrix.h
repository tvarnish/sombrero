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

class PositionMatrix : public Matrix {
  public:
    void Initialise();
    void Set(double x, double y, double z);
    double GetX();
    double GetY();
    double GetZ();
    PositionMatrix RotateY(double angle);
    PositionMatrix Round();
};

#endif
