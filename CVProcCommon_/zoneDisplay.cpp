
#include "cvUtilities.h"
#include "zoneDisplay.h"
#include "thresholdEstimation.h"
#include <cmath>

extern void saveToClipboard(string str);

zoneDisplay::zoneDisplay(char *windowName)
	: mWindowName(nullptr)
{
	mWindowName = new char[strlen(windowName) + 1];
	strcpy(mWindowName, windowName);
	mTrackVal = 0;
	mCompoundZones = nullptr;
	mUserDefinedTypes.clear();

	mSliderName = "Slider";
	mMenuMode = topMenu;
	mMenuTitle[0] = "| Zones |";
	mMenuTitle[1] = "| Sub-Zones |";
	mMenuTitle[2] = "| Properties |";

	mMenuStr[MENU_SUBZONES] = "Sub-Zones";
	mMenuSelected[MENU_SUBZONES] = false;
	mMenuStr[MENU_CIRCLE] = "Circle";
	mMenuSelected[MENU_CIRCLE] = false;
	mMenuStr[MENU_TRIANGLE] = "Triangle";
	mMenuSelected[MENU_TRIANGLE] = false;
	mMenuStr[MENU_SQUARE] = "Square";
	mMenuSelected[MENU_SQUARE] = false;
	mMenuStr[MENU_DLEFT] = "D-left";
	mMenuSelected[MENU_DLEFT] = false;
	mMenuStr[MENU_DRIGHT] = "D-right";
	mMenuSelected[MENU_DRIGHT] = false;
	mMenuStr[MENU_ADD] = "Add";
	mMenuSelected[MENU_ADD] = false;
	mMenuStr[MENU_MOVE] = "Move";
	mMenuSelected[MENU_MOVE] = false;
//	mMenuStr[MENU_RESIZE] = "Resize";
//	mMenuSelected[MENU_RESIZE] = false;
	mMenuStr[MENU_SETRAD] = "Def. Size";
	mMenuSelected[MENU_SETRAD] = false;
	mMenuStr[MENU_DELETE] = "Delete";
	mMenuSelected[MENU_DELETE] = false;
	mMenuStr[MENU_UPDATE] = "Update";
	mMenuSelected[MENU_UPDATE] = false;
	mMenuStr[MENU_SUB_UPDATE] = "Update";
	mMenuSelected[MENU_SUB_UPDATE] = false;
	mMenuStr[MENU_PROP_UPDATE] = "Update";
	mMenuSelected[MENU_PROP_UPDATE] = false;
	mMenuSelection = MENU_NONE;
	mMenuStr[MENU_SUB_TOP] = "Top Zones";
	mMenuSelected[MENU_SUB_TOP] = false;
	mMenuStr[MENU_SUB_ADD] = "Add";
	mMenuSelected[MENU_SUB_ADD] = false;
	mMenuStr[MENU_SUB_MOVE] = "Move";
	mMenuSelected[MENU_SUB_MOVE] = false;
	mMenuStr[MENU_SUB_RESIZE] = "Resize";
	mMenuSelected[MENU_SUB_RESIZE] = false;
	mMenuStr[MENU_SUB_DELETE] = "Delete";
	mMenuSelected[MENU_SUB_DELETE] = false;
	mMenuStr[MENU_PROP_TOP] = "Top Zones";
	mMenuSelected[MENU_PROP_TOP] = false;
	mMenuStr[MENU_PROP_CIRCLE] = "Make Circle";
	mMenuSelected[MENU_PROP_CIRCLE] = false;
	mMenuStr[MENU_PROP_SQUARE] = "Make Square";
	mMenuSelected[MENU_PROP_SQUARE] = false;
	mMenuStr[MENU_PROP_DLEFT] = "Make D-left";
	mMenuSelected[MENU_PROP_DLEFT] = false;
	mMenuStr[MENU_PROP_DRIGHT] = "Make D-right";
	mMenuSelected[MENU_PROP_DRIGHT] = false;
	mMenuStr[MENU_PROP_TRIANGLE] = "Make Triangle";
	mMenuSelected[MENU_PROP_TRIANGLE] = false;
	mMenuStr[MENU_PROP_EXCLUDE] = "Exclude";
	mMenuSelected[MENU_PROP_EXCLUDE] = false;
	mMenuStr[MENU_PROP_USERTYPE] = "UserType";
	mMenuSelected[MENU_PROP_USERTYPE] = false;
	mMenuStr[MENU_PROP_SETRAD] = "Size";
	mMenuSelected[MENU_PROP_SETRAD] = false;
	mMenuStr[MENU_PROP_DUPLICATE] = "Duplicate";
	mMenuSelected[MENU_PROP_DUPLICATE] = false;
	mMenuStr[MENU_PROP_MOVE] = "Move";
	mMenuSelected[MENU_PROP_MOVE] = false;
	mMenuStr[MENU_PROP_DEPTH] = "Depth";
	mMenuSelected[MENU_PROP_DEPTH] = false;
	mMenuStr[MENU_PROP_ANGLE] = "Angle";
	mMenuSelected[MENU_PROP_ANGLE] = false;
	mRadVal = DEFAULT_RAD;
	mRadValSub = DEFAULT_RAD / 3;
	mSelectedZoneIdx = -1;
	mSelectedZone = nullptr;
	mNextZoneID = 0;
}
zoneDisplay::~zoneDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;

}

void zoneDisplay::adjustDisplay() {

}

void zoneDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(mDisplayImage, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void zoneDisplay::menuQtyString(char *strBuf, int menuIdx, menuMode whichMenu) {
	switch (menuIdx) {
	default:
		*strBuf = '\0';
		break;
	case MENU_SETRAD:
		sprintf(strBuf, "%d", mRadVal);
		break;
	case MENU_PROP_DEPTH:
		sprintf(strBuf, "%d", mPropDepthVal);
		break;
	case MENU_PROP_SETRAD:
		sprintf(strBuf, "%d", mPropRadVal);
		break;
	case MENU_PROP_EXCLUDE:
		sprintf(strBuf, "%c", mPropExclude ? '*' : ' ');
		break;
	case MENU_PROP_ANGLE:
		sprintf(strBuf, "%d", mPropAngleVal);
		break;
	case MENU_PROP_USERTYPE:
		sprintf(strBuf, "%s", mUserDefinedTypes[mPropUserType].c_str());
		break;

	}
}
void zoneDisplay::showMenu(menuMode whichMenu) {
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
	case subMenu:
		showMenuLine(0, mMenuTitle[1]);
		while (y < MENU_SUB_QTY) {
			menuQtyString(&menuVal0[0], y + MENU_QTY, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY] ? '*' : ' ');
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	case properties:
		showMenuLine(0, mMenuTitle[2]);
		while (y < MENU_PROP_QTY) {
			menuQtyString(&menuVal0[0], y + MENU_QTY + MENU_SUB_QTY, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY + MENU_SUB_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY + MENU_SUB_QTY] ? '*' : ' ');
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	}
	imshow(mWindowName, mDisplayImage);
}


void zoneDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	zoneDisplay *pZoneDisplay = (zoneDisplay *)userdata;
	pZoneDisplay->mouseCallback(event, x, y, flags);
}
void zoneDisplay::trackCallback(int val, void* userdata) {
	zoneDisplay *pZoneDisplay = (zoneDisplay *)userdata;
	pZoneDisplay->trackCallback(val);
}

void zoneDisplay::clearAndSetMenu(int menuSelection, menuMode whichMenu) {
		if (mMenuSelection != MENU_NONE) {
			mMenuSelected[mMenuSelection] = false;
		}
		mMenuSelection = menuSelection;
		if (menuSelection != MENU_NONE) {
			mMenuSelected[menuSelection] = true;
		}
}
void zoneDisplay::flipMenu(int menuSelection, menuMode whichMenu) {
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
void zoneDisplay::clearMenuSelections() {
	for (int idx = 0; idx < MENU_ALL_QTY; idx++) {
		mMenuSelected[idx] = false;
	}
	mSelectedZoneIdx = mSelectedSubZoneIdx = -1;
}
void zoneDisplay::showPropertiesMenuOfZone(int zIdx, int szIdx) {
	if (mSelectedZone != nullptr) {
		mPropRadVal = mSelectedZone->mRadius;
		mPropDepthVal = mSelectedZone->mLimitZ;
		mPropAngleVal = mSelectedZone->getAngleDeg();
		mPropUserType = mSelectedZone->mUserDefinedType;
		mPropExclude = mSelectedZone->mExclusion;
		switch (mMenuSelection) {
			case MENU_PROP_SETRAD:
				cvSetTrackbarPos(mSliderName, mWindowName, mPropRadVal);
				break;
			case MENU_PROP_ANGLE:
				cvSetTrackbarPos(mSliderName, mWindowName, mPropAngleVal + 180);
				break;
			case MENU_PROP_DEPTH:
				cvSetTrackbarPos(mSliderName, mWindowName, mPropDepthVal);
				break;
			case MENU_PROP_USERTYPE:
				cvSetTrackbarPos(mSliderName, mWindowName, mPropUserType);
				break;

		}
	}
	showMenu(mMenuMode);
}

void zoneDisplay::mouseCallback(int event, int x, int y, int flags) {
	bool adviseOfChange = false;
	if (x < 0) x = 0;
	if (x >= mCols) x = mCols - 1;
	if (y < 0) y = 0;
	if (y >= mRows) y = mRows - 1;

	if (event == EVENT_RBUTTONDOWN) {
		int foundIdx = 0;
		bool search = true;
		while (search) {
			if ((mSelectedZoneIdx = findClickedZone(x, y, foundIdx)) != -1) {
				mSelectedSubZoneIdx = findClickedSubZone(x, y);
				if (mSelectedSubZoneIdx != -1) {
					search = false;
				}
				else {
					foundIdx = mSelectedZoneIdx + 1;
				}
			}
			else {
				break;
			}
		}
		if (mSelectedSubZoneIdx == -1) {
			// did not find a zone and sub zone
			mSelectedZoneIdx = findClickedZone(x, y, 0);
		}
		if (mSelectedZoneIdx != -1) {
			mMenuMode = properties;
			adjustImage();
			showPropertiesMenuOfZone(mSelectedZoneIdx, mSelectedSubZoneIdx);
		}
	}
	if (event == EVENT_LBUTTONDOWN)
	{
		switch (mMenuMode) {
		case topMenu:
			break;
		case subMenu:
			break;
		case properties:
			break;
		}
		int tmpMenu = menuSelected(x, y, mMenuMode);
		int tmpSliderVal = -1;
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_TRIANGLE:
				mMenuSelected[MENU_SQUARE] = false;
				mMenuSelected[MENU_CIRCLE] = false;
				mMenuSelected[MENU_DLEFT] = false;
				mMenuSelected[MENU_DRIGHT] = false;
				mMenuSelected[MENU_TRIANGLE] = true;
				break;
			case MENU_SQUARE:
				mMenuSelected[MENU_SQUARE] = true;
				mMenuSelected[MENU_CIRCLE] = false;
				mMenuSelected[MENU_TRIANGLE] = false;
				mMenuSelected[MENU_DLEFT] = false;
				mMenuSelected[MENU_DRIGHT] = false;
				break;
			case MENU_CIRCLE:
				mMenuSelected[MENU_CIRCLE] = true;
				mMenuSelected[MENU_SQUARE] = false;
				mMenuSelected[MENU_TRIANGLE] = false;
				mMenuSelected[MENU_DLEFT] = false;
				mMenuSelected[MENU_DRIGHT] = false;
				break;
			case MENU_DLEFT:
				mMenuSelected[MENU_CIRCLE] = false;
				mMenuSelected[MENU_SQUARE] = false;
				mMenuSelected[MENU_TRIANGLE] = false;
				mMenuSelected[MENU_DLEFT] = true;
				mMenuSelected[MENU_DRIGHT] = false;
				break;
			case MENU_DRIGHT:
				mMenuSelected[MENU_CIRCLE] = false;
				mMenuSelected[MENU_SQUARE] = false;
				mMenuSelected[MENU_TRIANGLE] = false;
				mMenuSelected[MENU_DLEFT] = false;
				mMenuSelected[MENU_DRIGHT] = true;
				break;
			case MENU_PROP_TOP:
			case MENU_SUB_TOP:
				clearMenuSelections();
				mMenuMode = topMenu;
				mMenuSelection = MENU_NONE;
				break;
			case MENU_SUBZONES:
				clearMenuSelections();
				mMenuMode = subMenu;
				mMenuSelection = MENU_NONE;
				break;
			case MENU_SUB_ADD:
			case MENU_SUB_DELETE:
			case MENU_SUB_RESIZE:
				flipMenu(tmpMenu, mMenuMode);
				break;
			case MENU_ADD:
			case MENU_MOVE:
			case MENU_SUB_MOVE:
			case MENU_PROP_MOVE:
			case MENU_PROP_DUPLICATE:
			case MENU_DELETE:
//			case MENU_RESIZE:
				flipMenu(tmpMenu, mMenuMode);
				break;
			case MENU_SETRAD:
				flipMenu(tmpMenu, mMenuMode);
				tmpSliderVal = mRadVal;
				break;
			case MENU_PROP_CIRCLE:
				if (mSelectedZone != nullptr) {
					mSelectedZone->mShape = zone3d::circle;
				}
				break;
			case MENU_PROP_SQUARE:
				if (mSelectedZone != nullptr) {
					mSelectedZone->mShape = zone3d::square;
				}
				break;
			case MENU_PROP_TRIANGLE:
				if (mSelectedZone != nullptr) {
					mSelectedZone->mShape = zone3d::triangle;
				}
				break;
			case MENU_PROP_DLEFT:
				if (mSelectedZone != nullptr) {
					mSelectedZone->mShape = zone3d::Dleft;
				}
				break;
			case MENU_PROP_DRIGHT:
				if (mSelectedZone != nullptr) {
					mSelectedZone->mShape = zone3d::Dright;
				}
				break;
			case MENU_PROP_EXCLUDE:
				if (mSelectedZoneIdx != -1) {
					if (mSelectedSubZoneIdx != -1) {
						toggleExclusion(mSelectedZoneIdx, mSelectedSubZoneIdx);
					}
				}
				break;
			case MENU_PROP_SETRAD:
				if (mSelectedZone != nullptr) {
					flipMenu(tmpMenu, mMenuMode);
					tmpSliderVal = mSelectedZone->mRadius;
				}
				break;
			case MENU_PROP_DEPTH:
				if (mSelectedZone != nullptr) {
					flipMenu(tmpMenu, mMenuMode);
					tmpSliderVal = mSelectedZone->mLimitZ;
				}
				break;
			case MENU_PROP_USERTYPE:
				if (mSelectedZone != nullptr) {
					flipMenu(tmpMenu, mMenuMode);
					tmpSliderVal = mSelectedZone->mUserDefinedType;
				}
				break;
			case MENU_PROP_ANGLE:
				if (mSelectedZone != nullptr) {
					flipMenu(tmpMenu, mMenuMode);
					tmpSliderVal = mSelectedZone->getAngleDeg() + 180;
				}
				break;
			case MENU_UPDATE:
			case MENU_SUB_UPDATE:
			case MENU_PROP_UPDATE:
				if (mOnChange != nullptr) {
					mOnChange();
				}
				break;
			}
			if (tmpSliderVal >= 0) {
				cvSetTrackbarPos(mSliderName, mWindowName, tmpSliderVal);
			}
			adjustImage();
			showMenu(mMenuMode);
		}
		else {
			switch (mMenuSelection) {
			case MENU_SUBZONES:
			case MENU_TRIANGLE:
			case MENU_SQUARE:
				break;
			case MENU_ADD:
				addZone(x, y);
				adviseOfChange = true;
				break;
			case MENU_MOVE:
				mSelectedZoneIdx = findClickedZone(x, y, 0);
				if (mSelectedZoneIdx != -1) {
					setZoneCentre(x, y, mSelectedZoneIdx);
				}
				mClkStat.mousedown = true;
				mClkStat.startX = x;
				mClkStat.startY = y;
				break;
			case MENU_PROP_MOVE:
				if ((mSelectedZoneIdx = findClickedZone(x, y, 0)) != -1) {
					mSelectedSubZoneIdx = findClickedSubZone(x, y);
					if (mSelectedSubZoneIdx != -1) {
						setSubZoneCentre(x, y, mSelectedZoneIdx, mSelectedSubZoneIdx);
					}
					else {
						setZoneCentre(x, y, mSelectedZoneIdx);
					}
				}
				mClkStat.mousedown = true;
				mClkStat.startX = x;
				mClkStat.startY = y;
				break;

			case MENU_SUB_MOVE:
				mSelectedZone = nullptr;
				if (findClickedZoneAndSubZone(x, y)) {
					setSubZoneCentre(x, y, mSelectedZoneIdx, mSelectedSubZoneIdx);
					mClkStat.mousedown = true;
					mClkStat.startX = x;
					mClkStat.startY = y;
				}
				break;
			case MENU_PROP_DUPLICATE:
				if (mSelectedZoneIdx != -1) {
					duplicateZone(x, y, mSelectedZoneIdx, mSelectedSubZoneIdx);
				}
				break;

				//case MENU_RESIZE:
			//	mSelectedZoneIdx = findClickedZone(x, y);
			//	if (mSelectedZoneIdx != -1) {
			//		setZoneRad(x, y, mSelectedZoneIdx);
			//	}
			//	mClkStat.mousedown = true;
			//	mClkStat.startX = x;
			//	mClkStat.startY = y;
			//	break;
			case MENU_DELETE:
				{
					int zoneIdx = findClickedZone(x, y, 0);
					if (zoneIdx != -1) {
						deleteZone(zoneIdx, -1);
						mSelectedZoneIdx = -1;
					}
					mSelectedZone = nullptr;

				}
				adviseOfChange = true;
				break;
			case MENU_SUB_ADD:
				addSubZone(x, y);
				adviseOfChange = true;
				break;
			case MENU_SUB_DELETE:
				mSelectedZone = nullptr;
				if (findClickedZoneAndSubZone(x, y)) {
					deleteZone(mSelectedZoneIdx, mSelectedSubZoneIdx);
					mSelectedZoneIdx = -1;
					mSelectedSubZoneIdx = -1;
					adviseOfChange = true;
				}
				break;
			case MENU_SUB_RESIZE:
				break;
			}
			adjustImage();
			showLocData(x, y, mAdjustedImage);
			showMenu(mMenuMode);
		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		bool imageUpdateOnLButtonUp = false;
		mClkStat.mousedown = false;
		switch (mMenuSelection) {
		case MENU_SUBZONES:
		case MENU_TRIANGLE:
		case MENU_SQUARE:
		case MENU_DLEFT:
		case MENU_DRIGHT:
		case MENU_ADD:
		case MENU_MOVE:
//		case MENU_RESIZE:
		case MENU_SUB_ADD:
		case MENU_SUB_MOVE:
		case MENU_PROP_MOVE:
			mSelectedZoneIdx = -1;
			break;
		case MENU_SUB_RESIZE:
			mSelectedSubZoneIdx = -1;
			mSelectedZoneIdx = -1;
			imageUpdateOnLButtonUp = true;
			adviseOfChange = true;
			break;
		case MENU_DELETE:
			break;
		}
		if (imageUpdateOnLButtonUp) adjustImage();
	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (mClkStat.mousedown) {
			switch (mMenuSelection) {
			case MENU_MOVE:
				if (mSelectedZoneIdx != -1) {
					setZoneCentre(x, y, mSelectedZoneIdx);
				}
				break;
			case MENU_PROP_MOVE:
				if (mSelectedZoneIdx != -1) {
					if (mSelectedSubZoneIdx != -1) {
						setSubZoneCentre(x, y, mSelectedZoneIdx, mSelectedSubZoneIdx);
					}
					else {
						setZoneCentre(x, y, mSelectedZoneIdx);
					}
				}
				break;
			case MENU_SUB_MOVE:
				if (mSelectedZoneIdx != -1 && mSelectedSubZoneIdx != -1) {
					setSubZoneCentre(x, y, mSelectedZoneIdx, mSelectedSubZoneIdx);
				}
				break;
			//case MENU_RESIZE:
			//	if (mSelectedZoneIdx != -1) {
			//		setZoneRad(x, y, mSelectedZoneIdx);
			//	}
			//	break;
			}
			adjustImage();
			showLocData(x, y, mAdjustedImage);
			showMenu(mMenuMode);
		}
	}
	if (adviseOfChange && mOnChange != nullptr) {
		mOnChange();
	}
}
void zoneDisplay::trackCallback(int val) {
	if (mMenuSelection != MENU_NONE) {
		switch (mMenuSelection) {
		case MENU_SETRAD:
			mRadVal = val;
			break;
		case MENU_PROP_DEPTH:
			mPropDepthVal = val;
			if (mSelectedZone != nullptr) {
				mSelectedZone->setSize(mSelectedZone->mRadius, val);
			}
			break;
		case MENU_PROP_ANGLE:
			mPropAngleVal = val - 180;
			if (mSelectedZone != nullptr) {
				mSelectedZone->setAngleDeg(val - 180);
			}
			break;
		case MENU_PROP_SETRAD:
			mPropRadVal = val;
			if (mSelectedZone != nullptr) {
				mSelectedZone->setSize(val, mSelectedZone->mLimitZ);
			}
			break;
		case MENU_PROP_USERTYPE:
			mPropUserType = (val >= mUserDefinedTypes.size()) ? mUserDefinedTypes.size() - 1 : val ;
			if (mSelectedZone != nullptr) {
				mSelectedZone->mUserDefinedType = val;
			}
			break;
		}
		adjustImage();
		showMenu(mMenuMode);
	}

}

int zoneDisplay::menuSelected(int x, int y, menuMode whichMenu) {
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
zone3d *zoneDisplay::findZone(const char *zoneName) {
	zone3d *retv = nullptr;
	int idx = findZoneIdx(zoneName);
	if (idx >= 0 && idx < mCompoundZones->size()) {
		retv = mCompoundZones->at(idx);
	}
	return retv;
}
int zoneDisplay::findZoneIdx(const char *zoneName) {
	int retv = -1;
	for (int idx = 0; idx < mCompoundZones->size(); idx++) {
		if (strcmp(mCompoundZones->at(idx)->getName(), zoneName) == 0) {
			retv = idx;
			break;
		}
	}
	return retv;
}

char *zoneDisplay::generateDuplicateName(const char *oldName) {
	int newNameLen = strlen(oldName) + 64;
	char *newName = new char[newNameLen];
	int uniqueIdx = 0;
	bool nameIsFound = true;
	do {
		if (uniqueIdx < 1) {
 			sprintf(newName, "%sDup", oldName);
		}
		else {
 			sprintf(newName, "%sDup%d", oldName, uniqueIdx);
		}
		if (findZoneIdx(newName) >= 0) {
			uniqueIdx++;
		}
		else {
			nameIsFound = false;
		}
	} while (nameIsFound);
	return newName;
}
void zoneDisplay::duplicateZone(int pxlX, int pxlY, int selectedZoneIdx, int selectedSubZoneIdx) {
	if (selectedZoneIdx >= 0 && selectedZoneIdx < mCompoundZones->size()) {
		zone3dCompound *tmpCompoundZone = nullptr;
		zone3d *sourceZone = mCompoundZones->at(selectedZoneIdx);
		const char *sName = sourceZone->getName();
		char *newName = generateDuplicateName(sName);
		Point2f centreXY(pxlX, pxlY);
		Point2f RWcentre = pPerspective->getRWloc(centreXY);
		Point3f RWcentre3 = Point3f(RWcentre.x, RWcentre.y, 0);
		mNextZoneID++;
		tmpCompoundZone = ((zone3dCompound *)sourceZone)->duplicate(newName, &mNextZoneID);
		delete newName;
		if (tmpCompoundZone != nullptr) {
			tmpCompoundZone->setCentre(Point3f(RWcentre.x, RWcentre.y, 0));
			mCompoundZones->push_back(tmpCompoundZone);
		}
	}
}
void zoneDisplay::getUserDefinedZoneTypes(vector<string> *udzOut) {
	udzOut->clear();
	for (int idx = 0; idx < mUserDefinedTypes.size(); idx++) {
		udzOut->push_back(mUserDefinedTypes[idx]);
	}
}
void zoneDisplay::setUserDefinedZoneTypes(vector<string> *udzIn) {
	mUserDefinedTypes.clear();
	for (int idx = 0; idx < udzIn->size(); idx++) {
		mUserDefinedTypes.push_back(udzIn->at(idx));
	}
}

void zoneDisplay::addZone(int pxlX, int pxlY) {
	char name[64];
	sprintf(name, "Z_%d", mNextZoneID++);
	Point2f centreXY(pxlX, pxlY);
	Point2f RWcentre = pPerspective->getRWloc(centreXY);
	Point3f RWcentre3 = Point3f(RWcentre.x, RWcentre.y, 0);
	zone3dCompound *tmpCompoundZone = nullptr;
	zone3d::zoneShape tmpShape = zone3d::circle;
	if (mMenuSelected[MENU_TRIANGLE]) {
		tmpShape = zone3d::triangle;
	}
	else if (mMenuSelected[MENU_SQUARE]){
		tmpShape = zone3d::square;

	}
	else if (mMenuSelected[MENU_CIRCLE]){
		tmpShape = zone3d::circle;

	}
	else if (mMenuSelected[MENU_DLEFT]){
		tmpShape = zone3d::Dleft;

	}
	else if (mMenuSelected[MENU_DRIGHT]){
		tmpShape = zone3d::Dright;

	}
	tmpCompoundZone = new zone3dCompound(tmpShape, 0, RWcentre3, mRadVal, mRadVal, name, mNextZoneID);
	if (tmpCompoundZone != nullptr) {
		tmpCompoundZone->setCentre(Point3f(RWcentre.x, RWcentre.y, 0));
		tmpCompoundZone->mRadius = mRadVal;
		mCompoundZones->push_back(tmpCompoundZone);
	}
}

void zoneDisplay::addSubZone(int pxlX, int pxlY) {
	int topZoneIdx = findClickedZone(pxlX, pxlY, 0);
	if (topZoneIdx >= 0) {
		char name[64];
		sprintf(name, "SZ_%d", mNextZoneID++);
		Point2f centreXY(pxlX, pxlY);
		Point2f RWcentre = pPerspective->getRWloc(centreXY);
		Point3f RWcentre3 = Point3f(RWcentre.x, RWcentre.y, 0);
		zone3d *tmpZone = nullptr;
		zone3d::zoneShape tmpShape = zone3d::circle;
		if (mMenuSelected[MENU_TRIANGLE]) {
			tmpShape = zone3d::circle;
		}
		else if (mMenuSelected[MENU_SQUARE]) {
			tmpShape = zone3d::square;

		}
		else if (mMenuSelected[MENU_CIRCLE]) {
			tmpShape = zone3d::circle;

		}
		else if (mMenuSelected[MENU_DLEFT]) {
			tmpShape = zone3d::Dleft;

		}
		else if (mMenuSelected[MENU_DRIGHT]) {
			tmpShape = zone3d::Dright;

		}
		tmpZone = new zone3d(mCompoundZones->at(topZoneIdx), tmpShape, 0, RWcentre3, mRadValSub, 2 * mRadValSub, name, mNextZoneID);
		if (tmpZone != nullptr) {
			tmpZone->setCentre(Point3f(RWcentre.x, RWcentre.y, 0));
			mCompoundZones->at(topZoneIdx)->addZone(tmpZone);
		}
	}
}

bool zoneDisplay::zoneIdxIsValid(int zIdx, int szIdx, bool checkSubIdx) {
	bool retv = false;
	if (zIdx >= 0 && zIdx < mCompoundZones->size()) {
		if (!checkSubIdx) {
			retv = true; // ignore sub idx for validity test
		}
		else {
			if (szIdx >= 0 && szIdx < mCompoundZones->at(zIdx)->subZones()->size()) {
				retv = true;
			}
		}
	}
	return retv;
}
void zoneDisplay::toggleExclusion(int zIdx, int szIdx) {
	if (zIdx >= 0 && zIdx < mCompoundZones->size()) {
		if (szIdx >= 0 && szIdx < mCompoundZones->at(zIdx)->subZones()->size()) {
			mCompoundZones->at(zIdx)->subZones()->at(szIdx)->mExclusion = !getExclusion(zIdx, szIdx);
		}
	}

}
bool zoneDisplay::getExclusion(int zIdx, int szIdx) {
	bool retv = false;
	if (zIdx >= 0 && zIdx < mCompoundZones->size()) {
		if (szIdx >= 0 && szIdx < mCompoundZones->at(zIdx)->subZones()->size()) {
			retv = mCompoundZones->at(zIdx)->subZones()->at(szIdx)->mExclusion;
		}
	}
	return retv;
}

void zoneDisplay::deleteZone(int zIdx, int szIdx) {
	if (zoneIdxIsValid(zIdx, szIdx, (szIdx >= 0))) {
		if (szIdx >= 0) {
			mCompoundZones->at(zIdx)->removeZone(szIdx);
		}
		else {
			delete mCompoundZones->at(zIdx); // delete the zone
			mCompoundZones->erase(mCompoundZones->begin() + zIdx); // remove list entry
		}
	}
}

void zoneDisplay::setZoneCentre(int pxlX, int pxlY, int idx) {
	if (idx >= 0 && idx < mCompoundZones->size()) {
		Point2f centreXY(pxlX, pxlY);
		Point2f RWcentre = pPerspective->getRWloc(centreXY);
		mCompoundZones->at(idx)->setCentre(Point3f(RWcentre.x, RWcentre.y, 0));
	}
}
void zoneDisplay::setSubZoneCentre(int pxlX, int pxlY, int idx, int subIdx) {
	if (idx >= 0 && idx < mCompoundZones->size()) {
		Point2f centreXY(pxlX, pxlY);
		Point2f RWcentre = pPerspective->getRWloc(centreXY);
		mCompoundZones->at(idx)->subZones()->at(subIdx)->setCentre(Point3f(RWcentre.x, RWcentre.y, 0));
	}
}
void zoneDisplay::setZoneRad(int pxlX, int pxlY, int idx) {
	if (idx >= 0 && idx < mCompoundZones->size()) {
		Point2f centreEdgeXY(pxlX, pxlY);
		Point2f RWedge = pPerspective->getRWloc(centreEdgeXY);
		float deltax = RWedge.x - mCompoundZones->at(idx)->Centre().x;
		deltax *= deltax;
		float deltay = RWedge.y - mCompoundZones->at(idx)->Centre().y;
		deltay *= deltay;
		mCompoundZones->at(idx)->mRadius = Math::Sqrt(deltax + deltay);
	}
}

void zoneDisplay::showLocData(int pxlX, int pxlY, Mat adjustedImage) {
	stringstream ss;
	//rectangle(HSVI10mage, cv::Point(10, 2), cv::Point(300, 20),
	//	cv::Scalar(255, 255, 255), -1);
	Point2f rwLoc = pPerspective->getRWloc(Point2f(pxlX, pxlY));
	ss << pxlX << " " << pxlY << " " << rwLoc.x << " " << rwLoc.y;
	string frameNumberString = ss.str();
	putText(adjustedImage, frameNumberString.c_str(), cv::Point(MENU_PANEL_WIDTH + 15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
}


void zoneDisplay::showImg(Mat img, perspective *pPersp, std::vector<zone3dCompound *> *zones, zonesChangedCallback onChange) {
	mOrgImage = img;
	mRows = img.rows;
	mCols = img.cols;
	mCompoundZones = zones;
	mOnChange = onChange;
	mNextZoneID = 0;
	for (int idx = 0; idx < mCompoundZones->size(); idx++) {
		int tmpMaxID = mCompoundZones->at(idx)->highestID();
		if (tmpMaxID > mNextZoneID) mNextZoneID = tmpMaxID;
	}
	mNextZoneID++;
	pPerspective = pPersp;
	adjustImage();
	imshow(mWindowName, mAdjustedImage);
	setMouseCallback(mWindowName, zoneDisplay::mouseCallback, this);
	createTrackbar(mSliderName, mWindowName, &mTrackVal, 360, zoneDisplay::trackCallback, this);
	showMenu(mMenuMode);
}
void zoneDisplay::hide() {
	destroyWindow(mWindowName);
}

void zoneDisplay::drawSubZone(Mat img, zone3d *pZn, bool selected) {
	drawZoneOutline(img, pZn, (selected) ? Scalar(0, 255, 0) : Scalar(128, 128, 128), (selected) ? 2 : 1);
	drawZoneName(img, pZn);
}

void zoneDisplay::drawZoneOutline(Mat img, zone3d *pZn, Scalar clr, int width) {

	Point2f pxlLoc = pPerspective->getAdjustedPictureloc( Point2f(pZn->Centre().x, pZn->Centre().y));
	Point2f pxlTangentLoc = pPerspective->getAdjustedPictureloc( Point2f(pZn->Centre().x + pZn->mRadius, pZn->Centre().y));
	int radPxls = pxlTangentLoc.x - pxlLoc.x;
	vector<Point2f> *corners;
	if (pZn->mExclusion) width *= 2;
	switch (pZn->mShape) {
	case zone3d::circle :
		circle(img, pxlLoc, radPxls, clr, width);
		break;
	case zone3d::Dleft :
		{
			corners = pZn->getCorners();
			if (corners->size() == 2) {
				ellipse(img, pxlLoc, Size(radPxls, radPxls), -(pZn->getAngleDeg()), 90, 270, clr, width);
				line(img, pPerspective->getAdjustedPictureloc(corners->at(0)), pPerspective->getAdjustedPictureloc(corners->at(1)), clr, width);
			}
		}
		break;
	case zone3d::Dright :
		{
			corners = pZn->getCorners();
			if (corners->size() == 2) {
				ellipse(img, pxlLoc, Size(radPxls, radPxls), 180 - pZn->getAngleDeg(), 90, 270, clr, width);
				line(img, pPerspective->getAdjustedPictureloc(corners->at(0)), pPerspective->getAdjustedPictureloc(corners->at(1)), clr, width);
			}
		}
		break;
	case zone3d::square :
		corners = pZn->getCorners();
		if (corners->size() == 4) {
			//Point2f pCentre = Point2f(pZn->Centre().x, pZn->Centre().y);
			//Point2f pCirecleTop = Point2f(pZn->Centre().x, pZn->Centre().y + pZn->mRadius);
			//Point2f pCirecleRight = Point2f(pZn->Centre().x + pZn->mRadius, pZn->Centre().y);
			//Point2f pCentrePxl = pPerspective->getAdjustedPictureloc(pCentre);
			//Point2f pCirecleTopPxl = pPerspective->getAdjustedPictureloc(pCirecleTop);
			//Point2f pCirecleRightPxl = pPerspective->getAdjustedPictureloc(pCirecleRight);
			//Point2f pSquareTopPxl = pPerspective->getAdjustedPictureloc(corners->at(0));
			//Point2f pSquareRightPxl = pPerspective->getAdjustedPictureloc(corners->at(1));

			line(img, pPerspective->getAdjustedPictureloc(corners->at(0)), pPerspective->getAdjustedPictureloc(corners->at(1)), clr, width);
			line(img, pPerspective->getAdjustedPictureloc(corners->at(1)), pPerspective->getAdjustedPictureloc(corners->at(2)), clr, width);
			line(img, pPerspective->getAdjustedPictureloc(corners->at(2)), pPerspective->getAdjustedPictureloc(corners->at(3)), clr, width);
			line(img, pPerspective->getAdjustedPictureloc(corners->at(3)), pPerspective->getAdjustedPictureloc(corners->at(0)), clr, width);
		}
		delete corners;
		break;
	case zone3d::triangle :
		corners = pZn->getCorners();
		if (corners->size() == 3) {
			line(img, pPerspective->getAdjustedPictureloc(corners->at(0)), pPerspective->getAdjustedPictureloc(corners->at(1)), clr, width);
			line(img, pPerspective->getAdjustedPictureloc(corners->at(1)), pPerspective->getAdjustedPictureloc(corners->at(2)), clr, width);
			line(img, pPerspective->getAdjustedPictureloc(corners->at(2)), pPerspective->getAdjustedPictureloc(corners->at(0)), clr, width);
		}
		delete corners;
		break;
	}
}
void zoneDisplay::drawZone(Mat img, zone3dCompound *pZn, bool selected) {
	Point2f pxlLoc = pPerspective->getAdjustedPictureloc( Point2f(pZn->Centre().x, pZn->Centre().y));
	Point2f pxlTangentLoc = pPerspective->getAdjustedPictureloc( Point2f(pZn->Centre().x + pZn->mRadius, pZn->Centre().y));
	int radPxls = pxlTangentLoc.x - pxlLoc.x;
	drawZoneOutline(img, pZn, (selected) ? Scalar(255, 0, 0) : Scalar(0, 0, 0), (selected) ? 2 : 1);
	for (int subIdx = 0; subIdx < pZn->subZones()->size(); subIdx++) {
		drawSubZone(img, pZn->subZones()->at(subIdx), selected && (mSelectedSubZoneIdx == subIdx));
	}
	drawZoneName(img, pZn);
}

void zoneDisplay::drawZoneName(Mat img, zone3d *pZn) {
	int nameX = pZn->Centre().x - pZn->mRadius;
	int nameY = pZn->Centre().y - pZn->mRadius;
	Point2f imageLoc = pPerspective->getAdjustedPictureloc(Point2f(nameX, nameY));
	putText(img, pZn->mName, cv::Point(imageLoc.x + 5, imageLoc.y - MENU_PANEL_LINE_HEIGHT + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
}
void zoneDisplay::adjustImage() {
	mOrgImage.copyTo(mAdjustedImage);

	pPerspective->warp(mOrgImage, mAdjustedImage);
	drawZonesOnAdjustedImage(mAdjustedImage);
}
void zoneDisplay::drawZonesOnAdjustedImage(Mat img) {
	if (mCompoundZones != nullptr) {
		for (int idx = 0; idx < mCompoundZones->size(); idx++) {
			drawZone(img, mCompoundZones->at(idx), mSelectedZoneIdx == idx);
		}
	}
}
int zoneDisplay::findClickedZone(int pxlX, int pxlY, int minIdx) {
	int retv = -1;
	Point2f centreXY(pxlX, pxlY);
	Point2f RWcentre = pPerspective->getRWloc(centreXY);

	mSelectedZone = nullptr;
	for (int idx = minIdx; idx < mCompoundZones->size(); idx++) {
		if (mCompoundZones->at(idx)->isWithinIgnoreExcludes(RWcentre.x, RWcentre.y, 0)) {
			retv = idx;
			mSelectedZone = mCompoundZones->at(idx);
			break;
		}
	}
	return retv;
}

int zoneDisplay::findClickedSubZone(int pxlX, int pxlY) {
	int retv = -1;
	if (mSelectedZoneIdx >= 0 && mSelectedZoneIdx < mCompoundZones->size()) {
		Point2f centreXY(pxlX, pxlY);
		Point2f RWcentre = pPerspective->getRWloc(centreXY);

		for (int idx = 0; idx < mCompoundZones->at(mSelectedZoneIdx)->subZones()->size(); idx++) {
			if (mCompoundZones->at(mSelectedZoneIdx)->subZones()->at(idx)->isWithin(RWcentre)) {
				retv = idx;
				mSelectedZone = mCompoundZones->at(mSelectedZoneIdx)->subZones()->at(idx);
				break;
			}
		}
	}
	return retv;
}
bool zoneDisplay::findClickedZoneAndSubZone(int pxlX, int pxlY) {
	bool retv = false;
	mSelectedZoneIdx = findClickedZone(pxlX, pxlY, 0);
	if (mSelectedZoneIdx != -1) {
		mSelectedZone = mCompoundZones->at(mSelectedZoneIdx);
		mSelectedSubZoneIdx = findClickedSubZone(pxlX, pxlY);
		if (mSelectedSubZoneIdx != -1) {
			mSelectedZone = mCompoundZones->at(mSelectedZoneIdx)->subZones()->at(mSelectedSubZoneIdx);
			retv = true;
		}
	}
	return retv;
}

