
#include "cvUtilities.h"
#include "maskDisplay.h"


maskDisplay::maskDisplay(char *windowName)
	: mWindowName(nullptr)
{
	mWindowName = new char[strlen(windowName) + 1];
	strcpy(mWindowName, windowName);
	mTrackVal = 0;
	mShowFilled = false;
	mSliderName = "Slider";
	mMenuTitle =  "| Zones |";

	mMenuStr[MENU_ADD] = "Add";
	mMenuSelected[MENU_ADD] = false;
	mMenuStr[MENU_DELETE] = "Delete";
	mMenuSelected[MENU_DELETE] = false;
	mMenuStr[MENU_CLEAR] = "Clear";
	mMenuSelected[MENU_CLEAR] = false;
	mMenuStr[MENU_SHOW] = "Show";
	mMenuSelected[MENU_SHOW] = false;
	mMenuStr[MENU_UPDATE] = "Update";
	mMenuSelected[MENU_UPDATE] = false;
	mMenuSelection = MENU_NONE;
	mSelectedRect = nullptr;
}
maskDisplay::~maskDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;

}

void maskDisplay::adjustDisplay() {

}

void maskDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(mDisplayImage, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void maskDisplay::showMenu() {
	mAdjustedImage.copyTo(mDisplayImage);
	char menuLine[64];
	char menuVal0[32];
	int y = 0;
	showMenuLine(0, mMenuTitle);
	while (y < MENU_QTY) {
		sprintf(menuLine, "%s %c", mMenuStr[y], mMenuSelected[y] ? '*' : ' ');
		showMenuLine(y + 1, menuLine);
		y++;
	}
	imshow(mWindowName, mDisplayImage);
}


void maskDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	maskDisplay *pmaskDisplay = (maskDisplay *)userdata;
	pmaskDisplay->mouseCallback(event, x, y, flags);
}

void maskDisplay::clearAndSetMenu(int menuSelection) {
	if (mMenuSelection != MENU_NONE) {
		mMenuSelected[mMenuSelection] = false;
	}
	mMenuSelection = menuSelection;
	if (menuSelection != MENU_NONE) {
		mMenuSelected[menuSelection] = true;
	}
}
void maskDisplay::flipMenu(int menuSelection) {
	if (menuSelection != MENU_NONE) {
		if (mMenuSelected[menuSelection]) {
			mMenuSelected[menuSelection] = false;
			mMenuSelection = MENU_NONE;
		}
		else {
			clearAndSetMenu(menuSelection);
		}
	}
}
void maskDisplay::clearMenuSelections() {
	for (int idx = 0; idx < MENU_QTY; idx++) {
		mMenuSelected[idx] = false;
	}
	mSelectedRectIdx = -1;
}

void maskDisplay::mouseCallback(int event, int x, int y, int flags) {
	bool adviseOfChange = false;
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
			case MENU_ADD:
			case MENU_DELETE:
			case MENU_SHOW:
				//			case MENU_RESIZE:
				flipMenu(tmpMenu);
				mShowFilled = mMenuSelected[MENU_SHOW];

				break;
			case MENU_CLEAR:
				for (int idx = 0; idx < mRects->size(); idx++) {
					delete(mRects->at(idx));
				}
				mRects->clear();
				mMaskIn = Scalar(0, 0, 0);

				break;
			case MENU_UPDATE:
				createMask();
				mMask.copyTo(*mMaskOut);
				break;
			}
			adjustImage();
			showMenu();
		}
		else {
			switch (mMenuSelection) {
			case MENU_ADD:
				mClkStat.mousedown = true;
				mClkStat.startX = x;
				mClkStat.startY = y;
				adviseOfChange = true;
				break;
			case MENU_DELETE:
			{
				int rectIdx = findRectIdx(x, y);
				if (rectIdx != -1) {
					deleteRect(rectIdx);
					mSelectedRectIdx = -1;
				}
				mSelectedRect = nullptr;

			}
			adviseOfChange = true;
			break;
			}
			adjustImage();
			showMenu();
		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		bool imageUpdateOnLButtonUp = false;
		switch (mMenuSelection) {
		case MENU_ADD:
			if (mClkStat.mousedown) {
				addRect(mClkStat.startX, mClkStat.startY, x, y);
			}
			break;
		case MENU_DELETE:
			break;
		}
		mClkStat.mousedown = false;
		if (imageUpdateOnLButtonUp) adjustImage();
	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (mClkStat.mousedown) {
			switch (mMenuSelection) {
			case MENU_ADD:
				adjustImage();
				Rect2i tmpRect(Point2i(x, y),Point2i(mClkStat.startX, mClkStat.startY));
				drawRect(mAdjustedImage, &tmpRect, true);
				showMenu();
				break;
				//case MENU_RESIZE:
				//	if (mSelectedZoneIdx != -1) {
				//		setZoneRad(x, y, mSelectedZoneIdx);
				//	}
				//	break;
			}
		}
	}
}

int maskDisplay::menuSelected(int x, int y) {
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
Rect2i *maskDisplay::findRect(int x, int y) {
	Rect2i *retv = nullptr;
	int idx = findRectIdx(x, y);
	if (idx >= 0 && idx < mRects->size()) {
		retv = mRects->at(idx);
	}
	return retv;
}



void maskDisplay::addRect(int pxlX, int pxlY, int pxlBRx, int pxlBRy) {
	char name[64];
	Point2f tlCorner(pxlX, pxlY);
	Rect2i *tmpRect2i = nullptr;
	tmpRect2i = new Rect2i(pxlX, pxlY, pxlBRx - pxlX, pxlBRy - pxlY);
	if (tmpRect2i != nullptr) {
		mRects->push_back(tmpRect2i);
	}
}


bool maskDisplay::rectIdxIsValid(int zIdx) {
	bool retv = false;
	if (zIdx >= 0 && zIdx < mRects->size()) {
				retv = true;
	}
	return retv;
}

void maskDisplay::deleteRect(int zIdx) {
	if (rectIdxIsValid(zIdx)) {
			delete mRects->at(zIdx); // delete the zone
			mRects->erase(mRects->begin() + zIdx); // remove list entry
	}
}



void maskDisplay::showImg(Mat img, std::vector<Rect2i *> *zones, Mat *outMask) {
	mOrgImage = img;
	mMaskOut = outMask;
	mMaskIn = mMaskOut->clone();
	mMask = img.clone();
	mRows = img.rows;
	mCols = img.cols;
	mRects = zones;
	adjustImage();
	imshow(mWindowName, mAdjustedImage);
	setMouseCallback(mWindowName, maskDisplay::mouseCallback, this);
	showMenu();
}
void maskDisplay::hide() {
	destroyWindow(mWindowName);
}

void maskDisplay::drawRect(Mat img, Rect2i *pZn, bool selected) {
	rectangle(img, *pZn, (selected) ? Scalar(255, 0, 0) : Scalar(0, 0, 0), (selected) ? 2 : 1);
}

void maskDisplay::createMask() {
	mMask = mMaskIn.clone(); // Scalar(0, 0, 0);
	drawRectsOnAdjustedImage(mMask, Scalar(255, 255, 255), -1);
}
void maskDisplay::adjustImage() {
	mOrgImage.copyTo(mAdjustedImage);
	if (mShowFilled) {
		createMask();
		mAdjustedImage = mOrgImage * 0.5 + mMask * 0.5;
//		bitwise_and(mOrgImage, mMask, mAdjustedImage);
	}
	else {
		drawRectsOnAdjustedImage(mAdjustedImage);
	}
}
void maskDisplay::drawRectsOnAdjustedImage(Mat img) {
	if (mRects != nullptr) {
		for (int idx = 0; idx < mRects->size(); idx++) {
			drawRect(img, mRects->at(idx), mSelectedRectIdx == idx);
		}
	}
}
void maskDisplay::drawRectsOnAdjustedImage(Mat img, Scalar clr, int width) {
	if (mRects != nullptr) {
		for (int idx = 0; idx < mRects->size(); idx++) {
			rectangle(img, *mRects->at(idx), clr, width);
		}
	}
}
int maskDisplay::findRectIdx(int pxlX, int pxlY) {
	int retv = -1;
	Point2i centreXY(pxlX, pxlY);

	mSelectedRect = nullptr;
	for (int idx = 0; idx < mRects->size(); idx++) {
		if (mRects->at(idx)->contains(centreXY)) {
			retv = idx;
			mSelectedRect = mRects->at(idx);
			break;
		}
	}
	return retv;
}

