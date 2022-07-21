#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

//#include <list>


using namespace cv;
using namespace std;

class cvUtilities
{
public:
	cvUtilities();
	~cvUtilities();
	static void cvUtilities::Erosion(Mat inputM, Mat outputM, int erosion_elem, int erosion_size);
	static void cvUtilities::Dilation(Mat inputM, Mat outputM, int dilation_elem, int erosion_size);
	static void cvUtilities::imageRebase(Mat srce, Mat *dest, int baseLevel, int contrast);
	// manual implementation of the CV threshold method, but this one returns a count of the pixels changed (ie those above the threshold)
	static int cvUtilities::imageThreshold(Mat srce, int baseLevel, int newLevel);

	static int cvUtilities::findImageContours(Mat inputM, vector<vector<Point>> &contours, vector<Vec4i> &hierarchy, bool filterBySize, float filterMin, float filterMax);
	static void cvUtilities::drawImageContours(Mat inputM, vector<vector<Point>> contours, int lineWidth);
	static void cvUtilities::drawImageContourCentres(Mat inputM, vector<Point2f> centres, int dotSize);
	static void cvUtilities::getContourCentres(vector<vector<Point>> contours, vector<Point2f>&centres);
	static void getCentreConnections(vector<vector<Point>> &contours, vector<Point2f>&centres, vector<vector<int>> &connections);
	static void cvUtilities::drawImageContourConnections(Mat inputM, vector<Point2f> centres, vector<vector<int>> &connections);

};

