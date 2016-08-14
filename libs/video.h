#ifndef __VIDEO_H_INCLUDED__
#define __VIDEO_H_INCLUDED__

#include <iostream>

class Video {
		std::string outputFileName;
		std::string imageFolder;
		std::string imagePrefix;
		std::string width;
		std::string height;

	public:
		Video(std::string imgFolder, std::string imgPrefix, int w, int h);
		void ClearImageFolder();
		void Build(std::string outputFName, int frameCount);
		void Open();
};

#endif
