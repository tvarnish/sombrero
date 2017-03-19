#include "video.h"
#include "misc.h"

#include <iostream>

using namespace std;

Video::Video(string _imageFolder, string _imagePrefix, int _width, int _height, int _framerate) {
	imageFolder = _imageFolder;
	imagePrefix = _imagePrefix;

	width = to_string(_width);
	height = to_string(_height);

	framerate = to_string(_framerate);
}

void Video::ClearImageFolder() {
	system( ("rm " + imageFolder + "*.png").c_str() );
}

void Video::Build(string _outputFileName, int frameCount) {
	outputFileName = _outputFileName;

	/*
	//string command = "/usr/local/bin/ffmpeg ";
	string command = "avconv ";
	//command += "-loglevel panic ";
	command += "-loglevel error ";
	command += "-r " + to_string(framerate) + " ";
	command += "-s " + width + "x" + height + " ";
	command += "-i " + imageFolder + imagePrefix + "%0" + to_string(NumberLength(frameCount - 1)) + "d.png ";
	command += "-vcodec libx264 ";
	command += "-y ";
	command += outputFileName;
	*/

	string command = "avconv -r " + framerate + " ";
	command += "-i " + imageFolder + imagePrefix + "%0" + to_string(NumberLength(frameCount - 1)) + "d.png ";
	command += "-s " + width + "x" + height + " ";
	//command += "-vcodec qtrle ";
	command += "-y -loglevel error ";
	command += outputFileName;

	cout << command << endl;

	system(command.c_str());
}

void Video::Open() {
	system( ("open " + outputFileName).c_str() );
}
