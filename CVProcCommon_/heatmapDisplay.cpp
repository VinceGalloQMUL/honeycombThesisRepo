
#include "cvUtilities.h"
#include "heatmapDisplay.h"
#include <cmath>

extern void saveToClipboard(string str);

heatmapDisplay::heatmapDisplay(char *windowName)
	: mWindowName(nullptr)
{
	mWindowName = new char[strlen(windowName) + 1];
	mSliderName = "Slider";
	strcpy(mWindowName, windowName);
	mTrackVal = mGridVal = DEFAULT_GRID;
	mThresholdVal = DEFAULT_THRESHOLD;
	mContrastVal = DEFAULT_CONTRAST;
	mBlurVal = DEFAULT_BLUR;
	pHeatMap = nullptr;
	pAllLocations = nullptr;
	mGapAllowed = 0.5;
	mHaveImageAndData = false;
	mSliderName = "Slider";
	mMenuMode = topMenu;
	mMenuTitle[0] = "| Grid |";

	mMenuSelection = MENU_SETGRID;
	mMenuStr[MENU_SETGRID] = "Grid Size";
	mMenuSelected[MENU_SETGRID] = true;
	mMenuStr[MENU_BLUR] = "Blur";
	mMenuSelected[MENU_BLUR] = false;
	mMenuStr[MENU_THRESHOLD] = "Threshold";
	mMenuSelected[MENU_THRESHOLD] = false;
	mMenuStr[MENU_CONTRAST] = "Contrast";
	mMenuSelected[MENU_CONTRAST] = false;
	mMenuStr[MENU_SAVE] = "Save";
	mMenuSelected[MENU_SAVE] = false;
}
heatmapDisplay::~heatmapDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;

}

void heatmapDisplay::adjustDisplay() {

}

void heatmapDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(mDisplayImage, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void heatmapDisplay::menuQtyString(char *strBuf, int menuIdx, menuMode whichMenu) {
	switch (menuIdx) {
	default:
		*strBuf = '\0';
		break;
	case MENU_SETGRID:
		sprintf(strBuf, "%d", mGridVal);
		break;
	case MENU_THRESHOLD:
		sprintf(strBuf, "%d", mThresholdVal);
		break;
	case MENU_BLUR:
		sprintf(strBuf, "%d", mBlurVal);
		break;
	case MENU_CONTRAST:
		sprintf(strBuf, "%d", mContrastVal);
		break;

	}
}

void heatmapDisplay::showMenu(menuMode whichMenu) {
	mAdjustedImage.copyTo(mDisplayImage);
	char menuLine[64];
	char menuVal0[32];
	int y = 0;
	switch (whichMenu) {
	case topMenu:
		showMenuLine(0, mMenuTitle[0]);
		while (y < MENU_QTY) {
			menuQtyString(&menuVal0[0], y, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y], &menuVal0[0], mMenuSelected[y] ? '*' : ' ');
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	}
	imshow(mWindowName, mDisplayImage);
	createTrackbar(mSliderName, mWindowName, &mTrackVal, 360, heatmapDisplay::trackCallback, this);
	setMouseCallback(mWindowName, heatmapDisplay::mouseCallback, this);
}


void heatmapDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	heatmapDisplay *pHeatmapDisplay = (heatmapDisplay *)userdata;
	pHeatmapDisplay->mouseCallback(event, x, y, flags);
}
void heatmapDisplay::trackCallback(int val, void* userdata) {
	heatmapDisplay *pHeatmapDisplay = (heatmapDisplay *)userdata;
	pHeatmapDisplay->trackCallback(val);
}

void heatmapDisplay::clearAndSetMenu(int menuSelection, menuMode whichMenu) {
		if (mMenuSelection != MENU_NONE) {
			mMenuSelected[mMenuSelection] = false;
		}
		mMenuSelection = menuSelection;
		if (menuSelection != MENU_NONE) {
			mMenuSelected[menuSelection] = true;
		}
}
void heatmapDisplay::flipMenu(int menuSelection, menuMode whichMenu) {
	if (menuSelection != MENU_NONE) {
		if (mMenuSelected[menuSelection]) {
			mMenuSelected[menuSelection] = false;
			mMenuSelection = MENU_NONE;
		}
		else {
			clearAndSetMenu(menuSelection, whichMenu);
		}
	}
}
void heatmapDisplay::clearMenuSelections() {
	for (int idx = 0; idx < MENU_ALL_QTY; idx++) {
		mMenuSelected[idx] = false;
	}
}

void heatmapDisplay::mouseCallback(int event, int x, int y, int flags) {
	bool adviseOfChange = false;
	if (x < 0) x = 0;
	if (x >= mCols) x = mCols - 1;
	if (y < 0) y = 0;
	if (y >= mRows) y = mRows - 1;

	if (event == EVENT_LBUTTONDOWN)
	{
		switch (mMenuMode) {
		case topMenu:
			break;
		}
		int tmpMenu = menuSelected(x, y, mMenuMode);
		int tmpSliderVal = -1;
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_SETGRID:
				flipMenu(tmpMenu, mMenuMode);
				tmpSliderVal = mGridVal;
				break;
			case MENU_BLUR:
				flipMenu(tmpMenu, mMenuMode);
				tmpSliderVal = mBlurVal;
				break;
			case MENU_THRESHOLD:
				flipMenu(tmpMenu, mMenuMode);
				tmpSliderVal = mThresholdVal;
				break;
			case MENU_CONTRAST:
				flipMenu(tmpMenu, mMenuMode);
				tmpSliderVal = mContrastVal;
				break;
			case MENU_SAVE:
				saveImg();
				break;
			}
			if (tmpSliderVal >= 0) {
				cvSetTrackbarPos(mSliderName, mWindowName, tmpSliderVal);
			}
			adjustImage();
			showMenu(mMenuMode);
		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		bool imageUpdateOnLButtonUp = false;
		mClkStat.mousedown = false;
		switch (mMenuSelection) {
		default:
			break;
		}
	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (mClkStat.mousedown) {
			switch (mMenuSelection) {
			}
			adjustImage();
			showLocData(x, y, mAdjustedImage);
			showMenu(mMenuMode);
		}
	}
}
void heatmapDisplay::trackCallback(int val) {
	if (mMenuSelection != MENU_NONE) {
		switch (mMenuSelection) {
		case MENU_SETGRID:
			mGridVal = (val > 0) ? val : 1;
			break;
		case MENU_BLUR:
			mBlurVal = val;
			break;
		case MENU_THRESHOLD:
			mThresholdVal = val;
			break;
		case MENU_CONTRAST:
			mContrastVal = val;
			break;
		}
		adjustImage();
		showMenu(mMenuMode);
	}

}

int heatmapDisplay::menuSelected(int x, int y, menuMode whichMenu) {
	int retv = MENU_NONE;
	int maxy = MENU_QTY;
	int offsety = 0;
	maxy += 1; // title allowance
	maxy *= MENU_PANEL_LINE_HEIGHT;
	if (x <= MENU_PANEL_WIDTH && y > MENU_PANEL_LINE_HEIGHT && y <= maxy) {
		retv = y / MENU_PANEL_LINE_HEIGHT;
		retv -= 1; // title allowance
		retv += offsety;
	}
	return retv;
}

void heatmapDisplay::saveImg() {
	imwrite((mBaseFilename + ".heatmap.png").c_str(), mAdjustedImage);
}
void heatmapDisplay::showLocData(int pxlX, int pxlY, Mat adjustedImage) {
	stringstream ss;
	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
	//	cv::Scalar(255, 255, 255), -1);
	Point2f rwLoc = pPerspective->getRWloc(Point2f(pxlX, pxlY));
	ss << pxlX << " " << pxlY << " " << rwLoc.x << " " << rwLoc.y;
	string frameNumberString = ss.str();
	putText(adjustedImage, frameNumberString.c_str(), cv::Point(MENU_PANEL_WIDTH + 15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
}


void heatmapDisplay::showImg(string videoFilename, Mat img, perspective *pPersp, std::vector<point3dMm> *allLocations) {
	mBaseFilename = videoFilename;
	mOrgImage = img;
	mRows = img.rows;
	mCols = img.cols;
	pPerspective = pPersp;
	pAllLocations = allLocations;
	Point2f tmpPnt =  pPersp->getAdjustedRWloc(Point2f(0, mRows - 1));
	mBLxMm = tmpPnt.x;
	mBLyMm = tmpPnt.y;
	tmpPnt =  pPersp->getAdjustedRWloc(Point2f(mCols - 1, 0));
	mTRxMm = tmpPnt.x;
	mTRyMm = tmpPnt.y;
	tmpPnt =  pPersp->getAdjustedRWloc(Point2f(0, 0));
	float mTLxMm = tmpPnt.x;
	float mTLyMm = tmpPnt.y;
	tmpPnt =  pPersp->getAdjustedRWloc(Point2f(mCols - 1, mRows - 1));
	float mBRxMm = tmpPnt.x;
	float mBRyMm = tmpPnt.y;
	if (mTLxMm < mBLxMm) mBLxMm = mTLxMm;
	if (mBRyMm < mBLyMm) mBLyMm = mBRyMm;
	if (mTLyMm > mTRyMm) mTRyMm = mTLyMm;
	if (mBRxMm > mTRxMm) mTRxMm = mBRxMm;
	mHaveImageAndData = true;
	adjustImage();
	showMenu(mMenuMode);
}
void heatmapDisplay::showRevisedPoints(std::vector<point3dMm> *allLocations) {
	pAllLocations = allLocations;
	mHaveImageAndData = true;
	adjustImage();
	showMenu(mMenuMode);
}

void heatmapDisplay::setHotColour(int r, int g, int b) {
	mHotColour = vector3f(r, g, b);
	if (mHaveImageAndData) {
		destroyWindow(mWindowName);
		adjustImage();
		showMenu(mMenuMode);

	}
}
void heatmapDisplay::setColdColour(int r, int g, int b) {
	mColdColour = vector3f(r, g, b);
	if (mHaveImageAndData) {
		destroyWindow(mWindowName);
		adjustImage();
		showMenu(mMenuMode);

	}
}

void heatmapDisplay::hide() {
	destroyWindow(mWindowName);
}

void heatmapDisplay::adjustImage() {
	Mat tmpImag;
	Mat tmpAImag;
	mOrgImage.copyTo(tmpImag); // mAdjustedImage);
	mOrgImage.copyTo(tmpAImag); // mAdjustedImage);

	drawHeatmapOnAdjustedImage(tmpImag); // mAdjustedImage);
	pPerspective->warp(mOrgImage, tmpAImag);
	mAdjustedImage = tmpAImag * 0.5 + tmpImag * 0.5;
}
void heatmapDisplay::drawHeatmapOnAdjustedImage(Mat img) {
	mGridRows = ((int)((mTRyMm - mBLyMm) / mGridVal)) + 1;
	mGridCols = ((int)((mTRxMm - mBLxMm) / mGridVal)) + 1;
	if (pHeatMap != nullptr) {
		delete pHeatMap;
	}
	pHeatMap = new float[mGridCols * mGridRows];
	for (int idx = 0; idx < mGridCols * mGridRows; idx++) {
		pHeatMap[idx] = 0;
	}
	int flagGridCol = (mTRxMm - mBLxMm) / mGridVal;
	int flagGridRow = (mTRyMm - mBLyMm) / mGridVal;
	int flagGridCol0 = 0;
	int flagGridRow0 = 0;
	pHeatMap[(flagGridRow)* mGridCols + flagGridCol] = 1;
	pHeatMap[(flagGridRow0)* mGridCols + flagGridCol] = 1;
	pHeatMap[(flagGridRow)* mGridCols + flagGridCol0] = 1;
	pHeatMap[(flagGridRow0)* mGridCols + flagGridCol0] = 1;
	float pixelsPerGridH = (float)mCols / (float)mGridCols;
	float pixelsPerGridV = (float)mRows / (float)mGridRows;
	for (int idx = 0; idx < (pAllLocations->size() - 1); idx++) {
		Point2f rwLoc = Point2f(pAllLocations->at(idx).RWxSmoothed, pAllLocations->at(idx).RWySmoothed);
		Point2f imageLoc = pPerspective->getAdjustedPictureloc(rwLoc);
		int locGridCol = (int)((float)imageLoc.x / pixelsPerGridH); // (pAllLocations->at(idx).RWxSmoothed - mBLxMm) / mGridVal;
		int locGridRow = (int)((float)imageLoc.y / pixelsPerGridV); // (pAllLocations->at(idx).RWySmoothed - mBLyMm) / mGridVal;
		float locPeriod = pAllLocations->at(idx + 1).fTime - pAllLocations->at(idx).fTime;
		if (locGridCol >= 0 && locGridCol < mGridCols && locGridRow >= 0 && locGridRow < mGridRows && locPeriod <= mGapAllowed) {
			for (int blurIdx = 1; blurIdx < mBlurVal; blurIdx++) {
				float locPeriodBlur = locPeriod / ((float)(blurIdx * blurIdx));
				int blurredXneg = locGridCol - blurIdx;
				int blurredXpos = locGridCol + blurIdx;
				for (int blurIdxY = 1; (blurIdxY <= (mBlurVal - blurIdx + 1)) && (blurIdxY <= blurIdx); blurIdxY++) {
					// do vertical side bars
					int blurredYneg = locGridRow - blurIdxY;
					int blurredYpos = locGridRow + blurIdxY;
					if (blurredYneg >= 0 && blurredYpos < mGridRows && blurredXneg >= 0 && blurredXpos < mGridCols) {
						pHeatMap[(blurredYneg) * mGridCols + blurredXneg] += locPeriodBlur;
						pHeatMap[(blurredYneg) * mGridCols + blurredXpos] += locPeriodBlur;
						pHeatMap[(blurredYpos) * mGridCols + blurredXneg] += locPeriodBlur;
						pHeatMap[(blurredYpos) * mGridCols + blurredXpos] += locPeriodBlur;

					}
				}
				if (blurredXneg >= 0 && blurredXpos < mGridCols) { // horizontal midpoints
					pHeatMap[(locGridRow)* mGridCols + blurredXneg] += locPeriodBlur;
					pHeatMap[(locGridRow)* mGridCols + blurredXpos] += locPeriodBlur;
				}
				int blurredYneg = locGridRow - blurIdx;
				int blurredYpos = locGridRow + blurIdx;
				for (int blurIdxX = 1; (blurIdxX < (mBlurVal - blurIdx + 1)) && (blurIdxX < blurIdx); blurIdxX++) { // Note, < as === blurIdx was addressed in the vetical side bar loop above
					// do top & bottom side bars
					int blurredXneg = locGridCol - blurIdxX;
					int blurredXpos = locGridCol + blurIdxX;
					if (blurredYneg >= 0 && blurredYpos < mGridRows && blurredXneg >= 0 && blurredXpos < mGridCols) {
						pHeatMap[(blurredYneg)* mGridCols + blurredXneg] += locPeriodBlur;
						pHeatMap[(blurredYneg)* mGridCols + blurredXpos] += locPeriodBlur;
						pHeatMap[(blurredYpos)* mGridCols + blurredXneg] += locPeriodBlur;
						pHeatMap[(blurredYpos)* mGridCols + blurredXpos] += locPeriodBlur;
					}
				}
				if (blurredYneg >= 0 && blurredYpos < mGridRows) { // vertical mdpoints
					pHeatMap[(blurredYneg)* mGridCols + locGridCol] += locPeriodBlur;
					pHeatMap[(blurredYpos)* mGridCols + locGridCol] += locPeriodBlur;
				}
			}
			pHeatMap[locGridRow * mGridCols + locGridCol] += locPeriod;
		}
	}
	float maxHeatMap = 0;
	for (int idy = 0; idy < mGridRows; idy++) {
		int rowOffset = idy * mGridCols;
		for (int idx = 0; idx < mGridCols; idx++) {
			if (pHeatMap[rowOffset + idx] > maxHeatMap)
				maxHeatMap = pHeatMap[rowOffset + idx];
		}
	}
	for (int idy = 0; idy < mGridRows; idy++) {
		int rowOffset = idy * mGridCols;
		for (int idx = 0; idx < mGridCols; idx++) {
			pHeatMap[rowOffset + idx] /= maxHeatMap;
		}
	}
	img = Scalar(0, 0, 0); // mColdColour.dz, mColdColour.dy, mColdColour.dx);
	int pixelsX = mCols / mGridCols;
	int pixelsY = mRows / mGridRows;
	float backgroundVal = (((float)mThresholdVal) / 1000.0f);
	for (int idy = 0; idy < mGridRows; idy++) {
		int rowOffset = idy * mGridCols;
		for (int idx = 0; idx < mGridCols; idx++) {
			int segmentBLx = idx * mCols / mGridCols;
			int segmentBLy = idy * mRows / mGridRows;// mRows - (idy * mRows / mGridRows);
			float heatVal = pHeatMap[rowOffset + idx];
			float intensityVal = (heatVal - backgroundVal) * (((float)mContrastVal) / 10.0f);
			if (intensityVal > 1.0) intensityVal = 1.0;
			if (heatVal > backgroundVal) {
				uchar clrR = mColdColour.dx + (mHotColour.dx - mColdColour.dx) * intensityVal;
				uchar clrG = mColdColour.dy + (mHotColour.dy - mColdColour.dy) * intensityVal;
				uchar clrB = mColdColour.dz + (mHotColour.dz - mColdColour.dz) * intensityVal;

				rectangle(img, Point(segmentBLx, segmentBLy), Point(segmentBLx + pixelsX, segmentBLy - pixelsY), Scalar(clrB, clrG, clrR), -1);
			}
		}
	}

}
void heatmapDisplay::adjustImage(Mat img, Mat *outImage, perspective *pPersp, std::vector<point3dMm> *allLocations, float refPxlX, float refPxlY, float blurPxls, float blurVal) {
	Mat tmpImag;
	Mat tmpAImag;
	img.copyTo(tmpImag); // mAdjustedImage);
	img.copyTo(tmpAImag); // mAdjustedImage);

	drawHeatmapOnAdjustedImage(tmpImag, pPersp, allLocations,  refPxlX,  refPxlY,  blurPxls,  blurVal);
	*outImage = tmpAImag * 0.5 + tmpImag * 0.5;
}
void heatmapDisplay::drawHeatmapOnAdjustedImage(Mat img, perspective *pPersp, std::vector<point3dMm> *allLocations, float refPxlX, float refPxlY, float blurPxls, float blurVal) {
	int lclGridRows = ((int)((img.rows) / blurPxls)) + 1;
	int lclGridCols = ((int)((img.cols) / blurPxls)) + 1;
	if (pHeatMap != nullptr) {
		delete pHeatMap;
	}
	pHeatMap = new float[lclGridCols * lclGridRows];
	for (int idx = 0; idx < lclGridCols * lclGridRows; idx++) {
		pHeatMap[idx] = 0;
	}
	//int flagGridCol = (img.rows) / blurPxls;
	//int flagGridRow = (img.cols) / blurPxls;
	//int flagGridCol0 = 0;
	//int flagGridRow0 = 0;
	//pHeatMap[(flagGridRow)* lclGridCols + flagGridCol] = 1;
	//pHeatMap[(flagGridRow0)* lclGridCols + flagGridCol] = 1;
	//pHeatMap[(flagGridRow)* lclGridCols + flagGridCol0] = 1;
	//pHeatMap[(flagGridRow0)* lclGridCols + flagGridCol0] = 1;
	float pixelsPerGridH = blurPxls;
	float pixelsPerGridV = blurPxls;
	if (allLocations->size() > 0) {
		for (int idx = 0; idx < (allLocations->size() - 1); idx++) {
			Point2f rwLoc = Point2f(allLocations->at(idx).RWxSmoothed, allLocations->at(idx).RWySmoothed);
			Point2f imageLoc = pPersp->getAdjustedPictureloc(rwLoc);
			imageLoc.x -= refPxlX;
			imageLoc.y -= refPxlY;
			int locGridCol = (int)((float)imageLoc.x / pixelsPerGridH); // (pAllLocations->at(idx).RWxSmoothed - mBLxMm) / mGridVal;
			int locGridRow = (int)((float)imageLoc.y / pixelsPerGridV); // (pAllLocations->at(idx).RWySmoothed - mBLyMm) / mGridVal;
			float locPeriod = allLocations->at(idx + 1).fTime - allLocations->at(idx).fTime;
			if (locGridCol >= 0 && locGridCol < lclGridCols && locGridRow >= 0 && locGridRow < lclGridRows && locPeriod <= mGapAllowed) {
				for (int blurIdx = 1; blurIdx < blurVal; blurIdx++) {
					float locPeriodBlur = locPeriod / ((float)(blurIdx * blurIdx));
					int blurredXneg = locGridCol - blurIdx;
					int blurredXpos = locGridCol + blurIdx;
					for (int blurIdxY = 1; (blurIdxY <= (blurVal - blurIdx + 1)) && (blurIdxY <= blurIdx); blurIdxY++) {
						// do vertical side bars
						int blurredYneg = locGridRow - blurIdxY;
						int blurredYpos = locGridRow + blurIdxY;
						if (blurredYneg >= 0 && blurredYpos < lclGridRows && blurredXneg >= 0 && blurredXpos < lclGridCols) {
							pHeatMap[(blurredYneg)* lclGridCols + blurredXneg] += locPeriodBlur;
							pHeatMap[(blurredYneg)* lclGridCols + blurredXpos] += locPeriodBlur;
							pHeatMap[(blurredYpos)* lclGridCols + blurredXneg] += locPeriodBlur;
							pHeatMap[(blurredYpos)* lclGridCols + blurredXpos] += locPeriodBlur;

						}
					}
					if (blurredXneg >= 0 && blurredXpos < lclGridCols) { // horizontal midpoints
						pHeatMap[(locGridRow)* lclGridCols + blurredXneg] += locPeriodBlur;
						pHeatMap[(locGridRow)* lclGridCols + blurredXpos] += locPeriodBlur;
					}
					int blurredYneg = locGridRow - blurIdx;
					int blurredYpos = locGridRow + blurIdx;
					for (int blurIdxX = 1; (blurIdxX < (blurVal - blurIdx + 1)) && (blurIdxX < blurIdx); blurIdxX++) { // Note, < as === blurIdx was addressed in the vetical side bar loop above
						// do top & bottom side bars
						int blurredXneg = locGridCol - blurIdxX;
						int blurredXpos = locGridCol + blurIdxX;
						if (blurredYneg >= 0 && blurredYpos < lclGridRows && blurredXneg >= 0 && blurredXpos < lclGridCols) {
							pHeatMap[(blurredYneg)* lclGridCols + blurredXneg] += locPeriodBlur;
							pHeatMap[(blurredYneg)* lclGridCols + blurredXpos] += locPeriodBlur;
							pHeatMap[(blurredYpos)* lclGridCols + blurredXneg] += locPeriodBlur;
							pHeatMap[(blurredYpos)* lclGridCols + blurredXpos] += locPeriodBlur;
						}
					}
					if (blurredYneg >= 0 && blurredYpos < lclGridRows) { // vertical mdpoints
						pHeatMap[(blurredYneg)* lclGridCols + locGridCol] += locPeriodBlur;
						pHeatMap[(blurredYpos)* lclGridCols + locGridCol] += locPeriodBlur;
					}
				}
				pHeatMap[locGridRow * lclGridCols + locGridCol] += locPeriod;
			}
		}
	}
	float maxHeatMap = 0;
	for (int idy = 0; idy < lclGridRows; idy++) {
		int rowOffset = idy * lclGridCols;
		for (int idx = 0; idx < lclGridCols; idx++) {
			if (pHeatMap[rowOffset + idx] > maxHeatMap)
				maxHeatMap = pHeatMap[rowOffset + idx];
		}
	}
	for (int idy = 0; idy < lclGridRows; idy++) {
		int rowOffset = idy * lclGridCols;
		for (int idx = 0; idx < lclGridCols; idx++) {
			pHeatMap[rowOffset + idx] /= maxHeatMap;
		}
	}
	img = Scalar(0, 0, 0); // mColdColour.dz, mColdColour.dy, mColdColour.dx);
	int pixelsX = blurPxls;
	int pixelsY = blurPxls;
	float backgroundVal = (((float)mThresholdVal) / 1000.0f);
	for (int idy = 0; idy < lclGridRows; idy++) {
		int rowOffset = idy * lclGridCols;
		for (int idx = 0; idx < lclGridCols; idx++) {
			int segmentBLx = idx * blurPxls;
			int segmentBLy = idy * blurPxls;// mRows - (idy * mRows / mGridRows);
			float heatVal = pHeatMap[rowOffset + idx];
			float intensityVal = (heatVal - backgroundVal) * (((float)mContrastVal) / 10.0f);
			if (intensityVal > 1.0) intensityVal = 1.0;
			if (heatVal > backgroundVal) {
				uchar clrR = mColdColour.dx + (mHotColour.dx - mColdColour.dx) * intensityVal;
				uchar clrG = mColdColour.dy + (mHotColour.dy - mColdColour.dy) * intensityVal;
				uchar clrB = mColdColour.dz + (mHotColour.dz - mColdColour.dz) * intensityVal;

				rectangle(img, Point(segmentBLx - pixelsX / 2, segmentBLy + 1 + pixelsY / 2), Point(segmentBLx + 1 + pixelsX / 2, segmentBLy - pixelsY / 2), Scalar(clrB, clrG, clrR), -1);
			}
		}
	}

}
void heatmapDisplay::addHeatmapForAdjustedImage(int cols, int rows, float *pOldHeatmap, perspective *pPersp, std::vector<point3dMm> *allLocations, float refPxlX, float refPxlY) {
	if (allLocations->size() > 0) {
		for (int idx = 0; idx < (allLocations->size() - 1); idx++) {
			Point2f rwLoc = Point2f(allLocations->at(idx).RWxSmoothed, allLocations->at(idx).RWySmoothed);
			Point2f imageLoc = pPersp->getAdjustedPictureloc(rwLoc);
			imageLoc.x -= refPxlX;
			imageLoc.y -= refPxlY;
			int locGridCol = (int)((float)imageLoc.x);
			int locGridRow = (int)((float)imageLoc.y);
			float locPeriod = allLocations->at(idx + 1).fTime - allLocations->at(idx).fTime;
			if (locGridCol >= 0 && locGridCol < cols && locGridRow >= 0 && locGridRow < rows && locPeriod <= mGapAllowed) {
				pOldHeatmap[locGridRow * rows + locGridCol] += locPeriod;
			}
		}
	}
}
void heatmapDisplay::heatmapImage(Mat *imgInOut, int cols, int rows, float *oldHeatmap, int blurRange) {
	float *blurredHeatmap = new float[cols * rows];
	int minusBlurRange = -(blurRange) / 2;
	int plusBlurRange = (blurRange) / 2;
	float blurLimit = plusBlurRange * plusBlurRange;
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			float blurredValue = 0;
			for (int deltaY = idy + minusBlurRange; deltaY <= idy + plusBlurRange; deltaY++) {
				for (int deltaX = idx + minusBlurRange; deltaX <= idx + plusBlurRange; deltaX++) {
					if (deltaX >= 0 && deltaY >= 0 && deltaX < cols && deltaY < rows) {
						float blurDecayX = deltaX - idx;
						//blurDecayX *= 8.0f / (float) blurRange; // normalised to reduce to a single BMP bit ie 1/256 at the blurrange
						float blurDecayY = deltaY - idy;
						//blurDecayY *= 8.0f / (float) blurRange;
						blurDecayX = 1.0f + (blurDecayX * blurDecayX) + (blurDecayY * blurDecayY); //Math::Abs(blurDecay); //  add 1 to prevent zero and force the central cell to have a fraction of 1
						if (blurDecayX < blurLimit) {
							blurredValue += oldHeatmap[deltaX + deltaY * cols] / blurDecayX;
						}
					}
				}
			}
			blurredHeatmap[rowOffset + idx] =  blurredValue; // oldHeatmap[rowOffset + idx]; //
		}
	}
	float maxHeatMap = 0;
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			if (blurredHeatmap[rowOffset + idx] > maxHeatMap)
				maxHeatMap = blurredHeatmap[rowOffset + idx];
		}
	}
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			blurredHeatmap[rowOffset + idx] /= maxHeatMap;
		}
	}
	Mat img;
	imgInOut->copyTo(img);
	img = Scalar(0, 0, 0); // mColdColour.dz, mColdColour.dy, mColdColour.dx);
	int chn = img.channels();
	float backgroundVal = (((float)mThresholdVal) / 1000.0f);
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			float heatVal = blurredHeatmap[rowOffset + idx];
			float intensityVal = (heatVal - backgroundVal) *(((float)mContrastVal) / 66.0f);
			if (intensityVal > 1.0) intensityVal = 1.0;
//			if (heatVal > backgroundVal) {
				uchar clrR = mColdColour.dx + (mHotColour.dx - mColdColour.dx) * intensityVal;
				uchar clrG = mColdColour.dy + (mHotColour.dy - mColdColour.dy) * intensityVal;
				uchar clrB = mColdColour.dz + (mHotColour.dz - mColdColour.dz) * intensityVal;
				img.at<Vec3b>(idy, idx)[0] = clrB;
				img.at<Vec3b>(idy, idx)[1] = clrG;
				img.at<Vec3b>(idy, idx)[2] = clrR;
//			}
		}
	}
	img.copyTo(*imgInOut);
}
void heatmapDisplay::heatmapImageAlpha(Mat *imgInOut, Mat *foregroundInOut, int cols, int rows, float *oldHeatmap, int blurRange) {
	float *blurredHeatmap = new float[cols * rows];
	int minusBlurRange = -(blurRange) / 2;
	int plusBlurRange = (blurRange) / 2;
	float blurLimit = plusBlurRange * plusBlurRange;
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			float blurredValue = 0;
			for (int deltaY = idy + minusBlurRange; deltaY <= idy + plusBlurRange; deltaY++) {
				for (int deltaX = idx + minusBlurRange; deltaX <= idx + plusBlurRange; deltaX++) {
					if (deltaX >= 0 && deltaY >= 0 && deltaX < cols && deltaY < rows) {
						float blurDecayX = deltaX - idx;
						//blurDecayX *= 8.0f / (float) blurRange; // normalised to reduce to a single BMP bit ie 1/256 at the blurrange
						float blurDecayY = deltaY - idy;
						//blurDecayY *= 8.0f / (float) blurRange;
						blurDecayX = 1.0f + (blurDecayX * blurDecayX) + (blurDecayY * blurDecayY); //Math::Abs(blurDecay); //  add 1 to prevent zero and force the central cell to have a fraction of 1
						if (blurDecayX < blurLimit) {
							blurredValue += oldHeatmap[deltaX + deltaY * cols] / blurDecayX;
						}
					}
				}
			}
			blurredHeatmap[rowOffset + idx] =  blurredValue; // oldHeatmap[rowOffset + idx]; //
		}
	}
	float maxHeatMap = 0;
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			if (blurredHeatmap[rowOffset + idx] > maxHeatMap)
				maxHeatMap = blurredHeatmap[rowOffset + idx];
		}
	}
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			blurredHeatmap[rowOffset + idx] /= maxHeatMap;
		}
	}
	Mat img;
	imgInOut->copyTo(img);
	img.convertTo(img, CV_32FC3);
	img.copyTo(*foregroundInOut);
	img = Scalar(0, 0, 0); // mColdColour.dz, mColdColour.dy, mColdColour.dx);
	int chn = img.channels();
	float backgroundVal = (((float)mThresholdVal) / 1000.0f);
	for (int idy = 0; idy < rows; idy++) {
		int rowOffset = idy * cols;
		for (int idx = 0; idx < cols; idx++) {
			float heatVal = blurredHeatmap[rowOffset + idx];
			float intensityVal = (heatVal - backgroundVal) *(((float)mContrastVal) / 66.0f);
			img.at<Vec3f>(idy, idx)[0] = intensityVal;
			img.at<Vec3f>(idy, idx)[1] = intensityVal;
			img.at<Vec3f>(idy, idx)[2] = intensityVal;
			foregroundInOut->at<Vec3f>(idy, idx)[2] = mHotColour.dx;	// OpenCV is BGR, not RBG
			foregroundInOut->at<Vec3f>(idy, idx)[1] = mHotColour.dy;
			foregroundInOut->at<Vec3f>(idy, idx)[0] = mHotColour.dz;
//			}
		}
	}
	img.copyTo(*imgInOut);
}
