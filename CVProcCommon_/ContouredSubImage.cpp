#include "ContouredSubImage.h"



ContouredSubImage::ContouredSubImage()
{
}


ContouredSubImage::~ContouredSubImage()
{
}

void ContouredSubImage::Erosion(Mat inputM, Mat* outputM, int erosion_elem, int erosion_size)
{
	int erosion_type = 0;
	if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	erode(inputM, *outputM, element);
}
void ContouredSubImage::Dilation(Mat inputM, Mat* outputM, int dilation_elem, int erosion_size)
{
	int dilation_type = 0;
	if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
	Mat element = getStructuringElement(dilation_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	dilate(inputM, *outputM, element);
}

int ContouredSubImage::countViableAreas(vector<vector<Point>> *pTestContours, int minArea, int maxArea) {
	int retv = 0;
	for (int idx = 0; idx < pTestContours->size(); idx++) {
		int area = contourArea(pTestContours->at(idx));
		if (area < 0) area = -area;
		if (area >= minArea && area <= maxArea)
			retv++;
	}
	return retv;
}
#define MIN_CIRCUMFERENCE 15
#define MIN_AREA 30
bool ContouredSubImage::processImage(int *tlX, int *tlY, int *rad) {
	bool retv = false;
	int orgThresh = 140;
	int thresh;
	Mat thrshImg;
	int imageArea = mImage.rows * mImage.cols;
	int minCircumference = (mImage.rows + mImage.cols) / 5;
	if (minCircumference < MIN_CIRCUMFERENCE) minCircumference = MIN_CIRCUMFERENCE;
	int minCircleArea = imageArea / 100;
	if (minCircleArea < MIN_AREA) minCircleArea = MIN_AREA;
	int maxCircleArea = imageArea / 15;
	cvtColor(mImage, mImage, CV_BGR2GRAY);
	medianBlur(mImage, mImage, 5);
	vector<vector<Point>> tmpContours;
	for (thresh = orgThresh; thresh > 50; thresh -= 10) {
		mImage.copyTo(thrshImg);
		threshold(thrshImg, thrshImg, thresh, 255, 0);
		tmpContours.clear();
		findContours(thrshImg, tmpContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		if (countViableAreas(&tmpContours, minCircleArea, maxCircleArea) > 1) {
			break;
		}
		orgThresh = thresh;
	}
	thresh = orgThresh;
	threshold(thrshImg, thrshImg, thresh, 255, 0);
	//imshow("thrshImg", thrshImg.clone());
	Erosion(thrshImg, &thrshImg, 0, 2);
	//imshow("thrshImgE1", thrshImg.clone());
	Dilation(thrshImg, &thrshImg, 0, 4);
	//imshow("thrshImgD", thrshImg.clone());
	Erosion(thrshImg, &thrshImg, 0, 2);
	//imshow("thrshImgE", thrshImg.clone());
	tmpContours.clear();
	findContours(thrshImg, tmpContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int idx = 0; idx < tmpContours.size(); idx++) {
		float circumference = (float)arcLength(tmpContours[idx], true);
		int area = contourArea(tmpContours[idx]);
		if (area >= minCircleArea && area <= maxCircleArea) {
//			vector<Point> approx;
			if (circumference > minCircumference) {
//				approxPolyDP(tmpContours[idx], approx, circumference * 0.07f, true);
				Rect circleRect = boundingRect(tmpContours[idx]); // approx);
				float aspectRatio = (float)circleRect.width / (float)circleRect.height;
				area = contourArea(tmpContours[idx]); // approx);
				if (area < 0) area = -area;
				float areaRatio = (float)(circleRect.width * circleRect.height) / (float)(area);
				if (area >= minCircleArea && area <= maxCircleArea && areaRatio < 3.0f && aspectRatio > 0.6f && aspectRatio < 1.65f) {
					vector<vector<Point>> tmpCont;
					//tmpCont.push_back(approx);
					drawContours(mImage, tmpContours, idx, Scalar(255, 255, 255)); // reduce the mask size
					retv = true;
					*rad = circleRect.width / 2;
					*tlX = mCorner.x + circleRect.tl().x;
					*tlY = mCorner.y + circleRect.tl().y;
				}
			}
		}
		return retv;
	}

//	threshold(mImage, mImage, thresh, 255, 0);
//	drawContours(mImageMask, tmpContours, 0, Scalar(255, 255, 255), -1, 8, Mat(), 1, Point(-cornerX, -cornerY));
//	drawContours(mImageMask, tmpContours, 0, Scalar(0, 0, 0), 2, 8, Mat(), 1, Point(-cornerX, -cornerY)); // reduce the mask size

	//Dilation(mImage, &mImage, 0, 2);
	//Erosion(mImage, &mImage, 0, 2);

}

void ContouredSubImage::loadImage(Mat srce, vector<Point> outline, float oversizeFactor) {
	int pxlR = 0;
	int pxlG = 0;
	int pxlB = 0;
	int outW, outH;
	int marginW, marginH;
	int cornerX, cornerY;

	Rect contourBox = boundingRect(outline);
	marginW = (float)contourBox.width * oversizeFactor;
	marginH = (float)contourBox.height * oversizeFactor;
	outH = contourBox.height + 2 * marginH;
	outW = contourBox.width + 2 * marginW;
	cornerX = contourBox.tl().x - marginW;
	cornerY = contourBox.tl().y - marginW;
	if (cornerX < 0) cornerX = 0;
	if (cornerY < 0) cornerY = 0;
	mCorner = Point(cornerX, cornerY);
	mImageMask = Mat::zeros(outH, outW, CV_8UC1);
	vector<vector<Point>> tmpContours;
	tmpContours.push_back(outline);
	drawContours(mImageMask, tmpContours, 0, Scalar(255, 255, 255), -1, 8, Mat(), 1, Point(-cornerX, -cornerY));
	drawContours(mImageMask, tmpContours, 0, Scalar(0, 0, 0), 2, 8, Mat(), 1, Point(-cornerX, -cornerY)); // reduce the mask size
	tmpContours.clear();
	findContours(mImageMask, tmpContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	if (tmpContours.size() > 0) {
		int pxlCnt = tmpContours[0].size();
		for (int idx = 0; idx < pxlCnt; idx++) {
			Vec3b colour = srce.at<Vec3b>(tmpContours[0][idx]);
			pxlB += colour.val[0];
			pxlG += colour.val[1];
			pxlR += colour.val[2];
		}
		pxlB /= pxlCnt;
		pxlG /= pxlCnt;
		pxlR /= pxlCnt;

	}
	else {
		pxlB = 128;
		pxlG = 128;
		pxlR = 128;
	}
	mImage = Mat(outH, outW, CV_8UC3, Scalar(pxlB, pxlG, pxlR));
	Dilation(mImageMask, &mImageMask, 0, 4);
	Erosion(mImageMask, &mImageMask, 0, 6);
	srce.rowRange(cornerY, cornerY + outH).colRange(cornerX, cornerX + outW).copyTo(mImage, mImageMask);
}

