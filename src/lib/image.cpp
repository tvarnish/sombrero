#include "image.h"
#include "body.h"
#include "font.h"
#include "units.h"
#include "linkedlist.h"

#include <fstream>
#include <pngwriter.h>

using namespace std;

//pngwriter png;

Image::Image(string filename, int w, int h, double _scale) {
	fileName = filename;
	width = w;
	height = h;
	scale = _scale;

	png = pngwriter(width, height, 0, filename.c_str());
}

void Image::Draw(int x, int y, int r, int g, int b) {
	double redValue = ((double)r / 255.0);
	double greenValue = ((double)g / 255.0);
	double blueValue = ((double)b / 255.0);

	png.plot(x, y, redValue, greenValue, blueValue);
}

void Image::DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b) {
	double redValue = ((double)r / 255.0);
	double greenValue = ((double)g / 255.0);
	double blueValue = ((double)b / 255.0);

	png.line(x1, height - y1, x2, height - y2, redValue, greenValue, blueValue);
}

void Image::DrawBody(double x, double y, int r, int g, int b) {
	int xScaled = (width / 2) + Scale(x, scale);
 	int yScaled = (height / 2) + Scale(y, scale);

	bool xValid = xScaled < width && xScaled >= 0;
	bool yValid = yScaled < height && yScaled >= 0;

	if (xValid && yValid) {
		Draw(xScaled, yScaled, r, g, b);
	}
}

int Image::Scale(double coordinate, double scale) {
	int position = (int)(coordinate / AU * scale);
	return position;
}

void Image::DrawAllBodies(int bodyCount, List bodyList, int r, int g, int b) {
	Body * body = bodyList.GetHead();
	while (body != NULL) {
		DrawBody(body->GetX(), body->GetY(), r, g, b);
		body = body->next;
	}
}

void Image::DrawTextArray(int textArray [5][5], int xStart, int yStart, int r, int g, int b) {
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			if (textArray[y][x] == 0)
			{
				png.plot(x + xStart, height - (y + yStart), 0.0, 0.0, 0.0);
			}
			else
			{
				png.plot(x + xStart, height - (y + yStart), ((double)r / 255.0), ((double)g / 255.0), ((double)b / 255.0));
			}
		}
	}
}

void Image::DrawText(string text, int x, int y, int r, int g, int b) {
	for (size_t i = 0; i < text.length(); i++)
	{
		int c = (int)i;

		// Handle Alphabet
		if (tolower(text[c]) >= 97 && tolower(text[c]) <= 122)
		{
			int index = tolower(text[c]) - 97;
			DrawTextArray(fontAlphabet[index], x, y, r, g, b);
		}

		// Handle Numbers
		else if (tolower(text[c]) >= 48 && tolower(text[c]) <= 57)
		{
			int index = tolower(text[c]) - 48;
			DrawTextArray(fontNumbers[index], x, y, r, g, b);
		}

		// Handle Punctuation
		else
		{
			switch (text[c])
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
	if (scale >= 3.0) {
		// Long enough to draw a line
		DrawLine(x, y, x + scale, y, r, g, b);
		Draw(x, height - y + 1, r, g, b);
		Draw(x + scale, height - y + 1, r, g, b);
		DrawText("1 AU", x + scale + 5, y - 2, r, g, b);
	}
	else {
		// Not long enough - just write a message
		string scaleText = "SCALE: " + to_string(scale) + " PX/AU";
		DrawText(scaleText.c_str(), x, y - 2, r, g, b);
	}
}

void Image::Save() {
	png.close();
}
