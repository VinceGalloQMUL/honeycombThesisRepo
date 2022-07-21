#include "cvUtilities.h"



cvUtilities::cvUtilities()
{
}


cvUtilities::~cvUtilities()
{
}
void cvUtilities::Erosion(Mat inputM, Mat outputM, int erosion_elem, int erosion_size)
{
	if (erosion_size > 0) {
		int erosion_type = 0;
		if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
		else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
		else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
		Mat element = getStructuringElement(erosion_type,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));
		erode(inputM, outputM, element);

	}
}
void cvUtilities::Dilation(Mat inputM, Mat outputM, int dilation_elem, int dilation_size)
{
	if (dilation_size > 0) {
		int dilation_type = 0;
		if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
		else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
		else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
		Mat element = getStructuringElement(dilation_type,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
		dilate(inputM, outputM, element);
	}
}
void cvUtilities::imageRebase(Mat srce, Mat *dest, int baseLevel, int contrast) {
	Mat A = Mat::ones(srce.rows, srce.cols, CV_8U) * ((2 * baseLevel) % 256); // make 100x100 matrix filled with 3.
	float baseVal;
	float fltVal = contrast;
	fltVal /= 128;
	if (baseLevel > 128) {
		*dest = (srce * fltVal) - A;
	}
	else {
		*dest = A + (srce * fltVal);

	}
	//int srceChannels = srce.channels();
	//for (int y = 0; y < srce.rows; y++) {
	//	uchar* pd1 = srce.ptr<uchar>(y);
	//	uchar* pdd = dest->ptr<uchar>(y);
	//	for (int x = 0; x < srce.cols; x++) {
	//		int idx = x * srceChannels;
	//		if (contrast == 0) {
	//			int orgPxl = 0;
	//			for (int c = 0; c < srceChannels; c++) {
	//				orgPxl += pd1[idx + c];
	//			}
	//			orgPxl /= srceChannels;
	//			for (int c = 0; c < srceChannels; c++) {
	//				pdd[idx + c] = orgPxl;
	//			}
	//		}
	//		else {
	//			for (int c = 0; c < srceChannels; c++) {
	//				int orgPxl = pd1[idx + c];
	//				int tmpProd = ((orgPxl * contrast) / 10) + baseLevel;
	//				if (tmpProd > 255) {
	//					tmpProd = 255;
	//				}
	//				pdd[idx + c] = tmpProd;
	//			}
	//		}
	//	}
	//}
}
int cvUtilities::imageThreshold(Mat srce, int baseLevel, int newLevel) {
	int retv = 0;
	int srceChannels = srce.channels();
	for (int y = 0; y < srce.rows; y++) {
		uchar* pd1 = srce.ptr<uchar>(y);
		for (int x = 0; x < srce.cols; x++) {
			int idx = x * srceChannels;
			for (int c = 0; c < srceChannels; c++) {
				int orgPxl = pd1[idx + c];
				if (orgPxl >= baseLevel) {
					pd1[idx + c] = newLevel;
					retv++;
				}
			}
		}
	}
	return retv;
}

int cvUtilities::findImageContours(Mat inputM, vector<vector<Point>> &contours, vector<Vec4i> &hierarchy, bool filterBySize, float filterMin, float filterMax) {

	findContours(inputM, contours, hierarchy, RETR_LIST /*RETR_EXTERNAL*/, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector< vector < Point >> tmpContours;
	if (filterBySize) {
		float tmpSize;
		for (size_t i = 0; i < contours.size(); i++)
		{ // test each contour as to how well the colour matches
			tmpSize = (float)(contourArea(contours[i]));
			Rect boundry = boundingRect(contours[i]);
			if (filterMax > tmpSize && filterMax < tmpSize * 5) {
				if (tmpSize > boundry.area() * (filterMin / 100)) {
					tmpContours.push_back(contours[i]);
				}
			}
		}
		contours.clear();
		contours.resize(tmpContours.size());
		vector<vector<Point> >hull(contours.size());
		for (size_t i = 0; i < tmpContours.size(); i++) {
			//contours.push_back(/tmpContours[i]/));
			convexHull( tmpContours[i], contours[i]);
		}
		tmpContours.clear();
		return (int)(contours.size());
	}
}
void cvUtilities::drawImageContours(Mat inputM, vector<vector<Point>> contours, int lineWidth) {
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(inputM, contours, (int)i, Scalar(255,255,255), lineWidth);
	}
}
void cvUtilities::drawImageContourCentres(Mat inputM, vector<Point2f> centres, int dotSize) {
	Moments cMom;
	for (size_t i = 0; i < centres.size(); i++)
	{
		circle(inputM,  centres[i] ,dotSize, Scalar(255, 255, 255), -1);
	}
}
void cvUtilities::drawImageContourConnections(Mat inputM, vector<Point2f> centres, vector<vector<int>> &connections) {
	for (size_t i = 0; i < centres.size(); i++)
	{
		if (connections[i].size() > 0) {
			circle(inputM,  centres[i] ,3, Scalar(255, 255, 255), -1);
			for (size_t j = 0; j < connections[i].size(); j++) {
				line(inputM, centres[i], centres[(connections[i])[j]], Scalar(255, 255, 255), 1);
			}

		}
	}

}
void cvUtilities::getContourCentres(vector<vector<Point>> contours,  vector<Point2f>&centres) {
	centres.clear();
	Moments cMom;
	for (size_t i = 0; i < contours.size(); i++)
	{
		cMom = moments(contours[i]);
		centres.push_back(Point(cMom.m10/cMom.m00, cMom.m01/cMom.m00));
	}
}
float distSq(Point2f pt0, Point2f pt1) {
	float dimX = pt0.x - pt1.x;
	float dimY = pt0.y - pt1.y;
	return dimX * dimX + dimY * dimY;
}
void cvUtilities::getCentreConnections(vector<vector<Point>> &contours, vector<Point2f>&centres, vector<vector<int>> &connections) {
	int minI = -1;
	int minJ = -1;
	float minDist = 10000000;
	for (size_t i = 0; i < connections.size(); i++) {
		connections[i].clear();
	}
	connections.clear();
	for (size_t i = 0; i < centres.size(); i++)
	{
		Rect boundry = boundingRect(contours[i]);
		float radSq = boundry.area();
		float distLimit = radSq * 2;
		vector<int> *thisConnections = new vector<int>();
		for (size_t j = 0; j < centres.size(); j++)
		{
			if (i != j) {
				Rect boundryJ = boundingRect(contours[j]);
				float radSqJ = boundryJ.area();
				distLimit = (radSqJ < radSq) ? radSqJ : radSq;
				distLimit *= 2;
				float dist = distSq(centres[i], centres[j]);
				if (dist < minDist) {
					minDist = dist;
					minI = i;
					minJ = j;
				}
				if (dist < distLimit) {
					thisConnections->push_back(j);
				}
			}

		}
		connections.push_back(*thisConnections);
	}

}
