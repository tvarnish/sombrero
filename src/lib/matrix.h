#ifndef __UNITS_H_INCLUDED__
#define __UNITS_H_INCLUDED__

#include <vector>

using namespace std;

class Matrix {
	vector<vector<double>> data;

	public:
		int rows, columns;

		void Initialise(int _rows, int _columns);
		void Display();
		double GetAt(int r, int c);
		void SetAt(int r, int c, double value);
		Matrix Rotate(double angle);
    Matrix Round();
};

#endif
