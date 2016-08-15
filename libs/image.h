// include guard
#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <vector>
#include <iostream>

class Body;

class Pixel {
	public:
		int R;
		int G;
		int B;

		void SetColour(int r, int g, int b) { R = r; G = g; B = b; };
};

class Image {
		int width;
		int height;
		std::string fileName;

		void DrawTextArray(int textArray [5][5], int xStart, int yStart, int r, int g, int b);
		int Scale(double coordinate, std::string positionUnits, double scale);

	public:
		std::vector< std::vector<Pixel> > pixels;

		Image(std::string filename, int w, int h);
		void DrawBody(int x, int y, int r, int g, int b);
		void DrawAllBodies(int bodyCount, Body * bodyArray [], int r, int g, int b, std::string positionUnits, double scale);
		void DrawText(std::string text, int x, int y, int r, int g, int b);
		void Save();
		void CleanUp();
};

#endif
