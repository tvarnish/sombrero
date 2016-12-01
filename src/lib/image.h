// include guard
#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <vector>
#include <iostream>
#include <pngwriter.h>

class Body;

class Image {
		int width;
		int height;
		double scale;
		std::string fileName;

		pngwriter png;

		void DrawTextArray(int textArray [5][5], int xStart, int yStart, int r, int g, int b);

	public:
		Image(std::string filename, int w, int h, double _scale);
		int Scale(double coordinate, double scale);
		void Draw(int x, int y, int r, int g, int b);
		void DrawBody(double x, double y, int r, int g, int b);
		void DrawAllBodies(int bodyCount, Body * bodyArray [], int r, int g, int b);
		void DrawText(std::string text, int x, int y, int r, int g, int b);
		void Save();
};

#endif
