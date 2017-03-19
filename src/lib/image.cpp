// image.cpp
// Image class - single image that body positions etc. are drawn onto

#include "image.h"
#include "body.h"
#include "font.h"
#include "units.h"
#include "linkedlist.h"
#include "misc.h"

#include <fstream>
#include <pngwriter.h>

using namespace std;

Image::Image(string _fileName, int _width, int _height, double _scale) {
	// Image constructor
	fileName = _fileName;
	width = _width;
	height = _height;
	scale = _scale;

	// Creat pngwriter object (used for image data)
	png = pngwriter(width, height, 0, fileName.c_str());
}

void Image::Draw(int x, int y, int r, int g, int b) {
	// Set the color (r,g,b) of a specific pixel (x,y)
	// pngwriter library requires R,G,B values to be in the format 0.0 -> 1.0 (not 0 -> 255)
	double redValue = ((double)r / 255.0);
	double greenValue = ((double)g / 255.0);
	double blueValue = ((double)b / 255.0);

	png.plot(x, y, redValue, greenValue, blueValue);
}

int Image::Scale(double position, double scale) {
	// Scale the real-world position to a location on the image
	int coordinate = (int)(position / scale);
	return coordinate;
}

void Image::DrawBody(double x, double y, double radius, int r, int g, int b) {
	// Draw a body on the image plane (scaled from the real-world coordinates), with the correct radius
	int xScaled = (width / 2) + Scale(x, scale);
 	int yScaled = (height / 2) + Scale(y, scale);
 	int radiusScaled = Scale(radius, scale);

 	// Check that the specified coordinate (once scaled) will be visible on the screen
 	// It is not required to draw a body that is off-screen
	bool xValid = xScaled < width && xScaled >= 0;
	bool yValid = yScaled < height && yScaled >= 0;

	if (xValid && yValid) {
		// Convert the color
		double redValue = ((double)r / 255.0);
		double greenValue = ((double)g / 255.0);
		double blueValue = ((double)b / 255.0);

		// The smallest size that a body can be on the image is 1 pixel
		if (radiusScaled == 0) {
			Draw(xScaled, yScaled, 255, 255, 255); // White pixels show up the best on the screen
		}
		else {
			png.filledcircle(xScaled, yScaled, radiusScaled, redValue, greenValue, blueValue);
		}
	}
}

void Image::DrawAllBodies(List bodyList, int r, int g, int b) {
	// Iterate through all of the items in bodyList, and draw them on the image
	Body * body = bodyList.GetHead();
	while (body != NULL) {
		DrawBody(body->GetX(), body->GetY(), body->GetRadius(), r, g, b);
		body = body->GetNext();
	}
}

void Image::DrawTextArray(int textArray [5][5], int xStart, int yStart, int r, int g, int b) {
	// Draw the bitmap of a character onto the image
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			if (textArray[y][x] == 0)
			{
				// Draw a black pixel instead of just ignoring it, so that it covers up anything below it
				png.plot(x + xStart, height - (y + yStart), 0.0, 0.0, 0.0);
			}
			else
			{
				// Convert the RGB color
				double redValue = (double)r / 255.0;
				double greenValue = (double)g / 255.0;
				double blueValue = (double)b / 255.0;

				png.plot(x + xStart, height - (y + yStart), redValue, greenValue, blueValue);
			}
		}
	}
}

void Image::DrawText(string text, int x, int y, int r, int g, int b) {
	// Draw text onto the image - lookup the character bit map, then draw that
	// size_t i instead of int i, as we are comparing i to text.length() (C++ rules)
	for (size_t i = 0; i < text.length(); i++)
	{
		int character = (int)i;

		// Handle Alphabet
		if (tolower(text[character]) >= 97 && tolower(text[character]) <= 122)
		{
			int index = tolower(text[character]) - 97;
			DrawTextArray(fontAlphabet[index], x, y, r, g, b);
		}

		// Handle Numbers
		else if (tolower(text[character]) >= 48 && tolower(text[character]) <= 57)
		{
			int index = tolower(text[character]) - 48;
			DrawTextArray(fontNumbers[index], x, y, r, g, b);
		}

		// Handle Punctuation
		else
		{
			switch (text[character])
			{
			case '.':
				DrawTextArray(fontPERIOD, x, y, r, g, b);
				break;
			case ':':
				DrawTextArray(fontCOLON, x, y, r, g, b);
				break;
			case '-':
				DrawTextArray(fontHYPHEN, x, y, r, g, b);
				break;
			case '/':
				DrawTextArray(fontSLASH, x, y, r, g, b);
				break;

			default:
				break;
			}
		}

		x += fontWidth + kerning;
	}
}

void Image::DrawScale(double scale, int x, int y, int r, int g, int b) {
	// Draw the scale of the simulation onto the image
	DrawText(ToStandardForm(scale) + "m / px", x, y - 2, r, g, b);
}

void Image::Save() {
	// Save the image to disk (with pngwriter)
	png.close();
}
