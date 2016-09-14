#include "video.h"
#include "misc.h"

#include <iostream>

using namespace std;

Video::Video(string imgFolder, string imgPrefix, int w, int h) {
	imageFolder = imgFolder;
	imagePrefix = imgPrefix;

	width = to_string(w);
	height = to_string(h);
}

void Video::ClearImageFolder() {
	system( ("rm " + imageFolder + "*.ppm").c_str() );
}

void Video::Build(string outputFName, int frameCount) {
	outputFileName = outputFName;
	int framerate = 60;

	//string command = "/usr/local/bin/ffmpeg ";
	string command = "avconv ";
	//command += "-loglevel panic ";
	command += "-r " + to_string(framerate) + " ";
	command += "-s " + width + "x" + height + " ";
	command += "-i " + imageFolder + imagePrefix + "%0" + to_string(NumberLength(frameCount) - 1) + "d.ppm ";
	command += "-vcodec libx264 -crf 25 -pix_fmt yuv420p ";
	command += "-y ";
	command += outputFileName;

	system( command.c_str() );
}

void Video::Open() {
	system( ("open " + outputFileName).c_str() );
}
