// include guard
#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <vector>
#include <iostream>
#include <pngwriter.h>

#include "linkedlist.h"

class Body;

class Image {
		int width;
		int height;
		double scale;
		std::string fileName;

		pngwriter png;

		void DrawTextArray(int textArray [5][5], int xStart, int yStart, int r, int g, int b);

	public:
		Image(std::string _fileName, int _width, int _height, double _scale);
		int Scale(double position, double scale);
		void Draw(int x, int y, int r, int g, int b);
		void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);
		void DrawBody(double x, double y, double radius, int r, int g, int b);
		void DrawAllBodies(List bodyList, int r, int g, int b);
		void DrawText(std::string text, int x, int y, int r, int g, int b);
		void DrawScale(double scale, int x, int y, int  r, int g, int b);
		void Save();
};

#endif
