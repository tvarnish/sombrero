#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

#include <vector>

using namespace std;

class Matrix {
	int rows, columns;
	
  	protected:
	   	vector<vector<double>> data;

	public:
		Matrix(int _rows, int _columns);

		void Display();
		double GetAt(int r, int c);
		void SetAt(int r, int c, double value);
};

#endif
