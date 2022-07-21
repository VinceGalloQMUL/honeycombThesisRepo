#define _CRT_SECURE_NO_WARNINGS

#include "cvUtilities.h"
#include "adjustableDisplay.h"
//#include "thresholdEstimation.h"
#include <cmath>

adjustableDisplay::adjustableDisplay(char *windowName)
	: mWindowName(nullptr)
{
	mWindowName = new char[strlen(windowName) + 1];
	strcpy(mWindowName, windowName);
	mWindowHistName = new char[strlen(windowName) + 2];
	mWindowHistName[0] = 'H';
	strcpy(mWindowHistName + 1, windowName);
	mWindowStatsName = new char[strlen(windowName) + 2];
	mWindowStatsName[0] = 'S';
	strcpy(mWindowStatsName + 1, windowName);
	mWindowContoursName = new char[strlen(windowName) + strlen("Contours ") + 1];
	strcpy(mWindowContoursName, "Contours ");
	strcat(mWindowContoursName, windowName);
	mContrast = 210;
	mBaseLevel = 220;
	mThreshold = 17;
	mErodeQty = -1;
	mDilateQty = -1;
	mContourMax = 50;
	mContourMin = 50;
	mIsolation = 0;
	mBlur = 0;
	mHistogramDisplayed = false;
	mTrackVal = 128;
	mDoLog = 0;
	mSliderName = "Slider";
	mMenuStr[MENU_BASE] = "Base";
	mMenuStr[MENU_CONTRAST] = "Contrast";
	mMenuStr[MENU_ERODE] = "Erode";
	mMenuStr[MENU_DILATE] = "Dilate";
	mMenuStr[MENU_THRESHOLD] = "Thrshld";
	mMenuStr[MENU_CONTOURS] = "Contours";
	mMenuStr[MENU_AREA_MAX] = "Area Max";
	mMenuStr[MENU_AREA_MIN] = "Area Min";
	mMenuStr[MENU_BLUR] = "Blur";
	mMenuStr[MENU_ISOLATION] = "Isoltn";
	mMenuStr[MENU_HIST] = "Histogram";
	mMenuStr[MENU_INVERT] = "Invert";
	mMenuStr[MENU_EDGE] = "Edge";
	mMenuStr[MENU_RECYCLE] = "Recycle";
	mMenuSelected[MENU_BASE] = false;
	mMenuSelected[MENU_CONTRAST] = false;
	mMenuSelected[MENU_ERODE] = false;
	mMenuSelected[MENU_DILATE] = false;
	mMenuSelected[MENU_THRESHOLD] = false;
	mMenuSelected[MENU_CONTOURS] = false;
	mMenuSelected[MENU_AREA_MAX] = false;
	mMenuSelected[MENU_AREA_MIN] = false;
	mMenuSelected[MENU_BLUR] = false;
	mMenuSelected[MENU_ISOLATION] = false;
	mMenuSelected[MENU_HIST] = false;
	mMenuSelected[MENU_INVERT] = false;
	mMenuSelected[MENU_EDGE] = false;
	mMenuSelected[MENU_RECYCLE] = false;
	mMenuSelection = MENU_NONE;
}
adjustableDisplay::~adjustableDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;
	if (mWindowHistName != nullptr) delete mWindowHistName;
}

void adjustableDisplay::adjustDisplay() {

}

void adjustableDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(mDisplayImage, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void adjustableDisplay::showMenu() {
	mAdjustedImage.copyTo(mDisplayImage);
	char menuLine[64];
	int y = 0;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_BASE], mBaseLevel, mMenuSelected[MENU_BASE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_CONTRAST], mContrast, mMenuSelected[MENU_CONTRAST] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_ERODE], mErodeQty, mMenuSelected[MENU_ERODE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_DILATE], mDilateQty, mMenuSelected[MENU_DILATE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_THRESHOLD], mThreshold, mMenuSelected[MENU_THRESHOLD] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_CONTOURS], mMenuSelected[MENU_CONTOURS] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_AREA_MIN], mContourMin, mMenuSelected[MENU_AREA_MIN] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_AREA_MAX], mContourMax, mMenuSelected[MENU_AREA_MAX] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_BLUR], mBlur, mMenuSelected[MENU_BLUR] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %d %c", mMenuStr[MENU_ISOLATION], mIsolation, mMenuSelected[MENU_ISOLATION] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_HIST], mMenuSelected[MENU_HIST] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_INVERT], mMenuSelected[MENU_INVERT] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_EDGE], mMenuSelected[MENU_EDGE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_RECYCLE], mMenuSelected[MENU_RECYCLE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	imshow(mWindowName, mDisplayImage);
}


void adjustableDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	adjustableDisplay *pAdjustableDisplay = (adjustableDisplay *)userdata;
	pAdjustableDisplay->mouseCallback(event, x, y, flags);
}
void adjustableDisplay::trackCallback(int val, void* userdata) {
	adjustableDisplay *pAdjustableDisplay = (adjustableDisplay *)userdata;
	pAdjustableDisplay->trackCallback(val);
}

void adjustableDisplay::clearAndSetMenu(int menuSelection) {
	bool holdContours = mMenuSelected[MENU_CONTOURS];
	if (mMenuSelection != MENU_NONE) {
		mMenuSelected[mMenuSelection] = false;
	}
	mMenuSelected[MENU_CONTOURS] = holdContours;
	mMenuSelection = menuSelection;
	if (menuSelection != MENU_NONE) {
		mMenuSelected[menuSelection] = true;
	}
}

void adjustableDisplay::mouseCallback(int event, int x, int y, int flags) {
	if (x < 0) x = 0;
	if (x >= mCols) x = mCols - 1;
	if (y < 0) y = 0;
	if (y >= mRows) y = mRows - 1;

	if (event == EVENT_LBUTTONDOWN)
	{
		int tmpMenu = menuSelected(x, y);
		int tmpSliderVal = -1;
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_BASE:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mBaseLevel;
				break;
			case MENU_CONTRAST:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mContrast;
				break;
			case MENU_ERODE:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = (mErodeQty + 1) * 16;
				break;
			case MENU_DILATE:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = (mDilateQty + 1) * 16;
				break;
			case MENU_THRESHOLD:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mThreshold;
				break;
			case MENU_CONTOURS:
				mMenuSelected[MENU_CONTOURS] = !mMenuSelected[MENU_CONTOURS];
				//if (mMenuSelected[MENU_CONTOURS]) {
				//	mMenuSelection = MENU_CONTOURS;
				//}
				//else {
				//	mMenuSelection = MENU_NONE;
				//}
				break;
			case MENU_AREA_MIN:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mContourMin;
				break;
			case MENU_AREA_MAX:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mContourMax;
				break;
			case MENU_BLUR:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mBlur;
				break;
			case MENU_ISOLATION:
				clearAndSetMenu(tmpMenu);
				tmpSliderVal = mIsolation;
				break;
			case MENU_HIST:
				showOrHideHistogram(mAdjustedImage);
				mMenuSelected[MENU_HIST] = mHistogramDisplayed;
				mMenuSelection = (mHistogramDisplayed) ? MENU_HIST : MENU_NONE;
				tmpSliderVal = mDoLog;
				break;
			case MENU_INVERT:
				mMenuSelected[MENU_INVERT] = !mMenuSelected[MENU_INVERT];
				break;
			case MENU_EDGE:
				mMenuSelected[MENU_EDGE] = !mMenuSelected[MENU_EDGE];
				break;
			case MENU_RECYCLE:
				mMenuSelected[MENU_RECYCLE] = !mMenuSelected[MENU_RECYCLE];
				break;

			}
			if (tmpSliderVal >= 0) {
				cvSetTrackbarPos(mSliderName, mWindowName, tmpSliderVal);
			}
			showMenu();
		}
		else {
			showSectionStats(x, y, mAdjustedImage, 24);
		}
	}
}
void adjustableDisplay::trackCallback(int val) {
	if (mMenuSelection != MENU_NONE) {
		switch (mMenuSelection) {
		case MENU_BASE:
			mBaseLevel = val;
			break;
		case MENU_CONTRAST:
			mContrast = val;
			break;
		case MENU_ERODE:
			mErodeQty = (val / 16) - 1;
			break;
		case MENU_DILATE:
			mDilateQty = (val / 16) - 1;
			break;
		case MENU_THRESHOLD:
			mThreshold = val;
			break;
		case MENU_CONTOURS:
			break;
		case MENU_AREA_MIN:
			mContourMin = val;
			break;
		case MENU_AREA_MAX:
			mContourMax = val;
			break;
		case MENU_HIST:
			mDoLog = val;
			break;
		case MENU_BLUR:
			mBlur = val;
			break;
		case MENU_ISOLATION:
			mIsolation = val;
			break;
		}
		adjustImage();
		if (mHistogramDisplayed) {
			showHistFlex(mAdjustedImage, mDoLog);
		}
		showMenu();
	}

}

int adjustableDisplay::menuSelected(int x, int y) {
	int retv = MENU_NONE;
	int maxy = MENU_QTY * MENU_PANEL_LINE_HEIGHT;
	if (x <= MENU_PANEL_WIDTH && y > 0 && y <= maxy) {
		retv = y / MENU_PANEL_LINE_HEIGHT;
	}
	return retv;
}


// show hist - real one
//void adjustableDisplay::showHist(Mat bwImage, int doLog) {
//	int trimDisp = doLog / 3;
//	doLog = doLog % 3;
//	int histSize = 256;
//	float range[] = { 1, 256 };
//	float histDelta[256];
//	const float* histRange = { range };
//	bool uniform = true; bool accumulate = false;
//	vector<Mat> bgr_planes;
//	split(bwImage, bgr_planes);
//	Mat hist;
//	calcHist(&bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
//	int hist_w = 512;
//	int hist_h = 400;
//	int bin_w = cvRound((double)hist_w / histSize);
//	Mat HSVI10mage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
//	int histMax = 0;
//	String histStr;
//	char oneStr[32];
//	float previousVal = -1;
//	float deltaMax = 0;
//	float histAverage = 0;
//	float histStatsItemCount = 0;
//	float histVar = 0;
//	float histHalfVar = 0;
//	float histHalfCount = 0;
//	for (int i = 1; i < histSize; i++)
//	{
//		float oneVal = hist.at<float>(i - 1);
//		if (doLog > 0) {
//			//if (oneVal > 1) {
//			//	oneVal = log(oneVal);
//			//	if (doLog > 1) {
//			//		if (oneVal > 1) {
//			//			oneVal = log(oneVal);
//			//		}
//			//	}
//			//}
//			if (oneVal >= 1) {
//				oneVal = log(oneVal);
//				if (doLog > 1) {
//					if (oneVal > 1) {
//						oneVal = log(oneVal);
//					}
//					else {
//						oneVal = 0;
//					}
//				}
//			}
//			////oneVal = log(oneVal + 1);
//				////if (doLog > 1) {
//				////		oneVal = log(oneVal + 1);
//				////}
//		}
//		hist.at<float>(i - 1) = oneVal;
//		histStatsItemCount += oneVal;
//		histAverage += oneVal * (i - 1);
//		if (previousVal < 0) previousVal = oneVal;
//		histDelta[i] = oneVal - previousVal;
//		previousVal = oneVal;
//		if (histDelta[i] > 0) {
//			if (deltaMax < histDelta[i]) deltaMax = histDelta[i];
//		}
//		else {
//			if (deltaMax < -histDelta[i]) deltaMax = -histDelta[i];
//		}
//		if (histMax < hist.at<float>(i - 1))
//			histMax = hist.at<float>(i - 1);
//		sprintf(oneStr, "%d, ", (int)(hist.at<float>(i - 1)));
//		histStr += oneStr;
//	}
//	histAverage /= histStatsItemCount; // change sum into average
//	for (int i = 1; i < histSize; i++)
//	{
//		float oneVal = hist.at<float>(i - 1);
//		oneVal *= ((i - 1) - histAverage) * ((i - 1) - histAverage);
//		histVar += oneVal;
//		if ((i - 1) > histAverage) {
//			histHalfVar += oneVal;
//			histHalfCount += hist.at<float>(i - 1);
//		}
//	}
//	histVar /= histStatsItemCount;
//	histVar = sqrt(histVar);
//	float histTotal2 = 0;
//	float histAverage2 = 0;
//	float histVar2 = 0;
//	int trimLimit = histAverage + histVar;
//	for (int i = 1; i < 256; i++)
//	{
//		if (i > trimLimit) {
//			histTotal2 += hist.at<float>(i - 1);
//			histAverage2 += hist.at<float>(i - 1) * i;
//		}
//	}
//	histAverage2 /= histTotal2;
//	for (int i = 0; i < 256; i++)
//	{
//		if (i > trimLimit) {
//			float tmp;
//			tmp = (i - histAverage2);
//			histVar2 += hist.at<float>(i - 1) * tmp * tmp;
//		}
//	}
//	histVar2 /= histTotal2;
//	histVar2 = sqrt(histVar2);
//	histHalfVar /= histHalfCount;
//	histHalfVar = sqrt(histHalfVar);
//	if (trimDisp > 0) {
//		int trimLevel = histAverage + (trimDisp - 1) * histVar;
//
//		for (int i = 1; i < histSize; i++)
//		{
//			if (i < trimLevel) {
//				hist.at<float>(i - 1) = 0;
//			}
//		}
//	}
//	normalize(hist, hist, 0, HSVI10mage.rows, NORM_MINMAX, -1, Mat());
//	deltaMax = hist_h / (2 * deltaMax);
//	for (int i = 1; i < histSize; i++)
//	{
//		line(HSVI10mage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
//			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
//			Scalar(255, 255, 255), 1, 8, 0);
//		line(HSVI10mage, Point(bin_w*(i - 1), hist_h / 2 + histDelta[i - 1] * deltaMax),
//			Point(bin_w*(i), hist_h / 2 + histDelta[i] * deltaMax),
//			Scalar(255, 55, 55), 1, 8, 0);
//	}
//	stringstream ss;
//	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
//	//	cv::Scalar(255, 255, 255), -1);
//	ss << histMax << " " << histAverage << " " << histVar << " " << histAverage2 << " " << histVar2  << " " << histHalfVar;
//	string frameNumberString = ss.str();
//	putText(HSVI10mage, frameNumberString.c_str(), cv::Point(15, 15),
//		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255));
//	imshow(mWindowHistName, HSVI10mage);
//	saveToClipboard(histStr);
//}

void adjustableDisplay::showOrHideHistogram(Mat srce) {
	if (mHistogramDisplayed) {
		destroyWindow(mWindowHistName);
		mHistogramDisplayed = false;
	}
	else {
		showHistFlex(srce, mDoLog);
		mHistogramDisplayed = true;
	}
}


void adjustableDisplay::showImg(Mat img, float divisionFactor) {
	mDivisionFactor = divisionFactor;
	mOrgImage = img.clone();
	cvtColor(mOrgImage, mOrgImage, COLOR_BGRA2GRAY);
	mRows = img.rows;
	mCols = img.cols;
	mHistogramComputed = false;
	adjustImage();
	imshow(mWindowName, mAdjustedImage);
	setMouseCallback(mWindowName, adjustableDisplay::mouseCallback, this);
	createTrackbar(mSliderName, mWindowName, &mTrackVal, 256, adjustableDisplay::trackCallback, this);
	showMenu();
}


vector<vector<Point>> contours;
vector<Point2f> centres;
vector<vector<int>> connections;

void adjustableDisplay::adjustImage() {
	Mat startImage;
	if (mMenuSelected[MENU_RECYCLE]) {
		startImage = mRecycleImage.clone();
	}
	else {
		startImage = mOrgImage.clone();

	}
	imshow("startImage", startImage);

	//Mat img_gray;

//	mOrgImage.copyTo(mAdjustedImage);
	cvUtilities::imageRebase(startImage, &mAdjustedImage, mBaseLevel, mContrast);
//	Mat mClrImage = mAdjustedImage.clone();
//	cvtColor(mClrImage, mAdjustedImage, COLOR_BGRA2GRAY);
	Mat rebasedImage = mAdjustedImage.clone();;
	if (mMenuSelected[MENU_INVERT]) {
		mAdjustedImage = (255) - mAdjustedImage;
	}
	if (mIsolation > 0) {

		imageIsolation(mAdjustedImage.clone(), &mAdjustedImage, mIsolation);
	}
	if (mBlur > 0) {
		int blurType = mBlur / 50;
		int blurVal = mBlur % 50;
		if (blurType == 0)
		medianBlur(mAdjustedImage, mAdjustedImage, blurVal * 2 + 1);
		if (blurType == 1)
			GaussianBlur(mAdjustedImage, mAdjustedImage, Size(blurVal * 2 + 1, blurVal * 2 + 1), 0, 0);
		if (blurType == 2)
			blur(mAdjustedImage, mAdjustedImage, Size(blurVal * 2 + 1, blurVal * 2 + 1));
	}
	if (mThreshold > 0) {
		adaptiveThreshold(mAdjustedImage, mAdjustedImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 63, mThreshold / 10);
		//Canny(mAdjustedImage, mAdjustedImage, mThreshold, 20 + mThreshold);
		//if (mThreshold > 128) {
		//	mAdjustedImage = mAdjustedImage + rebasedImage;
		//}
	}
		//threshold(mAdjustedImage, mAdjustedImage, mThreshold, 255, 0); // threshold_binary = 0
	cvUtilities::Erosion(mAdjustedImage, mAdjustedImage, 0, mErodeQty);
	cvUtilities::Dilation(mAdjustedImage, mAdjustedImage, 0, mDilateQty);
	if (mMenuSelected[MENU_CONTOURS]) {
		vector<Vec4i> hierarchy;
		float scaleMutiple =  2.5 * ((float)mRows / 2592);
		scaleMutiple *= scaleMutiple;
		cvUtilities::findImageContours(mAdjustedImage, contours, hierarchy, true, (float)mContourMin, (float)mContourMax * 10 * scaleMutiple);
		Mat contourImg =  mOrgImage.clone(); //mAdjustedImage * 0;//

//		cvUtilities::drawImageContours(contourImg, contours, 1);
		cvUtilities::getContourCentres(contours, centres);
		//cvUtilities::drawImageContourCentres(contourImg, centres, 3);
		cvUtilities::getCentreConnections(contours, centres, connections);
		cvUtilities::drawImageContourConnections(contourImg, centres, connections);

		imshow(mWindowContoursName, contourImg);
	}
	if (mMenuSelected[MENU_EDGE]) {
		//Mat img_gray;

		//cvtColor(mAdjustedImage, img_gray, COLOR_BGR2GRAY);
		//Mat img_blur;
		//GaussianBlur(mAdjustedImage, img_blur, Size(3, 3), 0);
		Mat edges;
		Canny(mAdjustedImage, edges, 100, 200, 3, false);
		mAdjustedImage = edges;
	}
	if (!mMenuSelected[MENU_RECYCLE]) {
		mRecycleImage = mAdjustedImage.clone();
	}
}

bool adjustableDisplay::getWallIntersection(Point2i ptFrom, Point2i ptTo, float * wallSide0, float *wallSide1) {
	bool retv = true;
	Point2i cell0Centre = ptFrom / mDivisionFactor;
	Point2i cell1Centre = ptTo / mDivisionFactor;
	int deltaX = cell1Centre.x - cell0Centre.x;
	int deltaY = cell1Centre.y - cell0Centre.y;
	vector<int> lineShades;
	uchar pixelVal;
	int startStep;
	int endStep;
	int stepRange;
	if (abs(deltaX) > abs(deltaY)) {
		int stepX = deltaX / abs(deltaX);
		float stepY = deltaY;
		stepY /= abs(deltaX);
		startStep = (abs(deltaX) / 3);
		endStep = ((2 * abs(deltaX)) / 3);
		stepRange = abs(deltaX);
		for (int idx = startStep; idx < endStep; idx++) {
			int tmpX = (int)(cell0Centre.x + idx * stepX);
			int tmpY = (int)(cell0Centre.y + stepY * idx);
			pixelVal = mAdjustedImage.at<uchar>(tmpY, tmpX );
			//pixelVal += mAdjustedImage.at<uchar>(tmpY - 1, tmpX );
			//pixelVal += mAdjustedImage.at<uchar>(tmpY + 1, tmpX );
			lineShades.push_back(pixelVal);
		}
	}
	else {
		float stepX = deltaX;
		stepX /= abs(deltaY);
		int stepY = deltaY / abs(deltaY);
		startStep = (abs(deltaY) / 3);
		endStep = ((2 * abs(deltaY)) / 3);
		stepRange = abs(deltaY);
		for (int idx = startStep; idx < endStep; idx++) {
			int tmpX = (int)(cell0Centre.x + idx * stepX);
			int tmpY = (int)(cell0Centre.y + stepY * idx);
			pixelVal = mAdjustedImage.at<uchar>(tmpY, tmpX);
			//pixelVal += mAdjustedImage.at<uchar>(tmpY, tmpX - 1);
			//pixelVal += mAdjustedImage.at<uchar>(tmpY, tmpX + 1);
			lineShades.push_back(pixelVal);
		}

	}
	int maxPixel = 0;
	int minPixel = 256;
	int peakIdx = -1;
	for (int pIdx = 0; pIdx < lineShades.size(); pIdx++) {
		if (lineShades[pIdx] > maxPixel) {
			maxPixel = lineShades[pIdx];
			peakIdx = pIdx;
		}
		if (lineShades[pIdx] < minPixel) {
			minPixel = lineShades[pIdx];
		}
	}
	if ((maxPixel < 48) || (maxPixel < (2 * minPixel))) { // not enough contrast to trust the outcome
		retv = false;
	}
	else {
		minPixel = (maxPixel * 3) / 4;
		int minIdx = peakIdx;
		int maxIdx = peakIdx;
		for (int pIdx = 1; pIdx < 2; pIdx++) {
			int testIdx = peakIdx - pIdx;
			if ((testIdx == minIdx - 1) && testIdx > 0 && lineShades[testIdx] > maxPixel) {
				minIdx = testIdx;
			}
			testIdx = peakIdx + pIdx;
			if ((testIdx == maxIdx + 1) && testIdx < lineShades.size() && lineShades[testIdx] > maxPixel) {
				maxIdx = testIdx;
			}
		}
		float range = (float)(stepRange);
		*wallSide0 = (float(minIdx + startStep))/ range;
		*wallSide1 = (float(maxIdx + startStep)) / range;
	}
	return retv;
}


void adjustableDisplay::getCellContoursCentresAndConnections(vector<vector<Point>> **retContours, vector<Point2f> **retCentres, vector<vector<int>> **retConnections) {
	*retContours = &contours;
	*retCentres = &centres;
	*retConnections = &connections;
}

void adjustableDisplay::setDisplayParams(float contrast, int baseLevel, int erodeQty, int dilateQty, int threshold) {
	mContrast = contrast;
	mBaseLevel = baseLevel;
	mErodeQty = erodeQty;
	mDilateQty = dilateQty;
	mThreshold = threshold;
}
void adjustableDisplay::getDisplayParams(float *pContrast, int *pBaseLevel, int *pErodeQty, int *pDilateQty, int *pThreshold) {
	*pContrast = mContrast;
	*pBaseLevel = mBaseLevel;
	*pErodeQty = mErodeQty;
	*pDilateQty = mDilateQty;
	*pThreshold = mThreshold;
}

int isolationDegree(int from, int to) {
	int retv = from - to;
	if (retv < 0) retv = 0;
	return retv;
}
int findLargest(int *samples, int qty) {
	int largest = *samples;
	int retv = 0;
	for (int idx = 1; idx < qty; idx++) {
		if (samples[idx] > largest) {
			largest = samples[idx];
			retv = idx;
		}
	}
	return retv;
}
void adjustableDisplay::imageIsolation(Mat srce, Mat *dest, int isolationLevel) {
	int pxlNeighbours[9];
	int pxlMinIsolation[9];
	if (isolationLevel >= 8) isolationLevel = 8;
	int srceChannels = srce.channels();
	for (int y = 1; y < srce.rows - 1; y++) {
		uchar* ps0 = srce.ptr<uchar>(y);
		uchar* psm = srce.ptr<uchar>(y-1);
		uchar* psp = srce.ptr<uchar>(y+1);
		uchar* pdd = dest->ptr<uchar>(y);
		for (int x = 1; x < srce.cols - 1; x++) {
			int idx = x * srceChannels;
			for (int c = 0; c < srceChannels; c++) {
				pxlNeighbours[0] = psm[idx - srceChannels - 1 + c];
				pxlNeighbours[1] = psm[idx - srceChannels + c];
				pxlNeighbours[2] = psm[idx - srceChannels + 1 + c];
				pxlNeighbours[3] = ps0[idx - 1 + c];
				pxlNeighbours[4] = ps0[idx + c];
				pxlNeighbours[5] = ps0[idx + 1 + c];
				pxlNeighbours[6] = psp[idx + srceChannels - 1 + c];
				pxlNeighbours[7] = psp[idx + srceChannels + c];
				pxlNeighbours[8] = psp[idx + srceChannels + 1 + c];
				int minIsolation = 0;
				for (int iso = 0; iso < isolationLevel; iso++) {
					int delta = isolationDegree(pxlNeighbours[4], pxlNeighbours[iso]);
					pxlMinIsolation[iso] = delta;
					if (minIsolation < delta) minIsolation = delta; // worst case degre before we are left with just these 4
				}
				int largestIsolationIdx = findLargest(&pxlMinIsolation[0], isolationLevel);
				for (int iso = isolationLevel; iso < 9; iso++) { // now check if any are higher
					int delta = isolationDegree(pxlNeighbours[4], pxlNeighbours[iso]);
					if (delta < pxlMinIsolation[largestIsolationIdx]) {
						pxlMinIsolation[largestIsolationIdx] = delta; // better case, case degre before we are left with just these 4
						largestIsolationIdx = findLargest(&pxlMinIsolation[0], isolationLevel);
					}
				}
				pdd[idx + c] = pxlNeighbours[4] - pxlMinIsolation[largestIsolationIdx];
			}
		}
	}
}
#define isolimit 25
void adjustableDisplay::imageIsolationTalestNeighbour(Mat srce, int *statsTalestNeighbour, int *statsAmTalest, int isolationLevel) {
	uchar *ps[isolimit * 2 + 1];
	if (isolationLevel >= isolimit) isolationLevel = isolimit;
	int srceChannels = srce.channels();
	for (int idxStats = 0; idxStats < 256; idxStats++)
		statsTalestNeighbour[idxStats] = statsAmTalest[idxStats] = 0;
	for (int y = isolationLevel; y < srce.rows - isolationLevel; y++) {
		ps[isolimit] = srce.ptr<uchar>(y);
		for (int iso = 1; iso <= isolationLevel; iso++) {
			ps[isolimit - iso] = srce.ptr<uchar>(y - iso);
			ps[isolimit + iso] = srce.ptr<uchar>(y + iso);
		}
		for (int x = isolationLevel; x < srce.cols - isolationLevel; x++) {
			for (int c = 0; c < srceChannels; c++) {
				int highestNeighbour = 0;
				int myVal = ps[isolimit][(x) * srceChannels + c];
				if (myVal > 0) {
					for (int isoy = 0; isoy < isolationLevel; isoy++) {
						uchar *psm = ps[isolimit - isoy];
						uchar *psp = ps[isolimit + isoy];
						for (int isox = 0; isox <= isolationLevel; isox++) {
							int tmppix = psm[(x + isox) * srceChannels + c];
							if (tmppix > highestNeighbour) highestNeighbour = tmppix;
							tmppix = psp[(x + isox) * srceChannels + c];
							if (tmppix > highestNeighbour) highestNeighbour = tmppix;
							tmppix = psm[(x - isox) * srceChannels + c];
							if (tmppix > highestNeighbour) highestNeighbour = tmppix;
							tmppix = psp[(x - isox) * srceChannels + c];
							if (tmppix > highestNeighbour) highestNeighbour = tmppix;
						}
					}
					if (highestNeighbour > myVal) {
						statsTalestNeighbour[highestNeighbour]++;
					}
					else {
						statsAmTalest[myVal]++;
					}
				}
			}
		}
	}
}
#define isoAreaLimit 128
void adjustableDisplay::image2ndTallest(Mat srce, int *stats2ndTalest, int isolationLevel) {
	if (isolationLevel < 4) isolationLevel = 4;
	if (isolationLevel >= isoAreaLimit) isolationLevel = isoAreaLimit;
	int srceChannels = srce.channels();
	for (int idxStats = 0; idxStats < 256; idxStats++) {
		stats2ndTalest[idxStats] = 0;
	}
	for (int y = 0; y < srce.rows - isolationLevel; y += isolationLevel) {
		for (int x = 0; x < srce.cols - isolationLevel; x += isolationLevel) {
			int max = 0;
			int max2nd = 0;
			for (int isoy = 0; isoy < isolationLevel; isoy++) {
				uchar *ps = srce.ptr<uchar>(y + isoy);
				for (int isox = 0; isox <= isolationLevel; isox++) {
					int idx = (x + isox) * srceChannels;
					for (int c = 0; c < srceChannels; c++) {
						int myVal = ps[idx + c];
						if (myVal > 0) {
							if (myVal > max) {
								max2nd = max;
								max = myVal;
							}
							else {
								if (myVal > max2nd) {
									max2nd = myVal;
								}
							}
						}
					}
				}
			}
			stats2ndTalest[max2nd]++;
		}
	}
}

void adjustableDisplay::imageHistOfAve(Mat srce, int *stats, int isolationLevel) {
	if (isolationLevel < 4) isolationLevel = 4;
	if (isolationLevel >= isoAreaLimit) isolationLevel = isoAreaLimit;
	int srceChannels = srce.channels();
	for (int idxStats = 0; idxStats < 256; idxStats++) {
		stats[idxStats] = 0;
	}
	for (int y = 0; y < srce.rows - isolationLevel; y += isolationLevel) {
		for (int x = 0; x < srce.cols - isolationLevel; x += isolationLevel) {
			int ave = 0;
			int nzQty = 0;
			for (int isoy = 0; isoy < isolationLevel; isoy++) {
				uchar *ps = srce.ptr<uchar>(y + isoy);
				for (int isox = 0; isox <= isolationLevel; isox++) {
					int idx = (x + isox) * srceChannels;
					for (int c = 0; c < srceChannels; c++) {
						int myVal = ps[idx + c];
						if (myVal > 0) {
							nzQty++;
							ave += myVal;
						}
					}
				}
			}
			//ave /= isolationLevel * isolationLevel;
			if (nzQty > 0) {
				ave /= nzQty;
			}
			else {
				ave = 0;
			}
			stats[ave]++;
		}
	}
}


void adjustableDisplay::showHist2ndTallest(Mat bwImage, int doLog) {
	int hist2ndTallest[256];
	image2ndTallest(bwImage, &hist2ndTallest[0], doLog);
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = 2;
	Mat HSVI10mage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	float histMax = 0;
	String histStr;
	float histScale;
	int maxIdx = 0;
	float histAverage = 0;
	float histVar = 0;
	float histTotal = 0;
	float histAverage2 = 0;
	float histVar2 = 0;
	float histTotal2 = 0;
	for (int i = 0; i < 256; i++)
	{
		histTotal += hist2ndTallest[i];
		histAverage += hist2ndTallest[i] * i;
		if (hist2ndTallest[i] > histMax) {
			histMax = (float)hist2ndTallest[i];
			maxIdx = i;
		}
	}
	histAverage /= histTotal;
	for (int i = 0; i < 256; i++)
	{
		float tmp;
		tmp = (i - histAverage);
		histVar += hist2ndTallest[i] * tmp * tmp;
	}
	histVar /= histTotal;
	histVar = sqrt(histVar);
	int trimLimit = (int)(histAverage + histVar * 3);
	int histCount2 = 0;
	for (int i = 0; i < 256; i++)
	{
		if (i > trimLimit && hist2ndTallest[i] > 0) {
			histCount2++;
			histTotal2 += hist2ndTallest[i];
			histAverage2 += hist2ndTallest[i] * i;
		}
	}
	histAverage2 /= histTotal2;
	for (int i = 0; i < 256; i++)
	{
		if (i > trimLimit) {
			float tmp;
			tmp = (i - histAverage2);
			histVar2 += hist2ndTallest[i] * tmp * tmp;
		}
	}
	histVar2 /= histTotal2;
	histVar2 = sqrt(histVar2);
	histScale = hist_h / histMax;
//	histScale = 1;
	for (int i = 1; i < 256; i++)
	{
//		if (histIsolation[i] < hist_h) {
			line(HSVI10mage, Point((int)(bin_w*(i - 1)), (int)(hist_h - histScale * hist2ndTallest[i - 1])),
				Point((int)(bin_w*(i)), (int)(hist_h - histScale * hist2ndTallest[i])),
				Scalar(255, 55, 55), 1, 8, 0);
//		}
	}
	stringstream ss;
	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
	//	cv::Scalar(255, 255, 255), -1);
	ss << histMax << " " << maxIdx << " " << histAverage << " " << histVar << " " << histCount2 << " " << histAverage2 << " " << histVar2;
	string frameNumberString = ss.str();
	putText(HSVI10mage, frameNumberString.c_str(), cv::Point(15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	imshow(mWindowHistName, HSVI10mage);
}
float adjustableDisplay::image2ndTallestThreshold(Mat srce, int scope, float varMultiple) {
	int hist2ndTallest[256];
	image2ndTallest(srce, &hist2ndTallest[0], scope);

	float histMax = 0;
	float histAverage = 0;
	float histVar = 0;
	float histTotal = 0;
	for (int i = 0; i < 256; i++)
	{
		histTotal += hist2ndTallest[i];
		histAverage += hist2ndTallest[i] * i;
	}
	histAverage /= histTotal;
	for (int i = 0; i < 256; i++)
	{
		float tmp;
		tmp = (i - histAverage);
		histVar += hist2ndTallest[i] * tmp * tmp;
	}
	histVar /= histTotal;
	histVar = sqrt(histVar);
	return histAverage + varMultiple * histVar;
}

void adjustableDisplay::showHistTallestNeighbour(Mat bwImage, int doLog) {
	int histTallestNeighbour[256];
	int histAmTallest[256];
	imageIsolationTalestNeighbour(bwImage, &histTallestNeighbour[0], &histAmTallest[0], doLog);
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = 2;
	Mat HSVI10mage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	float histMax = 0;
	float histMax2 = 0;
	String histStr;
	float histScale;
	float histScale2;
	int maxIdx = 0;
	int maxIdx2 = 0;
	float histAverage = 0;
	float histAverage2 = 0;
	float histVar = 0;
	float histVar2 = 0;
	float histTotal = 0;
	float histTotal2 = 0;
	for (int i = 0; i < 256; i++)
	{
		histTotal += histTallestNeighbour[i];
		histTotal2 += histAmTallest[i];
		histAverage += histTallestNeighbour[i] * i;
		histAverage2 += histAmTallest[i] * i;
		if (histTallestNeighbour[i] > histMax) {
			histMax = (float)histTallestNeighbour[i];
			maxIdx = i;
		}
		if (histAmTallest[i] > histMax2) {
			histMax2 = (float)histAmTallest[i];
			maxIdx2 = i;
		}
	}
	histAverage /= histTotal;
	histAverage2 /= histTotal2;
	for (int i = 0; i < 256; i++)
	{
		float tmp;
		tmp = (i - histAverage);
		histVar += histTallestNeighbour[i] * tmp * tmp;
		tmp = (i - histAverage2);
		histVar2 += histAmTallest[i] * tmp * tmp;
	}
	histVar /= histTotal;
	histVar2 /= histTotal2;
	histVar = sqrt(histVar);
	histVar2 = sqrt(histVar2);
	histScale = hist_h / histMax;
	histScale2 = hist_h / histMax2;
	//	histScale = 1;
	for (int i = 1; i < 256; i++)
	{
		//		if (histIsolation[i] < hist_h) {
		line(HSVI10mage, Point((int)(bin_w*(i - 1)), (int)(hist_h - histScale * histTallestNeighbour[i - 1])),
			Point((int)(bin_w*(i)), (int)(hist_h - histScale * histTallestNeighbour[i])),
			Scalar(255, 55, 55), 1, 8, 0);
		line(HSVI10mage, Point((int)(bin_w*(i - 1)), (int)(hist_h - histScale2 * histAmTallest[i - 1])),
			Point((int)(bin_w*(i)), (int)(hist_h - histScale2 * histAmTallest[i])),
			Scalar(55, 55, 255), 1, 8, 0);
		//		}
	}
	stringstream ss;
	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
	//	cv::Scalar(255, 255, 255), -1);
	ss << histMax << " " << histMax2 << " " << maxIdx << " " << maxIdx2 << " " << histAverage << " " << histVar << " " << histAverage2 << " " << histVar2;
	string frameNumberString = ss.str();
	putText(HSVI10mage, frameNumberString.c_str(), cv::Point(15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	imshow(mWindowHistName, HSVI10mage);
}

void adjustableDisplay::imageSectionStats(Mat srce, int *stats, int cx, int cy, int sectionSize) {
	if (sectionSize >= isolimit) sectionSize = isolimit;
	int srceChannels = srce.channels();
	for (int idxStats = 0; idxStats < 256; idxStats++)
		stats[idxStats] =  0;
	for (int y = cy - sectionSize; y < srce.rows && y < cy + sectionSize; y++) {
		uchar *ps = srce.ptr<uchar>(y);
		for (int x = cx - sectionSize; x < srce.cols && x < cx + sectionSize; x++) {
			for (int c = 0; c < srceChannels; c++) {
				int myVal = ps[(x)* srceChannels + c];

				stats[myVal]++;
			}
		}
	}
}
// temp showHist - showing stats of ave
// looks a promising way to determine whether the image contains extractable data
// stdDev low and +3Ave low -> nothing
// Turning product of the two values, 48, 65, 75, 110, 120, 180
// fft product 14, 220, 30, 180, 190, 180
void adjustableDisplay::showHistOfAve(Mat bwImage, int scope) {
	int hist[256];
	imageHistOfAve(bwImage, &hist[0], scope);
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = 2;
	Mat HSVI10mage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	int maxIdx = 0;
	float histMax = 0;
	float histAverage = 0;
	float histVar = 0;
	float histTotal = 0;
	float histAverage2 = 0;
	float histVar2 = 0;
	float histTotal2 = 0;
	String histStr;
	char oneStr[32];
	for (int i = 0; i < 256; i++)
	{
		sprintf(oneStr, "%d, ", hist[i]);
		histStr += oneStr;
		histTotal += hist[i];
		histAverage += hist[i] * i;
		if (hist[i] > histMax) {
			histMax = (float)(hist[i]);
			maxIdx = i;
		}
	}
	histAverage /= histTotal;
	for (int i = 0; i < 256; i++)
	{
		float tmp;
		tmp = (i - histAverage);
		histVar += hist[i] * tmp * tmp;
	}
	histVar /= histTotal;
	histVar = sqrt(histVar);
	int trimLimit = (int)(histAverage + 3 * histVar);
	for (int i = 1; i < 256; i++)
	{
		if (i > trimLimit) {
			histTotal2 += hist[i];
			histAverage2 += hist[i] * i;
		}
	}
	histAverage2 /= histTotal2;
	for (int i = 0; i < 256; i++)
	{
		if (i > trimLimit) {
			float tmp;
			tmp = (i - histAverage2);
			histVar2 += hist[i] * tmp * tmp;
		}
	}
	histVar2 /= histTotal2;
	histVar2 = sqrt(histVar2);
	float histScale = hist_h / histMax;
//	histScale = hist_h / (histMax / 10); // hist[(int)(histAverage + histVar)];
	for (int i = 1; i < 256; i++)
	{
		//		if (histIsolation[i] < hist_h) {
		line(HSVI10mage, Point((int)(bin_w*(i - 1)), (int)(hist_h - histScale * hist[i - 1])),
			Point((int)(bin_w*(i)), (int)(hist_h - histScale * hist[i])),
			Scalar(255, 55, 55), 1, 8, 0);
		//		}
	}
	stringstream ss;
	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
	//	cv::Scalar(255, 255, 255), -1);
	ss << histMax << " " << hist[(int)(histAverage + histVar)] << " " << maxIdx << " " << histAverage << " " << histVar << " " << histTotal2 << " " << histAverage2 << " " << histVar2;
	string frameNumberString = ss.str();
	putText(HSVI10mage, frameNumberString.c_str(), cv::Point(15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	imshow(mWindowHistName, HSVI10mage);

}

void adjustableDisplay::showSectionStats(int x, int y, Mat bwImage, int scope) {
	int hist[256];
	imageSectionStats(bwImage, &hist[0], x, y, scope);
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = 2;
	Mat HSVI10mage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	int maxIdx = 0;
	float histMax = 0;
	float histAverage = 0;
	float histVar = 0;
	float histTotal = 0;
	float histAverage2 = 0;
	float histVar2 = 0;
	float histTotal2 = 0;
	for (int i = 1; i < 256; i++)
	{
		histTotal += hist[i];
		histAverage += hist[i] * i;
		if (hist[i] > histMax) {
			histMax = (float)(hist[i]);
			maxIdx = i;
		}
	}
	histAverage /= histTotal;
	for (int i = 1; i < 256; i++)
	{
		float tmp;
		tmp = (i - histAverage);
		histVar += hist[i] * tmp * tmp;
	}
	histVar /= histTotal;
	histVar = sqrt(histVar);
	int trimLimit = (int)(histAverage + histVar);
	for (int i = 1; i < 256; i++)
	{
		if (i > trimLimit) {
			histTotal2 += hist[i];
			histAverage2 += hist[i] * i;
		}
	}
	histAverage2 /= histTotal2;
	for (int i = 0; i < 256; i++)
	{
		if (i > trimLimit) {
			float tmp;
			tmp = (i - histAverage2);
			histVar2 += hist[i] * tmp * tmp;
		}
	}
	histVar2 /= histTotal2;
	histVar2 = sqrt(histVar2);
	float histScale = hist_h / histMax;
	for (int i = 1; i < 256; i++)
	{
		//		if (histIsolation[i] < hist_h) {
		line(HSVI10mage, Point((int)(bin_w*(i - 1)), (int)(hist_h - histScale * hist[i - 1])),
			Point((int)(bin_w*(i)), (int)(hist_h - histScale * hist[i])),
			Scalar(255, 55, 55), 1, 8, 0);
		//		}
	}
	stringstream ss;
	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
	//	cv::Scalar(255, 255, 255), -1);
	ss << histMax << " " << maxIdx << " " << histAverage << " " << histVar << " " << histTotal2  << " " << histAverage2 << " " << histVar2;
	string frameNumberString = ss.str();
	putText(HSVI10mage, frameNumberString.c_str(), cv::Point(15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	imshow(mWindowStatsName, HSVI10mage);

}
void adjustableDisplay::showGraph(Mat canvas, int *l0, int len, float scaleFactor, Scalar colour) {
	float bin_w = (float)(canvas.cols);
	bin_w /= len;
	float bin_h = (float)(canvas.rows);
	for (int i = 1; i < len; i++)
	{
		//		if (histIsolation[i] < hist_h) {
		line(canvas, Point((int)(bin_w*(i - 1)), (int)(canvas.rows - canvas.rows * scaleFactor * l0[i - 1])),
			Point((int)(bin_w*(i)), (int)(canvas.rows - canvas.rows * scaleFactor * l0[i])),
			colour, 1, 8, 0);
		//		}
	}
}
int adjustableDisplay::arrayMax(int *l0, int len) {
	int lclScaleFactor = 1;
	for (int i = 0; i < len; i++)
	{
		if (l0[i] > lclScaleFactor)
			lclScaleFactor = l0[i];
	}
	return lclScaleFactor;

}
void adjustableDisplay::showGraph(int *l0, int *l1, int *l2, int len) {
	float scaleFactor = (float)(arrayMax(l0, len));
	float tmpMax = (float)(arrayMax(l1, len));
	if (tmpMax > scaleFactor)
		scaleFactor = tmpMax;
	tmpMax = (float)(arrayMax(l2, len));
	if (tmpMax > scaleFactor)
		scaleFactor = tmpMax;
	scaleFactor = 1 / scaleFactor;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = 2;
	Mat HSVI10mage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	showGraph(HSVI10mage,l0, len, scaleFactor, Scalar(255,0,0));
	showGraph(HSVI10mage,l1, len, scaleFactor, Scalar(0, 255, 0));
	showGraph(HSVI10mage,l2, len, scaleFactor, Scalar(0, 0, 255));
	imshow(mWindowHistName, HSVI10mage);
}
void adjustableDisplay::thresholdHist(Mat inputM, int scope) {
		int histSize = 256;
		float range[] = { 1, 256 };
		const float* histRange = { range };
		bool uniform = true; bool accumulate = false;
//		calcHist(&bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		int cummulative[256];
		int l1[256];
		int l2[256];
		int runningSum = 0;
		if (!mHistogramComputed) {
			vector<Mat> bgr_planes;
	//		split(inputM.rowRange(0, 16).colRange(0,8), bgr_planes);
			split(inputM, bgr_planes);
			Mat hist;
			Mat tmpAdjustedImage;
			for (int i = 0; i < histSize - 3; i += 4)
			{
				tmpAdjustedImage = inputM.clone();
				ml1[i] = ml1[i + 1] = ml1[i + 2] = ml1[i + 3] = cvUtilities::imageThreshold(tmpAdjustedImage, i, 255);
				//				l2[0] = threshold(tmpAdjustedImage, tmpAdjustedImage, i, 255, 0); // threshold_binary = 0
				//				split(tmpAdjustedImage, bgr_planes);
				//				Mat hist;
				//				calcHist(&bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
				//				float oneVal = hist.at<float>(254);
				//				l1[i] = oneVal;
				cvUtilities::Erosion(tmpAdjustedImage, tmpAdjustedImage, 0, 1);
				cvUtilities::Dilation(tmpAdjustedImage, tmpAdjustedImage, 0, 1);
				ml2[i] = ml2[i + 1] = ml2[i + 2] = ml2[i + 3] = cvUtilities::imageThreshold(tmpAdjustedImage, 255, 255);
				//				split(tmpAdjustedImage, bgr_planes);
				//				calcHist(&bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
				//				oneVal = hist.at<float>(254);
				//				l2[i] = oneVal;
			}
			mHistogramComputed = true;
		}
		Mat tmpAdjustedImage;
		for (int i = 0; i < histSize; i++)
		{
			if (i <= scope) {
				cummulative[i] = 0;
				l1[i] = 0;
				l2[i] = 0;

			}
			else {
				l1[i] = ml1[i];
				l2[i] = ml2[i ];
				cummulative[i] = (ml1[i] - ml2[i]) * 10;

			}
		}
		showGraph(&cummulative[0], &l1[0], &l2[0], histSize);


}


void adjustableDisplay::showHistFlex(Mat bwImage, int scope) {
	//////showHist2ndTallest(bwImage, scope);
	//showHistOfAve( bwImage,  scope);
	////showHistTallestNeighbour(bwImage, scope);
	thresholdHist( bwImage,  scope);
}


