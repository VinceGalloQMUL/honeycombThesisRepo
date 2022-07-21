#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "depthDisplay.h"

using namespace cv;
using namespace std;
using namespace System;

namespace FormDepthOpenCV {
	// 2%
	
	int marginFraction = 50;
	int pixelsW;
	int pixelsH;
	bool isOpen = false;
	float detectionRange;
	depthDisplay *pDepthDisplay;

	int getWidth() {
		return pixelsW;
	}
	int getHeight() {
		return pixelsH;
	}

	bool pause = true;

	bool mDisplayHalfsize = false;


	void setDetectionRange(float dist) {
		detectionRange = dist;
	}

	void saveConfigFile(int *totals) {
//		pSUperDisplay->saveConfigFileReq(totals);
	}

	void openDepth(const char* filepathname, float sizeDivision, const char *calibFilename, bool useCalibration)
	{
		std::string productName = string(filepathname);
		int pathoffset = productName.find_last_of('\\');
		std::string filepathStr = productName.substr(0, pathoffset);
		std::string filenameStr = productName.substr(pathoffset + 1);
		productName = productName.substr(0, pathoffset);
		filenameStr = filenameStr.substr(0, filenameStr.find_last_of('.'));
		std::string windowCaption = "SuperDisplay ";
		windowCaption += filenameStr;
		Mat combImage = imread(filepathname);
		std::string fileConfigStr = filepathStr + "\\" + filenameStr + "_config.txt";

		if (pDepthDisplay != nullptr) delete pDepthDisplay;
		pDepthDisplay = new depthDisplay((char *)(windowCaption.c_str()), sizeDivision);
		pDepthDisplay->setConfigFile((char *)(fileConfigStr.c_str()), (char *)calibFilename, useCalibration);
		pDepthDisplay->showImg(combImage);
	}
}