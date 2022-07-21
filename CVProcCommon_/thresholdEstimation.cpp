#include "cvUtilities.h"
#include "thresholdEstimation.h"



thresholdEstimation::thresholdEstimation()
{
}


thresholdEstimation::~thresholdEstimation()
{
}

int thresholdEstimation::estimate(Mat inputM, float thresholdMargin) {
	int minT = 0;
	int maxT = 255;
	int candidateT = 128;
	bool foundT = false;
	Mat tmpAdjustedImage;
	int minSensibleNative = ((int)(1 / thresholdMargin)) + 1;
	while (!foundT)
	{
		if ((maxT - minT) < 2) {
			break;
		}
		tmpAdjustedImage = inputM.clone();
		candidateT = (minT + maxT) / 2;
		int nativePixels = cvUtilities::imageThreshold(tmpAdjustedImage, candidateT, 255);
		cvUtilities::Erosion(tmpAdjustedImage, tmpAdjustedImage, 0, 1);
		cvUtilities::Dilation(tmpAdjustedImage, tmpAdjustedImage, 0, 1);
		int eroded = cvUtilities::imageThreshold(tmpAdjustedImage, 255, 255);
		int delta = nativePixels - eroded;
		if (delta < 0)
			delta = -delta;
		if ((nativePixels < minSensibleNative) || delta < (thresholdMargin * nativePixels)) {
			// threshold too high
			maxT = candidateT;
		}
		else {
			minT = candidateT;
		}
	}
	return candidateT;
}

int thresholdEstimation::findThreshold(Mat srce, int reasonableSizeMin, int reasonableSizeMax) {
	int retvMin = -1;
	int retvMax = -1;
	int histSize = 256;
	int histSizes[] = { histSize, histSize };
	float range[] = { 0, 256 };
	const float* histRanges[] = { range, range };
	int channels[] = { 0, 1 };
	bool uniform = true; bool accumulate = false;

	Mat hist;
	Mat tmpSection;
	vector<Mat> planes;
	split(srce, planes);
	calcHist(&planes[0], 1, channels, Mat(), hist, 1, histSizes, histRanges, uniform, accumulate);
	int cummulativeAboveThreshold = 0;
	for (int i = histSize - 1; i > 0; i--)
	{
		float oneVal = hist.at<float>(i);
		cummulativeAboveThreshold += oneVal;
		if (cummulativeAboveThreshold > reasonableSizeMax) {
			retvMax = i;
			break;
		}
		if (cummulativeAboveThreshold > reasonableSizeMin && retvMin == -1) {
			retvMin = i;
		}
	}
	int retv = (retvMin + retvMax) / 2;
	return retv;
}


