#include "image.h"
#include "body.h"
#include "font.h"

#include <fstream>

using namespace std;

Image::Image(string filename, int w, int h) {
	fileName = filename;
	width = w;
	height = h;

	// Initialize image (set all pixels to black (0,0,0))
	for (int y = 0; y < height; y++)
	{
		vector<Pixel> row(width);
		pixels.push_back(row);

		Pixel p;
		p.R = 0;
		p.G = 0;
		p.B = 0;

		for (int x = 0; x < width; x++)
		{
			pixels[y].push_back(p);
		}
	}
}

void Image::DrawBody(Body * body, int r, int g, int b) {
	pixels[body->GetY()][body->GetX()].SetColour(r, g, b);
}

void Image::DrawAllBodies(int bodyCount, Body * bodyArray [], int r, int g, int b) {
	for (int i = 0; i < bodyCount; i++)
	{
		DrawBody(bodyArray[i], r, g, b);
	}
}

void Image::DrawTextArray(int textArray [5][5], int xStart, int yStart, int r, int g, int b) {
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			if (textArray[y][x] == 0)
			{
				pixels[y + yStart][x + xStart].SetColour(0, 0, 0);
			}
			else
			{
				pixels[y + yStart][x + xStart].SetColour(r, g, b);
			}
		}
	}
}

void Image::DrawText(string text, int x, int y, int r, int g, int b) {
	for (int i = 0; i < text.length(); i++)
	{
		// Handle Alphabet
		if (tolower(text[i]) >= 97 && tolower(text[i]) <= 122)
		{
			int index = tolower(text[i]) - 97;
			DrawTextArray(fontAlphabet[index], x, y, r, g, b);
		}

		// Handle Numbers
		else if (tolower(text[i]) >= 48 && tolower(text[i]) <= 50)
		{
			int index = tolower(text[i]) - 48;
			DrawTextArray(fontNumbers[index], x, y, r, g, b);
		}

		// Handle Punctuation
		else
		{
			switch (text[i])
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

			default:
				break;
			}
		}

		x += fontWidth + kerning;
	}
}

void Image::Save() {
	ofstream imageFile(fileName, ios::out);
	imageFile << "P3\n";
	imageFile << to_string(width) << " " << to_string(height) << "\n";
	imageFile << "255\n";

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			imageFile << pixels[y][x].R << " ";
			imageFile << pixels[y][x].G << " ";
			imageFile << pixels[y][x].B << "\n";
		}
	}

	imageFile.close();
	CleanUp();
}

void Image::CleanUp() {
	pixels.clear();
	pixels.shrink_to_fit();
}
