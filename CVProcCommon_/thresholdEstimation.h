#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

//#include <list>


using namespace cv;
using namespace std;

class thresholdEstimation
{
public:
	thresholdEstimation();
	~thresholdEstimation();

	// assumes canavs is monochrome
	// threshold margin will typically be 0.1, is 10%
	// estimates the monochrome threshold that will eliminate noise, done by testing a threshold value and
	// counting the number of pixels above that level, then eroding 1, dilating 1 and testing the difference
	// when the noise and the thin strings are eliminated the difference will drop, and typically when it falls to lower
	// than 10% we are good to go.
	static int estimate(Mat canvas, float thresholdMargin);
	// find the threshold for each of the min and max, then split the difference
	static int thresholdEstimation::findThreshold(Mat srce, int reasonableSizeMin, int reasonableSizeMax);

};

