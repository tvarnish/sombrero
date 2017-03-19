// video.h
// Video object, used for wrangling image folder and video compilation

#ifndef __VIDEO_H_INCLUDED__
#define __VIDEO_H_INCLUDED__

#include <iostream>

using namespace std;

class Video {
	string outputFileName;
	string imageFolder;
	string imagePrefix;
	string width;
	string height;
	string framerate;

	public:
		Video(string _imageFolder, string _imagePrefix, int _width, int _height, int _framerate);

		void ClearImageFolder();
		void Build(string _outputFileName, int frameCount, string buildingMessage = "");
};

#endif
