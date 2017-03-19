// video.cpp
// Video object, used for wrangling image folder and video compilation

#include "video.h"
#include "misc.h"

#include <iostream>

using namespace std;

Video::Video(string _imageFolder, string _imagePrefix, int _width, int _height, int _framerate) {
	// Constructor - set the image folder and prefix, etc.
	imageFolder = _imageFolder;
	imagePrefix = _imagePrefix;

	width = to_string(_width);
	height = to_string(_height);

	framerate = to_string(_framerate);
}

void Video::ClearImageFolder() {
	// Clear the image folder
	system( ("rm " + imageFolder + "*.png").c_str() );
}

void Video::Build(string _outputFileName, int frameCount) {
	// Build the video from the images using avconv
	outputFileName = _outputFileName;

	// Form the avconv command
	string command = "avconv -r " + framerate + " ";
	command += "-i " + imageFolder + imagePrefix + "%0" + to_string(GetLengthOfNumber(frameCount - 1)) + "d.png ";
	command += "-s " + width + "x" + height + " ";
	command += "-y -loglevel error ";
	command += outputFileName;

	cout << command << endl;

	system(command.c_str());
}
