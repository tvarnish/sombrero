#include "video.h"

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
	int framerate = 10;

	string command = "/usr/local/bin/ffmpeg ";
	command += "-loglevel panic ";
	command += "-r " + to_string(framerate) + " ";
	command += "-s " + width + "x" + height + " ";
	command += "-i " + imageFolder + imagePrefix + "%02d.ppm "; // TODO :: Add %03d thingy.
	command += "-vcodec libx264 -crf 25 -pix_fmt yuv420p ";
	command += "-y ";
	command += outputFileName;

	system( command.c_str() );
}

void Video::Open() {
	system( ("open " + outputFileName).c_str() );
}
