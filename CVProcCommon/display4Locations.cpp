
#include "cvUtilities.h"
#include "display4Locations.h"

display4Locations::display4Locations(char *windowName, Mat baseImage, perspectiveImageReference perspectiveGeometry, updateCallback notification, int callerData)
	: mWindowName(nullptr)
{
	mWhichPoint = -1;
	mCallerSuppliedData = callerData;
	mCallBackMethod = notification;
	mWindowName = new char[strlen(windowName) + 1];
	strcpy(mWindowName, windowName);
	baseImage.copyTo(mOrgImage);
	mCols = mOrgImage.cols;
	mRows = mOrgImage.rows;
	mMenuMode = topMenu;
	mMenuTitle[0] = "Points";
	mMenuTitle[1] = "| Sub-Zones |";
	mMenuTitle[2] = "| Properties |";

	mMenuStr[MENU_SETTL] = "Top Left";
	mMenuSelected[MENU_SETTL] = false;
	mMenuStr[MENU_SETTR] = "Top Right";
	mMenuSelected[MENU_SETTR] = false;
	mMenuStr[MENU_SETBR] = "Bottom Right";
	mMenuSelected[MENU_SETBR] = false;
	mMenuStr[MENU_SETBL] = "Bottom Left";
	mMenuSelected[MENU_SETBL] = false;
	mMenuStr[MENU_SETALL] = "All";
	mMenuSelected[MENU_SETALL] = false;
	mMenuStr[MENU_UPDATE] = "Update";
	mMenuSelected[MENU_UPDATE] = false;
	mPerspectiveGeometry.orgTL = perspectiveGeometry.orgTL;
	mPerspectiveGeometry.orgTR = perspectiveGeometry.orgTR;
	mPerspectiveGeometry.orgBR = perspectiveGeometry.orgBR;
	mPerspectiveGeometry.orgBL = perspectiveGeometry.orgBL;
	mPerspectiveGeometry.referenceLocation = perspectiveGeometry.referenceLocation;
	showImg();
	setMouseCallback(mWindowName, display4Locations::mouseCallback, this);
}
display4Locations::~display4Locations()
{
	if (mWindowName != nullptr) delete mWindowName;
	destroyWindow(mWindowName);
}

void display4Locations::adjustDisplay() {

}

void display4Locations::getCopyOfPerspectiveData(perspectiveImageReference *pPerspectiveGeometry) {
	pPerspectiveGeometry->orgTL = mPerspectiveGeometry.orgTL;
	pPerspectiveGeometry->orgTR = mPerspectiveGeometry.orgTR;
	pPerspectiveGeometry->orgBR = mPerspectiveGeometry.orgBR;
	pPerspectiveGeometry->orgBL = mPerspectiveGeometry.orgBL;
	pPerspectiveGeometry->referenceLocation = mPerspectiveGeometry.referenceLocation;

}

void display4Locations::showMenuLine(Mat img, int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(img, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(img, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void display4Locations::menuQtyString(char *strBuf, int menuIdx, menuMode whichMenu) {
	switch (menuIdx) {
	default:
		*strBuf = '\0';
		break;
	//case MENU_NULL:
	//	break;
	}
}
void display4Locations::showMenu(menuMode whichMenu, Mat img) {
	char menuLine[64];
	char menuVal0[32];
	int y = 0;
	switch (whichMenu) {
	case topMenu:
		showMenuLine(img, 0, mMenuTitle[0]);
		while (y < MENU_QTY) {
			menuQtyString(&menuVal0[0], y, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y], &menuVal0[0], mMenuSelected[y] ? '*' : ' ');
			showMenuLine(img, y + 1, menuLine);
			y++;
		}
		break;
	case subMenu:
		showMenuLine(img, 0, mMenuTitle[1]);
		while (y < MENU_SUB_QTY) {
			menuQtyString(&menuVal0[0], y + MENU_QTY, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY] ? '*' : ' ');
			showMenuLine(img, y + 1, menuLine);
			y++;
		}
		break;
	case properties:
		showMenuLine(img, 0, mMenuTitle[2]);
		while (y < MENU_PROP_QTY) {
			menuQtyString(&menuVal0[0], y + MENU_QTY + MENU_SUB_QTY, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY + MENU_SUB_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY + MENU_SUB_QTY] ? '*' : ' ');
			showMenuLine(img, y + 1, menuLine);
			y++;
		}
		break;
	}
	imshow(mWindowName, mDisplayImage);
}

void display4Locations::setRefLocation(int menuChoiceIdx, int x, int y) {
	switch (menuChoiceIdx) {
	case MENU_SETTL - MENU_SETTL:
		mPerspectiveGeometry.orgTL.x = x;
		mPerspectiveGeometry.orgTL.y = y;
		break;
	case MENU_SETTR - MENU_SETTL:
		mPerspectiveGeometry.orgTR.x = x;
		mPerspectiveGeometry.orgTR.y = y;
		break;
	case MENU_SETBR - MENU_SETTL:
		mPerspectiveGeometry.orgBR.x = x;
		mPerspectiveGeometry.orgBR.y = y;
		break;
	case MENU_SETBL - MENU_SETTL:
		mPerspectiveGeometry.orgBL.x = x;
		mPerspectiveGeometry.orgBL.y = y;
		break;

	}
}


void display4Locations::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	display4Locations *pdisplay4Locations = (display4Locations *)userdata;
	pdisplay4Locations->mouseCallback(event, x, y, flags);
}

void display4Locations::clearAndSetMenu(int menuSelection, menuMode whichMenu) {
		if (mMenuSelection != MENU_NONE) {
			mMenuSelected[mMenuSelection] = false;
		}
		mMenuSelection = menuSelection;
		if (menuSelection != MENU_NONE) {
			mMenuSelected[menuSelection] = true;
		}
}
void display4Locations::flipMenu(int menuSelection, menuMode whichMenu) {
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
void display4Locations::clearMenuSelections() {
	for (int idx = 0; idx < MENU_ALL_QTY; idx++) {
		mMenuSelected[idx] = false;
	}
}
void display4Locations::mouseCallback(int event, int x, int y, int flags) {
	bool adviseOfChange = false;
	if (x < 0) x = 0;
	if (x >= mCols) x = mCols - 1;
	if (y < 0) y = 0;
	if (y >= mRows) y = mRows - 1;

	if (event == EVENT_LBUTTONDOWN)
	{
		int tmpMenu = menuSelected(x, y, mMenuMode);
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_SETTR:
			case MENU_SETTL:
			case MENU_SETBR:
			case MENU_SETBL:
			case MENU_SETALL:
				mMenuSelected[tmpMenu - MENU_SETTL] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				mRefAllIdx = 0;
				mWhichPoint = 0;
				break;
			case MENU_UPDATE:
				if (mCallBackMethod != nullptr) {
					mCallBackMethod(mCallerSuppliedData);
				}
				break;
			}
		}
		else if (primaryClickStatus.menuChoice != MENU_NONE) {
			switch (primaryClickStatus.menuChoice) {
			case MENU_SETTR:
			case MENU_SETTL:
			case MENU_SETBR:
			case MENU_SETBL:
				setRefLocation(primaryClickStatus.menuChoice - MENU_SETTL, x, y);
				primaryClickStatus.menuChoice = MENU_NONE;
				mMenuSelected[tmpMenu - MENU_SETTL] = false;
				break;
			case MENU_SETALL:
				setRefLocation(mRefAllIdx++, x, y);
				if (mRefAllIdx >= ((MENU_SETBL - MENU_SETTL) + 1)) {
					primaryClickStatus.menuChoice = MENU_NONE;
					mMenuSelected[tmpMenu - MENU_SETTL] = false;
				}
				break;
			}
		}
		showImg();
	}
}

int display4Locations::menuSelected(int x, int y, menuMode whichMenu) {
	int retv = MENU_NONE;
	int maxy = (whichMenu == topMenu) ?  MENU_QTY : (whichMenu == subMenu)? MENU_SUB_QTY : MENU_PROP_QTY;
	int offsety = (whichMenu == topMenu) ? 0  : (whichMenu == subMenu)? MENU_QTY : MENU_QTY + MENU_SUB_QTY;
	maxy += 1; // title allowance
	maxy *= MENU_PANEL_LINE_HEIGHT;
	if (x <= MENU_PANEL_WIDTH && y > MENU_PANEL_LINE_HEIGHT && y <= maxy) {
		retv = y / MENU_PANEL_LINE_HEIGHT;
		retv -= 1; // title allowance
		retv += offsety;
	}
	return retv;
}
void display4Locations::showImg() {
	adjustImage();
	showMenu(mMenuMode, mDisplayImage);
	imshow(mWindowName, mDisplayImage);
}
void display4Locations::hide() {
	destroyWindow(mWindowName);
}
void display4Locations::drawCross(Mat image, Point whereOnImage, char *name) {
	line(image, Point(whereOnImage.x, whereOnImage.y - 8), Point(whereOnImage.x, whereOnImage.y + 8), Scalar(255, 255, 255), 1);
	line(image, Point(whereOnImage.x - 8, whereOnImage.y), Point(whereOnImage.x + 8, whereOnImage.y), Scalar(255, 255, 255), 1);
	putText(image, name, cv::Point(whereOnImage.x - 8, whereOnImage.y - 10),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
}
void display4Locations::drawCrosses(Mat image) {
	drawCross(image, mPerspectiveGeometry.orgTL, "ref 1");
	drawCross(image, mPerspectiveGeometry.orgTR, "ref 2");
	drawCross(image, mPerspectiveGeometry.orgBR, "ref 3");
	drawCross(image, mPerspectiveGeometry.orgBL, "ref 4");
}

void display4Locations::adjustImage() {
	mOrgImage.copyTo(mDisplayImage);
	drawCrosses(mDisplayImage);
}
Mat display4Locations::translateImg(Mat &img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	Mat retv;
	warpAffine(img, retv, trans_mat, img.size());
	return retv;
}

