#include "video.h"
#include "misc.h"

#include <iostream>

using namespace std;

Video::Video(string imgFolder, string imgPrefix, int w, int h, int frate) {
	imageFolder = imgFolder;
	imagePrefix = imgPrefix;

	width = to_string(w);
	height = to_string(h);

	framerate = to_string(frate);
}

void Video::ClearImageFolder() {
	system( ("rm " + imageFolder + "*.png").c_str() );
}

void Video::Build(string outputFName, int frameCount) {
	outputFileName = outputFName;

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
