#pragma once
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
#include <list>

#include "CVcommon.h"

using namespace cv;
using namespace std;
using namespace System;

class ContouredSubImage
{
private:
	Mat mImage;
	Mat mImageMask;
	Point mCorner;
	void ContouredSubImage::Erosion(Mat inputM, Mat* outputM, int erosion_elem, int erosion_size);
	void ContouredSubImage::Dilation(Mat inputM, Mat* outputM, int dilation_elem, int erosion_size);
	int countViableAreas(vector<vector<Point>> *pTestContours, int minArea, int maxArea);

public:
	ContouredSubImage();
	~ContouredSubImage();
	void loadImage(Mat srce, vector<Point> outline, float oversizeFactor);
	bool processImage(int *centreX, int *centreY, int *rad);
	Mat getImage() { return mImage; }
	Mat getMask() { return mImageMask; }
};

