#define _CRT_SECURE_NO_WARNINGS

#include "cvUtilities.h"
#include "depthDisplay.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "CVcommon.h"
#include "csvFile.h"
using namespace std;
using namespace System;

depthDisplay::depthDisplay(char *windowName, float sizeDivision)
	: mWindowName(nullptr)
{
	mWindowName = new char[strlen(windowName) + 1];
	strcpy(mWindowName, windowName);
	mDispDivision = sizeDivision;
	mZoom = 1;

	mContrast = 1;
	mBaseLevel = 0;
	mThreshold = 0;
	mErodeQty = -1;
	mDilateQty = -1;
	mContourMax = 100;
	mContourMin = 40;
	mIsolation = 0;
	mBlur = 0;
	mTrackVal = 0;
	mSliderName = "Slider";

	mMenuStr[MENU_TOP] = "Top";
	mMenuStr[MENU_BOTTOM] = "Bottom";
	mMenuStr[MENU_PROBE_0] = "Pin 0";
	mMenuStr[MENU_PROBE_1] = "Pin 1";
	mMenuStr[MENU_PROBE_2] = "Pin 2";
	mMenuStr[MENU_PROBE_3] = "Pin 3";
	mMenuStr[MENU_PROBE_4] = "Pin 4";
	mMenuStr[MENU_PROBE_5] = "Pin 5";
	mMenuStr[MENU_PROBE_6] = "Pin 6";
	mMenuStr[MENU_ZOOM_CENTRE] = "Z Centre";
	mMenuStr[MENU_USE_CALIBRATION] = "Calibration";
	mMenuStr[MENU_SAVE] = "Save";
	mMenuSelected[MENU_TOP] = false;
	mMenuSelected[MENU_BOTTOM] = false;
	mMenuSelected[MENU_PROBE_0] = false;
	mMenuSelected[MENU_PROBE_1] = false;
	mMenuSelected[MENU_PROBE_2] = false;
	mMenuSelected[MENU_PROBE_3] = false;
	mMenuSelected[MENU_PROBE_4] = false;
	mMenuSelected[MENU_PROBE_5] = false;
	mMenuSelected[MENU_PROBE_6] = false;
	mMenuSelected[MENU_ZOOM_CENTRE] = false;
	mMenuSelected[MENU_USE_CALIBRATION] = mUseCalibration;
	mMenuSelected[MENU_SAVE] = false;
	mMenuSelection = MENU_NONE;
}
depthDisplay::~depthDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;
}

void depthDisplay::adjustDisplay() {

}

void depthDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(mDisplayImage, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void depthDisplay::showMetric(int lineNo, int SlotNo, char *txt) {
	int x = SlotNo - 3;
	x *= MENU_METRIC_WIDTH;
	x = (mDispCols / 2) + x;
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	y += MENU_METRIC_BASE;
	y = mDispRows - y;
	putText(mDisplayImage, txt, cv::Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));

}

void depthDisplay::showCurve(float *candidates, bool flattened) {
	float curveY = (candidates[0]);
	Scalar clr = (flattened) ? cv::Scalar(255, 255, 255) : cv::Scalar(0, 255, 0);
	curveY = MENU_CURVE_ZERO - MENU_CURVE_MM * curveY;
	for (int pIdx = 1; pIdx < 7; pIdx++) {
		int x = pIdx - 3;
		x *= MENU_METRIC_WIDTH;
		x = (mDispCols / 2) + x;
		float curveY1 = (candidates[pIdx]);
		curveY1 = MENU_CURVE_ZERO - MENU_CURVE_MM * curveY1;
		circle(mDisplayImage,Point(x - MENU_METRIC_WIDTH, mDispRows - curveY), 4 , clr , -1);
		line(mDisplayImage, Point(x - MENU_METRIC_WIDTH, mDispRows - curveY), Point(x, mDispRows - curveY1), clr, 2);
		circle(mDisplayImage,Point(x, mDispRows - curveY1), 4 , clr, -1);
		curveY = curveY1;
	}
}

void depthDisplay::showMenu() {
	mAdjustedImage.copyTo(mDisplayImage);
	char menuLine[64];
	int y = 0;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_TOP], mMenuSelected[MENU_TOP] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_BOTTOM], mMenuSelected[MENU_BOTTOM] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "%s %c %f", mMenuStr[MENU_PROBE_0 + pIdx], mMenuSelected[MENU_PROBE_0 + pIdx] ? '*' : ' ', mProbeMm[pIdx]);
		showMenuLine(y, menuLine);
		y++;

	}
	sprintf(menuLine, "%s %c", mMenuStr[MENU_ZOOM_CENTRE], mMenuSelected[MENU_ZOOM_CENTRE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_USE_CALIBRATION], mUseCalibration ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	sprintf(menuLine, "%s %c", mMenuStr[MENU_SAVE], mMenuSelected[MENU_SAVE] ? '*' : ' ');
	showMenuLine(y, menuLine);
	y++;
	showMetric(0, -1, "Raw");
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "% 0.3f", mProbeMm[pIdx]);
		showMetric(0, pIdx, menuLine);
	}
	float rebased[7];
	rebase(&mProbeMm[0], &rebased[0]);
	int dispIdx = 1;
	showMetric(dispIdx, -1, "Rebased");
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "% 0.3f", rebased[pIdx]);
		showMetric(dispIdx, pIdx, menuLine);
	}
	float flat[7];
	float errorval = flatten(&rebased[0], &flat[0]);
	errorval = calcRegression(&flat[0]);
	dispIdx++;
	showMetric(dispIdx, -1, "Flat");
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "% 0.3f", flat[pIdx]);
		showMetric(dispIdx, pIdx, menuLine);
	}
	sprintf(menuLine, "% 0.3f", errorval);
	showMetric(dispIdx, 7, menuLine);
	showCurve(&rebased[0], false);
	showCurve(&flat[0], true);
	float tmpErr[7];
	errorval = calcFacteted(&flat[0], &tmpErr[0]);
	errorval = calcRegression(&tmpErr[0]);
	dispIdx++;
	showMetric(dispIdx, -1, "Faceted");
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "% 0.3f", tmpErr[pIdx]);
		showMetric(dispIdx, pIdx, menuLine);
	}
	sprintf(menuLine, "% 0.3f", errorval);
	showMetric(dispIdx, 7, menuLine);
	errorval = calcCurved(&flat[0], &tmpErr[0]);
	errorval = calcRegression(&tmpErr[0]);
	dispIdx++;
	showMetric(dispIdx, -1, "Curved");
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "% 0.3f", tmpErr[pIdx]);
		showMetric(dispIdx, pIdx, menuLine);
	}
	sprintf(menuLine, "% 0.3f", errorval);
	showMetric(dispIdx, 7, menuLine);
	errorval = calcDomed(&rebased[0], &tmpErr[0]);
	dispIdx++;
	showMetric(dispIdx, -1, "Domed");
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		sprintf(menuLine, "% 0.3f", tmpErr[pIdx]);
		showMetric(dispIdx, pIdx, menuLine);
	}
	sprintf(menuLine, "% 0.3f", errorval);
	showMetric(dispIdx, 7, menuLine);
	imshow(mWindowName, mDisplayImage);
}



void depthDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	depthDisplay *pAdjustableDisplay = (depthDisplay *)userdata;
	pAdjustableDisplay->mouseCallback(event, x, y, flags);
}
void depthDisplay::trackCallback(int val, void* userdata) {
	depthDisplay *pAdjustableDisplay = (depthDisplay *)userdata;
	pAdjustableDisplay->trackCallback(val);
}

void depthDisplay::clearAndSetMenu(int menuSelection) {
	if (mMenuSelection != MENU_NONE) {
		mMenuSelected[mMenuSelection] = false;
	}
	mMenuSelection = menuSelection;
	if (menuSelection != MENU_NONE) {
		mMenuSelected[menuSelection] = true;
	}
}
void depthDisplay::mouseCallback(int event, int x, int y, int flags) {
	if (x < 0) x = 0;
	if (x >= mCols) x = mCols - 1;
	if (y < 0) y = 0;
	if (y >= mRows) y = mRows - 1;

	if (event == EVENT_LBUTTONDOWN)
	{
		int tmpMenu = menuSelected(x, y);
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_TOP:
				clearAndSetMenu(tmpMenu);
				break;
			case MENU_BOTTOM:
				clearAndSetMenu(tmpMenu);
				break;
			case MENU_ZOOM_CENTRE:
				clearAndSetMenu(tmpMenu);
				break;
			case MENU_USE_CALIBRATION:
				mUseCalibration = !mUseCalibration;
				break;
			case MENU_SAVE:
				clearAndSetMenu(tmpMenu);
				saveConfig();
				break;
			case MENU_PROBE_0:
			case MENU_PROBE_1:
			case MENU_PROBE_2:
			case MENU_PROBE_3:
			case MENU_PROBE_4:
			case MENU_PROBE_5:
			case MENU_PROBE_6:
				clearAndSetMenu(tmpMenu);
				break;
			}
			adjustImage();
			showMenu();
		}
		else {
			saveClickPosition(x, y, mMenuSelection);
			adjustImage();
			showMenu();
		}
	}
}
void depthDisplay::calcHeights() {
	float deltaX = (mTopLine.x - mBottomLine.x);
	float deltaY = (mTopLine.y - mBottomLine.y);
	float scaleMm = sqrt(deltaX * deltaX + deltaY * deltaY) / 10;
	float lowestMm = 1000000;
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		float tmpY = mProbes[pIdx].y;
		float tmpX = tmpY * (deltaX / deltaY);
		mProbeMm[pIdx] = sqrt(tmpY * tmpY + tmpX * tmpX);
		if (mProbeMm[pIdx] < lowestMm)
			lowestMm = mProbeMm[pIdx];
	}
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		mProbeMm[pIdx] -= lowestMm;
		mProbeMm[pIdx] /= scaleMm;
	}
}
#define PROBE_WIDTH_HALF 2.25f
#define ONE_PROBE_WIDTH ((2 * PROBE_WIDTH_HALF) / 6)

float depthDisplay::calcFacteted(float *candidates, float *errors) {
	float idealDepth = PROBE_WIDTH_HALF * Math::Tan(29.5f * Math::PI / 180); // NB check this!
	float error = 0;
	float ideal[7];
	ideal[0] = candidates[0];
	ideal[3] = idealDepth;
	ideal[6] = candidates[6];
	//ideal[1] = ideal[5] = idealDepth / 3;
	//ideal[2] = ideal[4] = idealDepth * 2 / 3;

	//for (int pIdx = 0; pIdx < 7; pIdx++) {
	//	errors[pIdx] = candidates[pIdx] - ideal[pIdx];
	//	error += errors[pIdx] * errors[pIdx];
	//}
	int deepestIdx = 2;
	float deepest = candidates[2];
	for (int pIdx = 3; pIdx < 5; pIdx++) {
		if (candidates[pIdx] > deepest) {
			deepest = candidates[pIdx];
			deepestIdx = pIdx;
		}
	}
	int highestEnd = (candidates[0] < candidates[6]) ? 0 : 6;
	float maxDepth = deepest - candidates[highestEnd];
	if (maxDepth < MIN_FACET_DEPTH) maxDepth = MIN_FACET_DEPTH;
	deepest = maxDepth + candidates[highestEnd];
	ideal[deepestIdx] = deepest;
	for (int pIdx = deepestIdx + 1; pIdx < 6; pIdx++) {
		ideal[pIdx] = candidates[6] + (deepest - candidates[6]) * (float)(6 - pIdx) / (float)(6 - deepestIdx);
	}
	for (int pIdx = 1; pIdx < deepestIdx; pIdx++) {
		ideal[pIdx] = candidates[0] + (deepest - candidates[0]) * (float)pIdx / (float)deepestIdx;
	}
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		errors[pIdx] = candidates[pIdx] - ideal[pIdx];
		error += errors[pIdx] * errors[pIdx];
	}
	return Math::Sqrt(error);
}
float depthDisplay::calcCurved(float *candidates, float *errors) {
	float error = 0;
	float centreDepth = candidates[3];
	if (centreDepth < MIN_CURVED_DEPTH) centreDepth = MIN_CURVED_DEPTH;
	if (centreDepth > MAX_CURVED_DEPTH) centreDepth = MAX_CURVED_DEPTH;
	float radius = (PROBE_WIDTH_HALF * PROBE_WIDTH_HALF + centreDepth * centreDepth) / (2 * centreDepth);
	float ideal[7];
	ideal[0] = candidates[0];
	ideal[3] = centreDepth;
	ideal[6] = candidates[6];
	ideal[1] = ideal[5] = centreDepth - (radius - Math::Cos(Math::Asin(ONE_PROBE_WIDTH * 2 / (radius))) * radius);
	ideal[2] = ideal[4] = centreDepth - (radius - Math::Cos(Math::Asin(ONE_PROBE_WIDTH / (radius))) * radius);
	//int deepestIdx = 0;
	//float deepest = candidates[0];
	//for (int pIdx = 1; pIdx < 7; pIdx++) {
	//	if (candidates[pIdx] > deepest) {
	//		deepest = candidates[pIdx];
	//		deepestIdx = pIdx;
	//	}
	//}
	//if (deepestIdx != 3) {
	//	if (deepestIdx == 2) {
	//		centreDepth = candidates[2] - candidates[0];
	//		//if (centreDepth < MIN_CURVED_DEPTH) centreDepth = MIN_CURVED_DEPTH;
	//		//if (centreDepth > MAX_CURVED_DEPTH) centreDepth = MAX_CURVED_DEPTH;
	//		float radius0 = (4 * ONE_PROBE_WIDTH * ONE_PROBE_WIDTH + centreDepth * centreDepth) / (2 * centreDepth);
	//		centreDepth = candidates[2] - candidates[6];
	//		if (centreDepth < MIN_CURVED_DEPTH) centreDepth = MIN_CURVED_DEPTH;
	//		if (centreDepth > MAX_CURVED_DEPTH) centreDepth = MAX_CURVED_DEPTH;
	//		float radius1 = (16 * ONE_PROBE_WIDTH * ONE_PROBE_WIDTH + centreDepth * centreDepth) / (2 * centreDepth);
	//		radius = (radius1 + radius0) / 2;
	//		float ideal[7];
	//		ideal[0] = candidates[0];
	//		ideal[2] = candidates[2];
	//		ideal[6] = candidates[6];
	//		ideal[1]  = ideal[3]  = centreDepth - (radius - Math::Cos(Math::Asin(ONE_PROBE_WIDTH/radius)) * radius);
	//		ideal[4]  = centreDepth - (radius - Math::Cos(Math::Asin((2 * PROBE_WIDTH_HALF) / (radius))) * radius);
	//		ideal[5]  = centreDepth - (radius - Math::Cos(Math::Asin((3 * PROBE_WIDTH_HALF) / (radius))) * radius);
	//	}
	//	else {
	//		centreDepth = candidates[4] - candidates[6];
	//		if (centreDepth < MIN_CURVED_DEPTH) centreDepth = MIN_CURVED_DEPTH;
	//		if (centreDepth > MAX_CURVED_DEPTH) centreDepth = MAX_CURVED_DEPTH;
	//		float radius0 = (4 * ONE_PROBE_WIDTH * ONE_PROBE_WIDTH + centreDepth * centreDepth) / (2 * centreDepth);
	//		centreDepth = candidates[4] - candidates[0];
	//		if (centreDepth < MIN_CURVED_DEPTH) centreDepth = MIN_CURVED_DEPTH;
	//		if (centreDepth > MAX_CURVED_DEPTH) centreDepth = MAX_CURVED_DEPTH;
	//		float radius1 = (16 * ONE_PROBE_WIDTH * ONE_PROBE_WIDTH + centreDepth * centreDepth) / (2 * centreDepth);
	//		radius = (radius1 + radius0) / 2;
	//		float ideal[7];
	//		ideal[0] = candidates[0];
	//		ideal[4] = candidates[4];
	//		ideal[6] = candidates[6];
	//		ideal[2] = ideal[5] = centreDepth - (radius - Math::Cos(Math::Asin(ONE_PROBE_WIDTH/radius)) * radius);
	//		ideal[1]  = centreDepth - (radius - Math::Cos(Math::Asin((3 * PROBE_WIDTH_HALF) / (4 * radius))) * radius);

	//	}

	//}

	
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		errors[pIdx] = candidates[pIdx] - ideal[pIdx];
		error += errors[pIdx] * errors[pIdx];
	}
	return Math::Sqrt( error);

}
float depthDisplay::calcDomed(float *candidates, float *errors) {
	float error = 0;
	float lowest = candidates[0];
	float highest = candidates[0];
	//float centreDepth = candidates[3];
	//if (centreDepth < MIN_DOMED_DEPTH) centreDepth = MIN_DOMED_DEPTH;
	//if (centreDepth > MAX_DOMED_DEPTH) centreDepth = MAX_DOMED_DEPTH;
	//float radius = (PROBE_WIDTH_HALF * PROBE_WIDTH_HALF + centreDepth * centreDepth) / (2 * centreDepth);
	//float ideal[7];
	//ideal[0] = candidates[0];
	//ideal[3] = centreDepth;
	//ideal[6] = candidates[6];
	//ideal[1] = ideal[5] = centreDepth - (radius - Math::Cos(Math::Asin(ONE_PROBE_WIDTH * 2 / (radius))) * radius);
	//ideal[2] = ideal[4] = centreDepth - (radius - Math::Cos(Math::Asin(ONE_PROBE_WIDTH / (radius))) * radius);
	
	//for (int pIdx = 0; pIdx < 7; pIdx++) {
	//	errors[pIdx] = candidates[pIdx] - ideal[pIdx];
	//	error += errors[pIdx] * errors[pIdx];
	//}
	for (int pIdx = 1; pIdx < 7; pIdx++) {
		if (lowest > candidates[pIdx]) lowest = candidates[pIdx];
		if (highest < candidates[pIdx]) highest = candidates[pIdx];
	}
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		errors[pIdx] = candidates[pIdx] - highest;
	}
	return highest - lowest;

}

float depthDisplay::flatten(float *candidates, float *flat) {
	float error = 0;
	float corrections[7];
	corrections[0] = candidates[0];
	corrections[6] = candidates[6];
	for (int pIdx = 1; pIdx < 6; pIdx++) {
		corrections[pIdx] = ((float)pIdx * corrections[6] + (float)(6 - pIdx) * corrections[0]) / 6;
	}

	for (int pIdx = 0; pIdx < 7; pIdx++) {
		flat[pIdx] = candidates[pIdx] - corrections[pIdx];
		error += flat[pIdx] * flat[pIdx];
	}
	return Math::Sqrt(error);
}
float depthDisplay::calcRegression(float *errors) {
	float meanY = 0;
	float sigmaDeltaYSq = 0;

	float meanX = 3;
	float sigmaDeltaXSq = 28;
	float error = 0;

	for (int pIdx = 0; pIdx < 7; pIdx++) {
		meanY += errors[pIdx];
	}
	meanY /= 7;
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		float deltaY = errors[pIdx] - meanY;
		sigmaDeltaYSq += deltaY * (pIdx - meanX) ;
	}
	float m = sigmaDeltaYSq / sigmaDeltaXSq;
	float b = meanY - m * meanX;
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		float deltaY = errors[pIdx] - (b + pIdx * m);
		error += deltaY * deltaY;
	}
	return error;
}
void depthDisplay::rebase(float *candidates, float *calibrated) {
	float corrections[7];
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		if (mUseCalibration) {
			calibrated[pIdx] = candidates[pIdx] - mCalibMm[pIdx];
		}
		else {
			calibrated[pIdx] = candidates[pIdx];
		}
	}
}

void depthDisplay::saveClickPosition(int x, int y, int menuSelection) {
	switch (menuSelection) {
	case MENU_TOP:
		mTopLine = unResizePoint(Point(x, y));
		clearAndSetMenu(MENU_BOTTOM);
		break;
	case MENU_BOTTOM:
		mBottomLine = unResizePoint(Point(x, y));
		clearAndSetMenu(MENU_PROBE_0);
		break;
	case MENU_ZOOM_CENTRE:
		mZoomCentre = unResizePoint( Point(x, y));
		clearAndSetMenu(MENU_NONE);
		break;
	case MENU_SAVE:
		//saveData();
		clearAndSetMenu(MENU_NONE);
		break;
	case MENU_PROBE_0:
	case MENU_PROBE_1:
	case MENU_PROBE_2:
	case MENU_PROBE_3:
	case MENU_PROBE_4:
	case MENU_PROBE_5:
		mProbes[menuSelection - MENU_PROBE_0] = unResizePoint(Point(x, y));
		clearAndSetMenu(menuSelection + 1);
		break;
	case MENU_PROBE_6:
		mProbes[menuSelection - MENU_PROBE_0] = unResizePoint(Point(x, y));
		clearAndSetMenu(MENU_NONE);
		break;
	}
	calcHeights();
}


void depthDisplay::trackCallback(int val) {
	mZoom = val + 10;
	mZoom /= 10;
	if (mZoom < 1) mZoom = 1;
	adjustImage();
	showMenu();
}

int depthDisplay::menuSelected(int x, int y) {
	int retv = MENU_NONE;
	int maxy = MENU_QTY * MENU_PANEL_LINE_HEIGHT;
	if (x <= MENU_PANEL_WIDTH && y > 0 && y <= maxy) {
		retv = y / MENU_PANEL_LINE_HEIGHT;
	}
	return retv;
}




void depthDisplay::resizeFrame(Mat* inputMat, Mat* outputMat) {
	Mat tmp = inputMat->clone();
	Mat tmpOut = Mat::zeros(mDispRows, mDispCols, inputMat->type());
	int minCol = 0;
	int maxCol = (mDispCols * mDispDivision);
	int minRow = 0;
	int maxRow = (mDispRows * mDispDivision);
	int destCols = mDispCols;
	int destRows = mDispRows;
	resize(tmp, tmp, cv::Size(destCols, destRows), 0, 0, CV_INTER_LINEAR);
	tmp.copyTo(tmpOut.rowRange(0, tmp.rows).colRange(0, tmp.cols));
	*outputMat = tmpOut;
}
void depthDisplay::resizeFrame(Mat* inputMat, Mat* outputMat, float dispDivision, float zoom) {
	Mat tmp = inputMat->clone();
	Mat tmpOut = Mat::zeros(mDispRows, mDispCols, inputMat->type());
	int imageH = mRows / mZoom;
	int imageW = mCols / mZoom; // how much of the original is to be displayed
	int minCol = mZoomCentre.x - imageW / 2;
	int maxCol = mZoomCentre.x + imageW / 2;
	int minRow = mZoomCentre.y - imageH / 2;
	int maxRow = mZoomCentre.y + imageH / 2;
	//if (minCol < 0) minCol = 0;
	//if (maxCol >= imageW) maxCol = imageW - 1;
	//if (minRow < 0) minRow = 0;
	//if (maxRow >= imageH) maxRow = imageH - 1;
	int destCols = mDispCols;
	int destRows = mDispRows;
	resize(tmp.rowRange(minRow, maxRow).colRange(minCol, maxCol), tmp, cv::Size(destCols, destRows), 0, 0, CV_INTER_LINEAR);
	//tmp.copyTo(tmpOut.rowRange(0, tmp.rows).colRange(0, tmp.cols));
	tmp.copyTo(tmpOut);
	*outputMat = tmpOut;
}


void depthDisplay::showImg(Mat img) {
	mOrgImage = img.clone();
	mRows = img.rows;
	mCols = img.cols;
	mZoomCentre = Point(mCols / 2, mRows / 2);
	mDispCols = mCols / mDispDivision;
	mDispRows = mRows / mDispDivision;
	loadConfig();
	adjustImage();
	imshow(mWindowName, mAdjustedImage);
	setMouseCallback(mWindowName, depthDisplay::mouseCallback, this);
	createTrackbar(mSliderName, mWindowName, &mTrackVal, 100, depthDisplay::trackCallback, this);
	showMenu();
}

void depthDisplay::setConfigFile(char *configFilename, char *calibrationConfigFilename, bool useCalibration) {
	mConfigFileName = new char[strlen(configFilename) + 1];
	strcpy(mConfigFileName, configFilename);
	std::ifstream str;
	str.open(calibrationConfigFilename);
	std::vector<std::string>   lineArray = csvFile::getNextLineAndSplitIntoTokensIgnoreComment(str);
	int idx = 0;
	if (lineArray.size() >= 7) {
		for (int pIdx = 0; pIdx < 7; pIdx++) {
			mCalibMm[pIdx] = std::stof(lineArray[pIdx]);
		}
	}
	str.close();
	mUseCalibration = useCalibration;
}

void depthDisplay::saveConfig() {
	ofstream locFile(mConfigFileName, ios::out);
	locFile << "# zommCentre, Top, Bottom" << "\n";
	locFile << "# N * prob.x, probe.y, probeMm, rebasedMm" << "\n";
	locFile << "# probeMm, rebasedMm, Flat + err, Faceted + err, Curved + err, Domed + err" << "\n";
	locFile << mZoomCentre.x << "," << mZoomCentre.y << "," << mTopLine.x << "," << mTopLine.y << ", " << mBottomLine.x << ", " << mBottomLine.y << "\n";
	for (int idx = 0; idx < 7; idx++) {
		locFile << mProbes[idx].x << "," << mProbes[idx].y << "," << mProbeMm[idx] << "," << mProbeMm[idx] - mCalibMm[idx] << "\n";
	}
	locFile << std::setprecision(3) << "\n";
	for (int idx = 0; idx < 6; idx++) {
		locFile << mProbeMm[idx] << ",";
	}
	locFile << mProbeMm[6] << "\n";
	float rebased[7];
	rebase(&mProbeMm[0], &rebased[0]);
	for (int idx = 0; idx < 6; idx++) {
		locFile << rebased[idx] << ",";
	}
	locFile << rebased[6] << "\n";
	float flat[7];
	float errorval = flatten(&rebased[0], &flat[0]);
	for (int idx = 0; idx < 6; idx++) {
		locFile << flat[idx] << ",";
	}
	locFile << flat[6] << ",  " << errorval << "\n";
	float tmpErr[7];
	errorval = calcFacteted(&flat[0], &tmpErr[0]);
	for (int idx = 0; idx < 6; idx++) {
		locFile << tmpErr[idx] << ",";
	}
	locFile << tmpErr[6] << ",  " << errorval << "\n";
	errorval = calcCurved(&flat[0], &tmpErr[0]);
	for (int idx = 0; idx < 6; idx++) {
		locFile << tmpErr[idx] << ",";
	}
	locFile << tmpErr[6] << ",  " << errorval << "\n";
	errorval = calcDomed(&rebased[0], &tmpErr[0]);
	for (int idx = 0; idx < 6; idx++) {
		locFile << tmpErr[idx] << ",";
	}
	locFile << tmpErr[6] << ",  " << errorval << "\n";
	locFile.close();
}
void depthDisplay::loadConfig() {
	std::ifstream str;
	str.open(mConfigFileName);
	std::vector<std::string>   lineArray = csvFile::getNextLineAndSplitIntoTokensIgnoreComment(str);
	int idx = 0;
	if (lineArray.size() >= 6) {
		mZoomCentre.x = std::stof(lineArray[idx++]);
		mZoomCentre.y = std::stof(lineArray[idx++]);
		mTopLine.x = std::stof(lineArray[idx++]);
		mTopLine.y = std::stof(lineArray[idx++]);
		mBottomLine.x = std::stof(lineArray[idx++]);
		mBottomLine.y = std::stof(lineArray[idx++]);
	}
	for (int pIdx = 0; pIdx < 7; pIdx++) {
		lineArray = csvFile::getNextLineAndSplitIntoTokensIgnoreComment(str);
		if (lineArray.size() >= 3) {
			idx = 0;
			mProbes[pIdx].x = std::stof(lineArray[idx++]);
			mProbes[pIdx].y = std::stof(lineArray[idx++]);
			mProbeMm[pIdx] = std::stof(lineArray[idx++]);
		}
	}
	str.close();
}
void depthDisplay::adjustImage() {
	mAdjustedImage = mOrgImage.clone();
	resizeFrame(&mAdjustedImage, &mAdjustedImage, mDispDivision, mZoom);
	drawCrosses(mAdjustedImage);
}

Point depthDisplay::resizePoint(Point2f inputLoc) {
	return resizePoint(inputLoc, mDispDivision, mZoom);
}
Point depthDisplay::unResizePoint(Point2f inputLoc) {
	return unResizePoint(inputLoc, mDispDivision, mZoom);
}

Point depthDisplay::resizePoint(Point2f inputLoc, float dispDivision, float zoom) {
	int centreOffsetX = inputLoc.x - mZoomCentre.x;
	int centreOffsetY = inputLoc.y - mZoomCentre.y;
	Point2f retv = Point2f(((centreOffsetX)) / (mDispDivision / zoom) + mDispCols / 2, ((centreOffsetY)) / (mDispDivision / zoom) + mDispRows / 2);
	return retv;
}
Point depthDisplay::unResizePoint(Point2f inputLoc, float dispDivision, float zoom) {
	int centreOffsetX = inputLoc.x - mDispCols / 2;
	int centreOffsetY = inputLoc.y - mDispRows / 2;
	Point retv = Point2f(((centreOffsetX * dispDivision / zoom) + mZoomCentre.x), ((centreOffsetY * dispDivision / zoom)) + mZoomCentre.y);
	return retv;
}

void depthDisplay::drawCross(Mat image, Point whereOnImage, char *name, bool plusNotTimes) {
	if (plusNotTimes) {
		line(image, Point(whereOnImage.x, whereOnImage.y - 8), Point(whereOnImage.x, whereOnImage.y + 8), Scalar(255, 255, 255), 1);
		line(image, Point(whereOnImage.x - 8, whereOnImage.y), Point(whereOnImage.x + 8, whereOnImage.y), Scalar(255, 255, 255), 1);
	}
	else {
		line(image, Point(whereOnImage.x - 8, whereOnImage.y - 8), Point(whereOnImage.x + 8, whereOnImage.y + 8), Scalar(255, 255, 255), 1);
		line(image, Point(whereOnImage.x - 8, whereOnImage.y + 8), Point(whereOnImage.x + 8, whereOnImage.y - 8), Scalar(255, 255, 255), 1);
	}
	putText(image, name, cv::Point(whereOnImage.x - 8, whereOnImage.y - 10),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
}
void depthDisplay::drawCrosses(Mat image) {
	drawCross(image, resizePoint(mTopLine), "ref Top", false);
	drawCross(image, resizePoint(mBottomLine), "ref Bottom", false);
	for (int idx = 0; idx < 7; idx++) {
		drawCross(image, resizePoint(mProbes[idx]), "", true);

	}
}


void depthDisplay::setDisplayParams(float contrast, int baseLevel, int erodeQty, int dilateQty, int threshold) {
	mContrast = contrast;
	mBaseLevel = baseLevel;
	mErodeQty = erodeQty;
	mDilateQty = dilateQty;
	mThreshold = threshold;
}
void depthDisplay::getDisplayParams(float *pContrast, int *pBaseLevel, int *pErodeQty, int *pDilateQty, int *pThreshold) {
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
void depthDisplay::imageIsolation(Mat srce, Mat *dest, int isolationLevel) {
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
void depthDisplay::imageIsolationTalestNeighbour(Mat srce, int *statsTalestNeighbour, int *statsAmTalest, int isolationLevel) {
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
void depthDisplay::image2ndTallest(Mat srce, int *stats2ndTalest, int isolationLevel) {
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

void depthDisplay::imageHistOfAve(Mat srce, int *stats, int isolationLevel) {
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

