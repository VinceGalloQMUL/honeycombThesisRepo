
#include "cvUtilities.h"
#include "csvFile.h"
#include "superimposeAreaDisplay.h"

superimposeAreaDisplay::superimposeAreaDisplay(char *windowName, Mat combImage, Mat combImagePrevious, int dispRows, int dispCols, float dispDivision)
	: mWindowName(nullptr)
{
	superimposeAreaDisplayCommon(windowName, combImage, dispRows, dispCols, dispDivision);
	combImagePrevious.copyTo(mCombImagePrevious);
	mCombImagePrevious.copyTo(mRescaledCombImagePrevious);
	resizeFrame(&mRescaledCombImagePrevious, &mRescaledCombImagePrevious);
	mHavePrevious = true;
}
superimposeAreaDisplay::superimposeAreaDisplay(char *windowName, Mat combImage, int dispRows, int dispCols, float dispDivision)
	: mWindowName(nullptr)
{
	superimposeAreaDisplayCommon(windowName, combImage, dispRows, dispCols, dispDivision);
}
void superimposeAreaDisplay::superimposeAreaDisplayCommon(char *windowName, Mat combImage, int dispRows, int dispCols, float dispDivision)
{
	mWindowName = new char[strlen(windowName) + 1];
	mConfigFilename = "";
	mConfigFilenamePrevious = "";
	mHavePrevious = false;
	mPreviousCells = false;
	mContrast = 0.5f;
	mShowDelta = false;
	strcpy(mWindowName, windowName);
	combImage.copyTo(mCombImage);
	mCols = mCombImage.cols;
	mRows = mCombImage.rows;
	mDispDivision = dispDivision;
	if (dispDivision <= 0) {
		mDispDivision = 1;
	}
	mDispCols = mCols / mDispDivision;
	mDispRows = mRows / mDispDivision;
	mCombImage.copyTo(mRescaledCombImage);
	resizeFrame(&mRescaledCombImage, &mRescaledCombImage);

	mSliderContrast = "Contrast";
	mMenuMode = topMenu;

	
	mMenuTitle[0] = "| img0:img1 |";
	mMenuTitle[1] = "| Ref Locs. |";
	mMenuTitle[2] = "| Geometries |";

	mMenuTypeStr[MENU_BROODIDX_EGG] = "Egg";
	mMenuTypeStr[MENU_BROODIDX_LARVA] = "Larva";
	mMenuTypeStr[MENU_BROODIDX_PUPA] = "Pupa";
	mMenuTypeStr[MENU_BROODIDX_NOT_EGG] = "!Egg";
	mMenuTypeStr[MENU_BROODIDX_NOT_LARVA] = "!Larva";
	mMenuTypeStr[MENU_BROODIDX_NOT_PUPA] = "!Pupa";
	mMenuTypeStr[MENU_BROODIDX_EGG_NOT_LARVA] = "Egg!Larva";
	mMenuTypeStr[MENU_BROODIDX_PUPA_NOT_LARVA] = "Pupa!Larva";
	mMenuTypeStr[MENU_BROODIDX_LARVA_NOT_PUPA] = "Larva!Pupa";
	mMenuTypeStr[MENU_BROODIDX_LARVA_NOT_EGG] = "Larva!Egg";
	mMenuTypeIdx = MENU_BROODIDX_PUPA;

	mMenuStr[MENU_0] = "0:100";
	mMenuSelected[MENU_0] = false;
	mMenuStr[MENU_25] = "25:75";
	mMenuSelected[MENU_25] = false;
	mMenuStr[MENU_50] = "50:50";
	mMenuSelected[MENU_50] = false;
	mMenuStr[MENU_75] = "75:25";
	mMenuSelected[MENU_75] = false;
	mMenuStr[MENU_100] = "100:0";
	mMenuSelected[MENU_100] = false;

	mMenuStr[MENU_DISTORT] = "Reshape";
	mMenuSelected[MENU_DISTORT] = false;
	mMenuStr[MENU_LOCS] = "Locs";
	mMenuSelected[MENU_LOCS] = false;
	mMenuStr[MENU_MENU_CORNER] = "Menu Corner";
	mMenuSelected[MENU_MENU_CORNER] = false;
	mMenuStr[MENU_GEO] = "Geometry";
	mMenuSelected[MENU_GEO] = false;
	mMenuStr[MENU_SAVE] = "Save";
	mMenuSelected[MENU_SAVE] = false;

	mMenuStr[MENU_SUB_LOC0] = "Loc 1";
	mMenuSelected[MENU_SUB_LOC0] = false;
	mMenuStr[MENU_SUB_LOC1] = "Loc 2";
	mMenuSelected[MENU_SUB_LOC1] = false;
	mMenuStr[MENU_SUB_LOC2] = "Loc 3";
	mMenuSelected[MENU_SUB_LOC2] = false;
	mMenuStr[MENU_SUB_LOC3] = "Loc 4";
	mMenuSelected[MENU_SUB_LOC3] = false;
	mMenuStr[MENU_SUB_DONE] = "Done";
	mMenuSelected[MENU_SUB_DONE] = false;

//#define MENU_GEO_END (0 + MENU_QTY + MENU_SUB_QTY)
//#define MENU_GEO_SIDE (1 + MENU_QTY + MENU_SUB_QTY)
//#define MENU_GEO_BENT (2 + MENU_QTY + MENU_SUB_QTY)
//#define MENU_GEO_DONE (4 + MENU_QTY + MENU_SUB_QTY)
	mMenuStr[MENU_GEO_0] = "0:100";
	mMenuSelected[MENU_GEO_0] = false;
	mMenuStr[MENU_GEO_25] = "25:75";
	mMenuSelected[MENU_GEO_25] = false;
	mMenuStr[MENU_GEO_50] = "50:50";
	mMenuSelected[MENU_GEO_50] = false;
	mMenuStr[MENU_GEO_75] = "75:25";
	mMenuSelected[MENU_GEO_75] = false;
	mMenuStr[MENU_GEO_100] = "100:0";
	mMenuSelected[MENU_GEO_100] = false;

	mMenuStr[MENU_GEO_SCALE] = "Scale";
	mMenuSelected[MENU_GEO_SCALE] = false;
	mMenuStr[MENU_GEO_TYPE] = mMenuTypeStr[mMenuTypeIdx];
	mMenuSelected[MENU_GEO_TYPE] = false;
	mMenuStr[MENU_GEO_DRONE] = "Drone";
	mMenuSelected[MENU_GEO_DRONE] = false;
	mMenuStr[MENU_GEO_POINT] = "Point";
	mMenuSelected[MENU_GEO_POINT] = false;
	mMenuStr[MENU_GEO_LINE] = "Line";
	mMenuSelected[MENU_GEO_LINE] = false;
	mMenuStr[MENU_GEO_TRI] = "Triangle";
	mMenuSelected[MENU_GEO_TRI] = false;
	mMenuStr[MENU_GEO_QUAD] = "Quad";
	mMenuSelected[MENU_GEO_QUAD] = false;
	mMenuStr[MENU_GEO_PT0] = "Point 0";
	mMenuSelected[MENU_GEO_PT0] = false;
	mMenuStr[MENU_GEO_PT1] = "Point 1";
	mMenuSelected[MENU_GEO_PT1] = false;
	mMenuStr[MENU_GEO_PT2] = "Point 2";
	mMenuSelected[MENU_GEO_PT2] = false;
	mMenuStr[MENU_GEO_PT3] = "Point 3";
	mMenuSelected[MENU_GEO_PT3] = false;
	mMenuStr[MENU_GEO_DELETE] = "Delete";
	mMenuSelected[MENU_GEO_DELETE] = false;
	mMenuStr[MENU_GEO_COUNT] = "Count";
	mMenuSelected[MENU_GEO_COUNT] = false;
	mMenuStr[MENU_GEO_DELTA] = "Delta";
	mMenuSelected[MENU_GEO_DELTA] = false;
	mMenuStr[MENU_GEO_PREVIOUS] = "Prev";
	mMenuSelected[MENU_GEO_PREVIOUS] = false;

	mMenuStr[MENU_GEO_DONE] = "Done";
	mMenuSelected[MENU_GEO_DONE] = false;

	mMenuOrg = Point(0, (mDispRows / 2) - 50);
	mTrackVal = 50;
	mGeoBeingEdited = -1;
//	mAreaDescriptions.push_back(new areaDesc());
	namedWindow(mWindowName, WINDOW_NORMAL);
	resizeWindow(mWindowName, mDispCols, mDispRows);
	showImg();
	setMouseCallback(mWindowName, superimposeAreaDisplay::mouseCallback, this);
}
superimposeAreaDisplay::~superimposeAreaDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;
	destroyWindow(mWindowName);
}
void superimposeAreaDisplay::setScaleDistance(float dist) {
	mScaleDistanceCells = (int)(dist);
}
void superimposeAreaDisplay::setDetectionRange(float dist) {
	mDetectionRangeSq = dist;
}
float superimposeAreaDisplay::getScaleDistance() {
	return mScaleDistanceCells;
}

void superimposeAreaDisplay::setConfigFile(const char* pConfigFilename) {
	mConfigFilename = string(pConfigFilename);
	loadConfigFile();
	rescaleSuperImage();
	showImg();
}

void superimposeAreaDisplay::setConfigFile(const char* pConfigFilename, const char* pConfigFilenamePrevious) {
	mConfigFilename = string(pConfigFilename);
	mConfigFilenamePrevious = string(pConfigFilenamePrevious);
	loadConfigFile();
	rescaleSuperImage();
	showImg();
}

void superimposeAreaDisplay::loadConfigFile() {
	if (mHavePrevious) {
		mConfigPrevious.read(mConfigFilenamePrevious.c_str(), &mAreaDescriptionsPrevious);
		mPerspectiveMobilePrevious.clone(mConfigPrevious.mobileRefs);
	}
	mConfig.read(mConfigFilename.c_str(), &mAreaDescriptions);
	mHive = mConfig.hive;
	mScaleDistanceCells = mConfig.mScaleDist;
	mScaleCorner = mConfig.mScaleCorner;
	mScaleHorizntal = mConfig.mScaleHorizontal;
	mScaleDiagonal = mConfig.mScaleDiagonal;
	mPerspectiveMobile.clone(mConfig.mobileRefs);

}
void superimposeAreaDisplay::saveConfigFileReq(int *totals) {
	mMenuSelected[MENU_DISTORT] = mApplyDistortion = true;
	showImg();
	saveConfigFile(totals);
}

int superimposeAreaDisplay::calcCount(int whichGeo) {
	mMenuSelected[MENU_DISTORT] = mApplyDistortion = true;
	showImg();
	mConfig.hive = mHive;
	mConfig.mScaleDist = mScaleDistanceCells;
	mConfig.mScaleCorner = mScaleCorner;
	mConfig.mScaleHorizontal = mScaleHorizntal;
	mConfig.mScaleDiagonal = mScaleDiagonal;
	mConfig.mobileRefs.clone(mPerspectiveMobile);
	return mConfig.calcSize(whichGeo, &mAreaDescriptions, mPersp, mDispDivision, mScaleDistanceCells, mScaleCorner, mScaleHorizntal, mScaleDiagonal, mDispCols, mDispRows);

}

void superimposeAreaDisplay::saveConfigFile(int *totals) {
	std::vector<Point2f> newDots;
	std::vector<Point2f> missingDots;
	std::vector<Point2f> coincidingDots;
	float pixelsPerCellHor = mDispRows / mPersp.fullImageCols; // NUMCELLSWIDTH;
	float pixelsPerCellHeight = mDispCols / mPersp.fullImageRows; // NUMCELLSHEIGHTBROOD;

	intermediateDotsDelta(pixelsPerCellHor, pixelsPerCellHeight, newDots, missingDots, coincidingDots);
	int sum = totals[8] = newDots.size();
	sum += (totals[9] = missingDots.size());
	sum += (totals[10] = coincidingDots.size());
	totals[11] = sum; // place the computed deltas into the top section of teh totals, these will be written to the file by the config.write method
	mConfig.hive = mHive;
	mConfig.mScaleDist = mScaleDistanceCells;
	mConfig.mScaleCorner = mScaleCorner;
	mConfig.mScaleHorizontal = mScaleHorizntal;
	mConfig.mScaleDiagonal = mScaleDiagonal;
	mConfig.mobileRefs.clone(mPerspectiveMobile);
	mConfig.write(mConfigFilename.c_str(), &mAreaDescriptions, mPersp, mDispDivision, mScaleDistanceCells, mScaleCorner, mScaleHorizntal, mScaleDiagonal, mDispCols, mDispRows, totals);
}

string superimposeAreaDisplay::getHive() {
	return mHive;
}
void superimposeAreaDisplay::setHive(string hive) {
	mHive = hive;
}

void superimposeAreaDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT + mMenuOrg.y;
	int x = mMenuOrg.x;
	rectangle(mDisplayImage, cv::Point(x, y), cv::Point(x + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(x + 5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void superimposeAreaDisplay::menuQtyString(char *strBuf, int menuIdx, menuMode whichMenu) {
	switch (menuIdx) {
	default:
		*strBuf = '\0';
		break;
	//case MENU_NULL:
	//	break;
	}
}
void superimposeAreaDisplay::showMenu(menuMode whichMenu) {
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
	case reflocs:
		showMenuLine(0, (mStaticPointsNotMobile) ? "StaticRefs" : "MobileRefs"); //mMenuTitle[1]);
		while (y < MENU_SUB_QTY) {
			menuQtyString(&menuVal0[0], y + MENU_QTY, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY] ? '*' : ' ');
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	case geometries:
		showMenuLine(0, mMenuTitle[2]);
		while (y < MENU_GEO_QTY) {
			menuQtyString(&menuVal0[0], y + MENU_QTY + MENU_SUB_QTY, whichMenu);
			if (MENU_GEO_COUNT == y + MENU_QTY + MENU_SUB_QTY) {
				if (mMenuSelected[y + MENU_QTY + MENU_SUB_QTY])
					sprintf(menuLine, "%s %s %d", mMenuStr[y + MENU_QTY + MENU_SUB_QTY], &menuVal0[0], whichGeoCount);
				else
					sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY + MENU_SUB_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY + MENU_SUB_QTY] ? '*' : ' ');
			}
			else {
				sprintf(menuLine, "%s %s %c", mMenuStr[y + MENU_QTY + MENU_SUB_QTY], &menuVal0[0], mMenuSelected[y + MENU_QTY + MENU_SUB_QTY] ? '*' : ' ');
			}
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	}
	imshow(mWindowName, mDisplayImage);
}


void superimposeAreaDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	superimposeAreaDisplay* psuperimposeAreaDisplay = (superimposeAreaDisplay*)userdata;
	psuperimposeAreaDisplay->mouseCallback(event, x, y, flags);
}
void superimposeAreaDisplay::trackCallback(int val, void* userdata) {
	superimposeAreaDisplay* psuperimposeAreaDisplay = (superimposeAreaDisplay*)userdata;
	psuperimposeAreaDisplay->trackCallback(val);
}

void superimposeAreaDisplay::clearAndSetMenu(int menuSelection, menuMode whichMenu) {
	if (mMenuSelection != MENU_NONE) {
		mMenuSelected[mMenuSelection] = false;
	}
	mMenuSelection = menuSelection;
	if (menuSelection != MENU_NONE) {
		mMenuSelected[menuSelection] = true;
	}
}
void superimposeAreaDisplay::flipMenu(int menuSelection, menuMode whichMenu) {
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
void superimposeAreaDisplay::clearMenuSelections() {
	for (int idx = 0; idx < MENU_ALL_QTY; idx++) {
		mMenuSelected[idx] = false;
	}
}
void superimposeAreaDisplay::clearMenuLocSelections() {
}
int distanceBetween(Point2f pt0, Point2f pt1) {
	int distX = pt0.x - pt1.x;
	int distY = pt0.y - pt1.y;
	return distX * distX + distY * distY;
}
int superimposeAreaDisplay::findClosestGeometry(Point testLoc) {
	int retv = -1;
	int minDist = -1;
	for (int idx = 0; idx < mAreaDescriptions.size(); idx++) {
		if (!(mAreaDescriptions[idx]->isEmpty()) && mAreaDescriptions[idx]->mDelta == mShowDelta) {
			int tmpDist = distanceBetween(testLoc, mAreaDescriptions[idx]->pt0);
			if (minDist < 0 || minDist > tmpDist) {
				minDist = tmpDist;
				retv = idx;
			}
		}
	}
	return retv;
}
void superimposeAreaDisplay::removeGeometry(int whichOne) {
	if (mAreaDescriptions.size() > 1) {
		if (whichOne >= 0 && whichOne < mAreaDescriptions.size()) {
			mAreaDescriptions.erase(mAreaDescriptions.begin() + whichOne);
		}
	}
	else {
		//mAreaDescriptions[0]->emptyEntry();
		mAreaDescriptions.clear();
	}
}
void superimposeAreaDisplay::geoEntryDone() {
	if (mGeoBeingEdited >= 0) {
		mGeoBeingEdited = -1;

	}
	else { // otherwise, this is an entry
		// nothing to do yet
	}
}


void superimposeAreaDisplay::setRefLocation(int whichPoint, bool staticNotMobile, int x, int y) {
	Point2f rescaledPoint = unResizePoint(staticNotMobile, Point2f(x, y), mApplyDistortion);
	//if (!staticNotMobile && mApplyDistortion) {
	//	rescaledPoint = mPersp.adjustedPerspective(rescaledPoint, true, true);
	//}
	x = rescaledPoint.x;
	y = rescaledPoint.y;
	perspectiveImageReference *relevantPerspective = &mPerspectiveMobile;
	switch (whichPoint) {
	case 0:
		relevantPerspective->orgTL.x = x;
		relevantPerspective->orgTL.y = y;
		break;
	case 1:
		relevantPerspective->orgTR.x = x;
		relevantPerspective->orgTR.y = y;
		break;
	case 2:
		relevantPerspective->orgBR.x = x;
		relevantPerspective->orgBR.y = y;
		break;
	case 3:
		relevantPerspective->orgBL.x = x;
		relevantPerspective->orgBL.y = y;
		break;

	}
}
void superimposeAreaDisplay::flagRefLocationAsSet(int whichPoint) {
	bool *relevantFlags = &mHaveMobileLocs[0];
	relevantFlags[whichPoint] = true;
	bool allAreSet = relevantFlags[0] && relevantFlags[1] && relevantFlags[2] && relevantFlags[3];
	mHaveAllMobileLocs = allAreSet;
}
areaDesc::shape superimposeAreaDisplay::getGeometryTypeBeingEdited() {
	areaDesc::shape retv;
	retv = getCurrentAreaLocation()->mShape;
	return retv;
}
areaDesc* superimposeAreaDisplay::getCurrentAreaLocation() {
	areaDesc* retv = &mTmpAreaDesc;
	//if (mGeoBeingEdited >= 0) {
	//	retv = mAreaDescriptions.at(mGeoBeingEdited);
	//}
	return retv;
}

void	superimposeAreaDisplay::setAreaLocation(areaDesc *pCurrentArea, int whichPoint, int x, int y) {
	Point2f adjustedPt = Point(0, 0);
	bool clearPt = true;
	if (x > 0 && y > 0) {
		adjustedPt = unResizePoint(false, Point2f(x, y), mApplyDistortion);
		clearPt = false;
	}
	switch (whichPoint) {
		case MENU_GEO_PT0 - MENU_GEO_PT0:
			pCurrentArea->pt0 = adjustedPt;
			break;
		case MENU_GEO_PT1 - MENU_GEO_PT0:
			pCurrentArea->pt1 = adjustedPt;
			break;
		case MENU_GEO_PT2 - MENU_GEO_PT0:
			pCurrentArea->pt2 = adjustedPt;
			break;
		case MENU_GEO_PT3 - MENU_GEO_PT0:
			pCurrentArea->pt3 = adjustedPt;
			break;
	}
}


void superimposeAreaDisplay::mouseCallback(int event, int x, int y, int flags) {
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
			case MENU_0:
			case MENU_GEO_0:
				mContrast = 0;
				break;
			case MENU_25:
			case MENU_GEO_25:
				mContrast = 0.35f;
				break;
			case MENU_50:
			case MENU_GEO_50:
				mContrast = 0.50f;
				break;
			case MENU_75:
			case MENU_GEO_75:
				mContrast = 0.65f;
				break;
			case MENU_100:
			case MENU_GEO_100:
				mContrast = 1;
				break;
			case MENU_DISTORT:
				mMenuSelected[MENU_DISTORT] = mApplyDistortion = !mApplyDistortion;
				break;
			case MENU_LOCS:
				clearMenuSelections();
				mMenuMode = menuMode::reflocs;
				mWhichPoint = -2;
				mStaticPointsNotMobile = (tmpMenu == MENU_LOCS);
				mMenuSelection = MENU_NONE;
				break;
			case MENU_SAVE:
				mMenuSelected[MENU_DISTORT] = mApplyDistortion = true;
				showImg();
				int sink[12];
				saveConfigFile(sink);
				break;
			case MENU_SUB_LOC0:
			case MENU_SUB_LOC1:
			case MENU_SUB_LOC2:
			case MENU_SUB_LOC3:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				mWhichPoint = tmpMenu - MENU_SUB_LOC0;
				break;
			case MENU_GEO_TYPE:
				if (mMenuTypeIdx == MENU_BROODIDX_PUPA && mShowDelta) {
					mMenuTypeIdx = MENU_BROODIDX_NOT_PUPA;
				}
				else {
					mMenuTypeIdx++;

				}
				if (mMenuTypeIdx >= MENU_BROODIDX_QTY) {
					mMenuTypeIdx = 0;
				}
				mMenuStr[MENU_GEO_TYPE] = mMenuTypeStr[mMenuTypeIdx];
				getCurrentAreaLocation()->mType = mMenuTypeIdx;
				break;

			case MENU_GEO_SCALE:
				if (primaryClickStatus.menuChoice == tmpMenu) {
					// repeated click, so cancel
					clearMenuLocSelections();
					mMenuSelected[MENU_GEO_SCALE] = false;
					primaryClickStatus.menuChoice = MENU_NONE;
					mWhichPoint = -1;

				}
				else {
					clearMenuLocSelections();
					mMenuSelected[MENU_GEO_SCALE] = true;
					primaryClickStatus.menuChoice = MENU_GEO_SCALE;
					mWhichPoint = 0;

				}
				break;
			case MENU_GEO_POINT:
			case MENU_GEO_LINE:
			case MENU_GEO_TRI:
			case MENU_GEO_QUAD:
				mTmpAreaDesc.emptyEntry();
				mTmpAreaDesc.mShape = (tmpMenu == MENU_GEO_POINT) ? areaDesc::shape::point :(tmpMenu == MENU_GEO_LINE) ? areaDesc::shape::line :(tmpMenu == MENU_GEO_TRI) ? areaDesc::shape::tri : areaDesc::shape::quad;
				mTmpAreaDesc.mType = mMenuTypeIdx;
				mTmpAreaDesc.mDrone = mMenuSelected[MENU_GEO_DRONE];
				mTmpAreaDesc.mDelta = mMenuSelected[MENU_GEO_DELTA];
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_PT0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_PT0;
				mWhichPoint = 0;
				break;
			case MENU_GEO_DRONE:
				mTmpAreaDesc.mDrone = mMenuSelected[MENU_GEO_DRONE] = !mMenuSelected[MENU_GEO_DRONE];
				break;
			case MENU_GEO_DELTA:
				mTmpAreaDesc.mDelta = mShowDelta = mMenuSelected[MENU_GEO_DELTA] = !mShowDelta;
				break;
			case MENU_GEO_PREVIOUS:
				mTmpAreaDesc.mDelta = mPreviousCells = mMenuSelected[MENU_GEO_PREVIOUS] = !mPreviousCells;
				break;
			case MENU_GEO_PT0:
			case MENU_GEO_PT1:
			case MENU_GEO_PT2:
			case MENU_GEO_PT3:
				if (mMenuSelected[tmpMenu]) {
					setAreaLocation(getCurrentAreaLocation(), mWhichPoint, 0, 0);
				}
				else {
					clearMenuLocSelections();
					mMenuSelected[tmpMenu] = true;
					primaryClickStatus.menuChoice = tmpMenu;
					mWhichPoint = tmpMenu - MENU_GEO_PT0;
				}
				break;
			case MENU_GEO_COUNT:
			case MENU_GEO_DELETE:
			case MENU_MENU_CORNER:
				if (mMenuSelected[tmpMenu]) {
					mMenuSelected[tmpMenu] = false;
					mMenuSelection = MENU_NONE;
					mGeoBeingEdited = -1;
					primaryClickStatus.menuChoice = MENU_NONE;
				}
				else {
					clearMenuLocSelections();
					mMenuSelected[tmpMenu] = true;
					primaryClickStatus.menuChoice = tmpMenu;
				}
				break;
			case MENU_GEO:
				clearMenuSelections();
				mWhichPoint = -1;
				mMenuMode = menuMode::geometries;
				mMenuSelection = MENU_NONE;
				break;
			case MENU_GEO_DONE:
			case MENU_SUB_DONE:
				clearMenuSelections();
				mMenuMode = menuMode::topMenu;
				mMenuSelection = MENU_NONE;
				break;
			}
			primaryClickStatus.clickedOnMenuItem = true;
		}
		else {
			primaryClickStatus.clickedOnMenuItem = false;
			if (mMenuMode == menuMode::reflocs) {
				bool presetPoint = true;
				if (!(mWhichPoint >= 0 && mWhichPoint <= 3)) {
					presetPoint = false;
				}
				setRefLocation(mWhichPoint, mStaticPointsNotMobile, x, y);
				flagRefLocationAsSet(mWhichPoint);
				if (presetPoint) {
					tmpMenu = mWhichPoint + MENU_SUB_LOC0;
					mMenuSelected[tmpMenu] = false;
					if (mWhichPoint < 3) {
						mMenuSelected[tmpMenu + 1] = true;
						primaryClickStatus.menuChoice = tmpMenu + 1;
						mWhichPoint++;
						primaryClickStatus.clickedOnMenuItem = true; // spoof the effect of having clicked on the next menu item
					}
					else {
						mWhichPoint = -1;
					}
				}
				else {
					mWhichPoint = -1;
				}

//				showImg();
			}
			else if (mMenuMode == menuMode::geometries) {
				if (primaryClickStatus.menuChoice == MENU_GEO_DELETE) {
					int whichGeoDesc = findClosestGeometry(unResizePoint(false, Point(x, y), mApplyDistortion));
					if (whichGeoDesc >= 0) {
						removeGeometry(whichGeoDesc);
					}
					mMenuSelected[MENU_GEO_DELETE] = false;
					mMenuSelection = MENU_NONE;
				}
				else if (primaryClickStatus.menuChoice == MENU_GEO_COUNT) {
					int whichGeoDesc = findClosestGeometry(unResizePoint(false, Point(x, y), mApplyDistortion));
					whichGeoCount = calcCount(whichGeoDesc);
				}
				else if (primaryClickStatus.menuChoice == MENU_GEO_SCALE) {
					switch (mWhichPoint) {
					case 0:
						mScaleCorner = unResizePoint(false, Point(x, y), mApplyDistortion);
						mWhichPoint = 1;
						break;
					case 1:
						mScaleHorizntal = unResizePoint(false, Point(x, y), mApplyDistortion);
						mWhichPoint = 2;
						break;
					case 2:
						mScaleDiagonal = unResizePoint(false, Point(x, y), mApplyDistortion);
						mWhichPoint = -1;
						mMenuSelected[MENU_GEO_SCALE] = false;
						mMenuSelection = MENU_NONE;
						break;
					}
				} else {
					setAreaLocation(getCurrentAreaLocation(), mWhichPoint, x, y);
				}
//				showImg();
			} else {
				if (primaryClickStatus.menuChoice == MENU_MENU_CORNER) {
					mMenuOrg = Point(x, y);
					mMenuSelected[MENU_MENU_CORNER] = false;
					mMenuSelection = MENU_NONE;
					primaryClickStatus.menuChoice = MENU_NONE;
				}
			}
		}
		primaryClickStatus.mousedown = true;
		primaryClickStatus.startX = x;
		primaryClickStatus.startY = y;
		showImg();
	}
	if (event == EVENT_MOUSEWHEEL) {
		float wheelIncr = 1.0f;
		int wheelData = getMouseWheelDelta(flags);
		if (wheelData < 0) {
			mContrast += wheelIncr;
			mContrast = (mContrast > 1) ? 1 : mContrast;
		}
		else {
			mContrast -= wheelIncr;
			mContrast = (mContrast< 0) ? 0 : mContrast;

		}
		showImg();
	}
	if (event == EVENT_RBUTTONDOWN)
	{
		int tmpMenu = menuSelected(x, y, mMenuMode);
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_GEO_TYPE:
				if (mMenuTypeIdx == MENU_BROODIDX_NOT_PUPA && mShowDelta) {
					mMenuTypeIdx = MENU_BROODIDX_PUPA;
				}
				else {
					mMenuTypeIdx--;

				}
				if (mMenuTypeIdx < 0) {
					mMenuTypeIdx = MENU_BROODIDX_QTY - 1;
				}
				mMenuStr[MENU_GEO_TYPE] = mMenuTypeStr[mMenuTypeIdx];
				getCurrentAreaLocation()->mType = mMenuTypeIdx;
				break;

			}
			primaryClickStatus.clickedOnMenuItem = true;
			showImg();
		}
		else {
			int whichGeoDesc = findClosestGeometry(unResizePoint(false, Point(x, y), mApplyDistortion));
			if (whichGeoDesc >= 0) {
				removeGeometry(whichGeoDesc);
				showImg();
			}

		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		primaryClickStatus.mousedown = false;

		if (mMenuMode == menuMode::geometries) {
			if (!primaryClickStatus.clickedOnMenuItem) {
				if (primaryClickStatus.menuChoice == MENU_GEO_SCALE || primaryClickStatus.menuChoice == MENU_GEO_COUNT) {

				}
				else {
					int tmpMenu = mWhichPoint + MENU_GEO_PT0;
					if (mWhichPoint >= 0 && mWhichPoint < (MENU_GEO_DONE - MENU_GEO_PT0)) {
						mMenuSelected[tmpMenu] = false;
						if (mWhichPoint < (MENU_GEO_DONE - MENU_GEO_QUAD) - 1) {
							mWhichPoint++;
							areaDesc::shape curShape = superimposeAreaDisplay::getGeometryTypeBeingEdited();
							switch (curShape) {
							case areaDesc::shape::quad:
								if (mWhichPoint > (MENU_GEO_PT3 - MENU_GEO_PT0)) {
									mAreaDescriptions.push_back(new areaDesc());
									mAreaDescriptions.back()->clone(mTmpAreaDesc);
									mTmpAreaDesc.emptyEntry();
									mTmpAreaDesc.mShape = areaDesc::shape::quad;
									mTmpAreaDesc.mType = mMenuTypeIdx;
									mTmpAreaDesc.mDrone = mMenuSelected[MENU_GEO_DRONE];
									mTmpAreaDesc.mDelta = mMenuSelected[MENU_GEO_DELTA];
									mWhichPoint = 0;
								}
								break;
							case areaDesc::shape::tri:
								if (mWhichPoint >= (MENU_GEO_PT3 - MENU_GEO_PT0)) {
									mAreaDescriptions.push_back(new areaDesc());
									mAreaDescriptions.back()->clone(mTmpAreaDesc);
									mTmpAreaDesc.emptyEntry();
									mTmpAreaDesc.mShape = areaDesc::shape::tri;
									mTmpAreaDesc.mType = mMenuTypeIdx;
									mTmpAreaDesc.mDrone = mMenuSelected[MENU_GEO_DRONE];
									mTmpAreaDesc.mDelta = mMenuSelected[MENU_GEO_DELTA];
									mWhichPoint = 0;
								}
								break;
							case areaDesc::shape::line:
								if (mWhichPoint >= (MENU_GEO_PT2 - MENU_GEO_PT0)) {
									mAreaDescriptions.push_back(new areaDesc());
									mAreaDescriptions.back()->clone(mTmpAreaDesc);
									mTmpAreaDesc.emptyEntry();
									mTmpAreaDesc.mShape = areaDesc::shape::line;
									mTmpAreaDesc.mType = mMenuTypeIdx;
									mTmpAreaDesc.mDrone = mMenuSelected[MENU_GEO_DRONE];
									mTmpAreaDesc.mDelta = mMenuSelected[MENU_GEO_DELTA];
									mWhichPoint = 0;
								}
								break;
							case areaDesc::shape::point:
								mAreaDescriptions.push_back(new areaDesc());
								mAreaDescriptions.back()->clone(mTmpAreaDesc);
								mTmpAreaDesc.emptyEntry();
								mTmpAreaDesc.mShape = areaDesc::shape::point;
								mTmpAreaDesc.mType = mMenuTypeIdx;
								mTmpAreaDesc.mDrone = mMenuSelected[MENU_GEO_DRONE];
								mTmpAreaDesc.mDelta = mMenuSelected[MENU_GEO_DELTA];
								mWhichPoint = 0;
								break;
							}
							if (mWhichPoint >= 0) {
								mMenuSelected[mWhichPoint + MENU_GEO_PT0] = true;
								primaryClickStatus.menuChoice = mWhichPoint + MENU_GEO_PT0;
								primaryClickStatus.clickedOnMenuItem = true; // spoof the effect of having clicked on the next menu item

							}
							else {
								primaryClickStatus.menuChoice = MENU_NONE;
								primaryClickStatus.clickedOnMenuItem = false;

							}
						}
						else
						{
							mWhichPoint = -1;
						}
					}
					else if (mWhichPoint >= 0 && mWhichPoint == (MENU_GEO_DONE - MENU_GEO_PT0) - 1) {
						mMenuSelected[tmpMenu] = false;
					}

				}
				showImg();
			}
		}
		else {
			if (mWhichPoint == -2) {
				mWhichPoint = 0;
			}
			if (!primaryClickStatus.clickedOnMenuItem) {
				clearMenuLocSelections();
				mWhichPoint = -1;
			}
			primaryClickStatus.clickedOnMenuItem = false;

		}

	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (primaryClickStatus.mousedown) {
			//if ((mMenuMode == menuMode::topMenu)) {
			//	showImg();
			//}
			//if ((mMenuMode == menuMode::reflocs)) {
			//	if ((mWhichPoint >= 0 && mWhichPoint <= 3)) {
			//		setRefLocation(mWhichPoint, true, x, y);
			//		showImg();
			//	}
			//}
			if (mMenuMode == menuMode::geometries) {
//				if (!primaryClickStatus.clickedOnMenuItem) {
					areaDesc::shape curShape = superimposeAreaDisplay::getGeometryTypeBeingEdited();
					if (curShape == areaDesc::shape::line) {
						if (mWhichPoint == (MENU_GEO_PT1 - MENU_GEO_PT0)) {
							float len = mTmpAreaDesc.calcLength(mTmpAreaDesc.pt0, mTmpAreaDesc.pt1);
							float movedLenFactor = mTmpAreaDesc.calcLength(mTmpAreaDesc.pt0, unResizePoint(false, Point2f(x, y), mApplyDistortion)) / len;
							mTmpAreaDesc.linearCellGapMultiplier =(( movedLenFactor - 1) * 2 ) + 1;
						}
						showImg();
					}
//				}
			}
		}
	}
}
void superimposeAreaDisplay::rightClick(int x, int y) {
}
void superimposeAreaDisplay::trackCallback(int val) {
	mTrackVal = val;
	showImg();
}
void superimposeAreaDisplay::rescaleSuperImage() {
	mCombImage.copyTo(mRescaledCombImage);
	Point center = Point(mRescaledCombImage.cols / 2, mRescaledCombImage.rows / 2);
	// You can try more different parameters
	//Size dsize = Size(mDispCols, mDispRows);
	Size dsize = Size(mRescaledCombImage.cols, mRescaledCombImage.rows);
	resizeFrame(&mRescaledCombImage, &mRescaledCombImage);
	if (mHavePrevious) {
		mCombImagePrevious.copyTo(mRescaledCombImagePrevious);
		resizeFrame(&mRescaledCombImagePrevious, &mRescaledCombImagePrevious);

	}
}

int superimposeAreaDisplay::menuSelected(int x, int y, menuMode whichMenu) {
	int relX = x - mMenuOrg.x;
	int relY = y - mMenuOrg.y;

	int retv = MENU_NONE;
	int maxy = (whichMenu == topMenu) ?  MENU_QTY : (whichMenu == reflocs)? MENU_SUB_QTY : MENU_GEO_QTY;
	int offsety = (whichMenu == topMenu) ? 0  : (whichMenu == reflocs)? MENU_QTY : MENU_QTY + MENU_SUB_QTY;
	maxy += 1; // title allowance
	maxy *= MENU_PANEL_LINE_HEIGHT;
	if (relX > 0 && relX <= MENU_PANEL_WIDTH && relY > MENU_PANEL_LINE_HEIGHT && relY <= maxy) {
		retv = relY / MENU_PANEL_LINE_HEIGHT;
		retv -= 1; // title allowance
		retv += offsety;
	}
	return retv;
}
void superimposeAreaDisplay::showImg() {
	adjustImage(true); // mMenuMode == menuMode::reflocs);
	imshow(mWindowName, mAdjustedImage);
	showMenu(mMenuMode);
}
void superimposeAreaDisplay::hide() {
	destroyWindow(mWindowName);
}

void superimposeAreaDisplay::resizeFrame(Mat* inputMat, Mat* outputMat) {
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

Point superimposeAreaDisplay::resizePoint(bool previousNotLatest, Point2f inputLoc, bool andPerspective) {
	Point2f retv = Point2f(((inputLoc.x)) / mDispDivision, ((inputLoc.y)) / mDispDivision);
	if (andPerspective) 
		retv = (previousNotLatest ) ? mPerspPrevious.adjustedPerspective(retv) : mPersp.adjustedPerspective(retv);
	return retv;
}
Point superimposeAreaDisplay::unResizePoint(bool previousNotLatest, Point2f inputLoc, bool andPerspective) {
	Point2f retv = inputLoc;
	if (andPerspective)
		retv = (previousNotLatest ) ? mPerspPrevious.adjustedPerspective(retv, true)  : mPersp.adjustedPerspective(retv, true);
	retv = Point2f(((retv.x * mDispDivision)), ((retv.y * mDispDivision)));
	return retv;
}


void superimposeAreaDisplay::adjustImage(bool bDrawCrosses) {
	mRescaledCombImage.copyTo(mAdjustedImage);
	mPersp.setToDoAdjustment(false);
	if (mHavePrevious) {
		mRescaledCombImagePrevious.copyTo(mAdjustedImagePrevious);
		mPerspPrevious.setToDoAdjustment(false);
	}
	if (bDrawCrosses) {
		drawCrosses(mAdjustedImage, &mPerspectiveMobile, false);
		//drawGeometries(mAdjustedImage, mZoom, mZoom, mZoomLocus);
	}
	if (mApplyDistortion && mHaveAllMobileLocs) {
		perspectiveImageReference rescaledMobile;
		perspectiveImageReference rescaledStatic;
		rescaledMobile.clone(mPerspectiveMobile);
		rescaledMobile.orgTL = resizePoint(false, rescaledMobile.orgTL, false);
		rescaledMobile.orgTR = resizePoint(false, rescaledMobile.orgTR, false);
		bool superFrame = false;
		int targetY = mDispRows - 1;
		int targetX0 = 0;
		int targetX1 = mDispCols - 1;
		mPersp.fullImageCols = NUMCELLSWIDTH_ACTUAL;
		mPersp.fullImageRows = NUMCELLSHEIGHTBROOD_ACTUAL;
		if (rescaledMobile.orgBL.y < (mRows * 3) / 4) {
			superFrame = true;
			targetX0 = ((mDispCols * (NUMCELLSWIDTHDISP - NUMCELLSWIDTH_ACTUAL)) / 2) / NUMCELLSWIDTHDISP;
			targetX1 = mDispCols - targetX0 - 1;
			targetY = ((mDispRows * NUMCELLSHEIGHTSUPER )/ NUMCELLSHEIGHTBROODDISP) - 1;
			mPersp.fullImageCols = NUMCELLSWIDTHDISP;
			mPersp.fullImageRows = NUMCELLSHEIGHTBROODDISP;
		}
		rescaledMobile.orgBL = resizePoint(false, rescaledMobile.orgBL, false);
		rescaledMobile.orgBR = resizePoint(false, rescaledMobile.orgBR, false);
		rescaledStatic.orgTL = Point(targetX0,0);
		rescaledStatic.orgTR = Point(targetX1, 0);
		rescaledStatic.orgBL = Point(targetX0, targetY);
		rescaledStatic.orgBR = Point(targetX1, targetY);
		mPersp.calcPerspectiveLambda(rescaledMobile, rescaledStatic);
		mPersp.warp(mAdjustedImage, mAdjustedImage, true);
		mPersp.setToDoAdjustment(true);
		if (mHavePrevious) {
			rescaledMobile.clone(mPerspectiveMobilePrevious);
			rescaledMobile.orgTL = resizePoint(false, rescaledMobile.orgTL, false);
			rescaledMobile.orgTR = resizePoint(false, rescaledMobile.orgTR, false);
			bool superFrame = false;
			int targetY = mDispRows - 1;
			mPerspPrevious.fullImageCols = NUMCELLSWIDTH_ACTUAL;
			mPerspPrevious.fullImageRows = NUMCELLSHEIGHTBROOD_ACTUAL;
			if (rescaledMobile.orgBL.y < (mRows * 3) / 4) {
				superFrame = true;
				targetX0 = ((mDispCols * (NUMCELLSWIDTHDISP - NUMCELLSWIDTH_ACTUAL)) / 2) / NUMCELLSWIDTHDISP;
				targetX1 = mDispCols - targetX0 - 1;
				targetY = ((mDispRows * NUMCELLSHEIGHTSUPER) / NUMCELLSHEIGHTBROODDISP) - 1;
				mPerspPrevious.fullImageCols = NUMCELLSWIDTHDISP;
				mPerspPrevious.fullImageRows = NUMCELLSHEIGHTBROODDISP;
			}
			rescaledMobile.orgBL = resizePoint(false, rescaledMobile.orgBL, false);
			rescaledMobile.orgBR = resizePoint(false, rescaledMobile.orgBR, false);
			rescaledStatic.orgTL = Point(targetX0, 0);
			rescaledStatic.orgTR = Point(targetX1, 0);
			rescaledStatic.orgBL = Point(targetX0, targetY);
			rescaledStatic.orgBR = Point(targetX1, targetY);
			mPerspPrevious.calcPerspectiveLambda(rescaledMobile, rescaledStatic);
			mPerspPrevious.warp(mAdjustedImagePrevious, mAdjustedImagePrevious, true);
			mPerspPrevious.setToDoAdjustment(true);
			mAdjustedImage = mAdjustedImage * mContrast + mAdjustedImagePrevious * (1 - mContrast);
		}
	}
	float contrast = mTrackVal;
	contrast /= 100;
	drawGeometries(mShowDelta, mPreviousCells, mAdjustedImage);
}
void superimposeAreaDisplay::drawCross(Mat image, Point whereOnImage, char *name, bool plusNotTimes) {
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
void superimposeAreaDisplay::drawCrosses(Mat image, perspectiveImageReference *relevantPerspective, bool plusNotTimes) {
	drawCross(image, resizePoint(false, relevantPerspective->orgTL, true), "ref 1", plusNotTimes);
	drawCross(image, resizePoint(false, relevantPerspective->orgTR, true), "ref 2", plusNotTimes);
	drawCross(image, resizePoint(false, relevantPerspective->orgBR, true), "ref 3", plusNotTimes);
	drawCross(image, resizePoint(false, relevantPerspective->orgBL, true), "ref 4", plusNotTimes);
	circle(image, resizePoint(false, mScaleCorner, true), 15, Scalar(255, 0, 0), 2);
	circle(image, resizePoint(false, mScaleHorizntal, true), 15, Scalar(255, 0, 0), 2);
	circle(image, resizePoint(false, mScaleDiagonal, true), 15, Scalar(255, 0, 0), 2);
	line(image, resizePoint(false, mScaleCorner, true), resizePoint(false, mScaleHorizntal, true), Scalar(255, 0, 0), 2);
	line(image, resizePoint(false, mScaleCorner, true), resizePoint(false, mScaleDiagonal, true), Scalar(255, 0, 0), 2);
}
//void superimposeAreaDisplay::intermediateDotsDelta(float pixelsPerCellHor, float pixelsPerCellHeight, std::vector<Point2f>& newPtArray, std::vector<Point2f>& missingPtArray, std::vector<Point2f>& coincidePtArray) {
//	std::vector<Point2f> intermediateDots;
//	std::vector<Point2f> intermediateDotsPrevious;
//	int qtyNow = intermediateDotsForGeometries(false, false, MENU_BROODIDX_PUPA, intermediateDots);
//	int qtyPrev = intermediateDotsForGeometries(false, true, MENU_BROODIDX_PUPA, intermediateDotsPrevious);
//	bool *flagArrayPrev = new bool[qtyPrev];
//	for (int prevIdx = 0; prevIdx < intermediateDotsPrevious.size(); prevIdx++) {
//		flagArrayPrev[prevIdx] = false;
//	}
//	for (int nowIdx = 0; nowIdx < intermediateDots.size(); nowIdx++) {
//		float closest = 10;
//		int closestIdx = -1;
//		for (int prevIdx = 0; prevIdx < intermediateDotsPrevious.size(); prevIdx++) {
//			if (!flagArrayPrev[prevIdx]) { // dont check ones weve already found to be coincident
//				float distH = Math::Abs(intermediateDots[nowIdx].x - intermediateDotsPrevious[prevIdx].x) / pixelsPerCellHor;
//				float distV = Math::Abs(intermediateDots[nowIdx].y - intermediateDotsPrevious[prevIdx].y) / pixelsPerCellHeight;
//				float distThis = distH * distH + distV * distV;
//				if (distThis < closest) {
//					closest = distThis;
//					closestIdx = prevIdx;
//				}
//			}
//		}
//		if (closestIdx >= 0 && closest < 0.6f) {
//			// old and new are coincident, so remove from searchable collection
//			coincidePtArray.push_back(intermediateDotsPrevious[closestIdx]);
//			flagArrayPrev[closestIdx] = true;
//		}
//		else {
//			newPtArray.push_back(intermediateDots[nowIdx]);
//		}
//	}
//	for (int prevIdx = 0; prevIdx < intermediateDotsPrevious.size(); prevIdx++) {
//		if (!flagArrayPrev[prevIdx]) { // not yet been flagged as coincident
//			missingPtArray.push_back(intermediateDotsPrevious[prevIdx]);
//		}
//	}
//	delete flagArrayPrev;
//}
void superimposeAreaDisplay::intermediateDotsDelta(float pixelsPerCellHor, float pixelsPerCellHeight, std::vector<Point2f>& newPtArray, std::vector<Point2f>& missingPtArray, std::vector<Point2f>& coincidePtArray) {
	std::vector<Point2f> intermediateDots;
	std::vector<Point2f> intermediateDotsPrevious;
	int qtyNow = intermediateDotsForGeometries(false, false, MENU_BROODIDX_PUPA, intermediateDots);
	int qtyPrev = intermediateDotsForGeometries(false, true, MENU_BROODIDX_PUPA, intermediateDotsPrevious);
	int *prevoiusClosestToNow = new int[qtyNow];
	float *prevoiusDistToNow = new float[qtyNow];
	int *nowClosestToPrevoius = new int[qtyPrev];
	float *nowDistToPrevoius = new float[qtyPrev];

	for (int prevIdx = 0; prevIdx < qtyPrev; prevIdx++) {
		nowClosestToPrevoius[prevIdx] = -1;
		nowDistToPrevoius[prevIdx] = -1;
	}
	for (int nowIdx = 0; nowIdx < qtyNow; nowIdx++) {
		prevoiusClosestToNow[nowIdx] = -1;
		prevoiusDistToNow[nowIdx] = -1;
	}
	for (int nowIdx = 0; nowIdx < intermediateDots.size(); nowIdx++) {
		for (int prevIdx = 0; prevIdx < intermediateDotsPrevious.size(); prevIdx++) {
				float distH = Math::Abs(intermediateDots[nowIdx].x - intermediateDotsPrevious[prevIdx].x) / pixelsPerCellHor;
				float distV = Math::Abs(intermediateDots[nowIdx].y - intermediateDotsPrevious[prevIdx].y) / pixelsPerCellHeight;
				float distThis = distH * distH + distV * distV;
				if (prevoiusDistToNow[nowIdx] < 0 || distThis < prevoiusDistToNow[nowIdx]) {
					prevoiusDistToNow[nowIdx] = distThis;
					prevoiusClosestToNow[nowIdx] = prevIdx;
				}
				if (nowDistToPrevoius[prevIdx] < 0 || distThis < nowDistToPrevoius[prevIdx]) {
					nowDistToPrevoius[prevIdx] = distThis;
					nowClosestToPrevoius[prevIdx] = nowIdx;
				}
		}
	}
	for (int prevIdx = 0; prevIdx < qtyPrev; prevIdx++) {
		if ((nowClosestToPrevoius[prevIdx] >= 0 && nowDistToPrevoius[prevIdx] < mDetectionRangeSq) && prevoiusClosestToNow[nowClosestToPrevoius[prevIdx]] == prevIdx){
			coincidePtArray.push_back(intermediateDotsPrevious[prevIdx]);
		}
		else {
			missingPtArray.push_back(intermediateDotsPrevious[prevIdx]);
		}
	}
	for (int nowIdx = 0; nowIdx < qtyNow; nowIdx++) {
		if ((prevoiusClosestToNow[nowIdx] < 0 || prevoiusDistToNow[nowIdx] > mDetectionRangeSq) || nowClosestToPrevoius[prevoiusClosestToNow[nowIdx]] != nowIdx){
//		if ((prevoiusClosestToNow[nowIdx] < 0 || prevoiusDistToNow[nowIdx] > mDetectionRangeSq) ){
			newPtArray.push_back(intermediateDots[nowIdx]);
		}
	}
	delete prevoiusClosestToNow;
	delete prevoiusDistToNow;
	delete nowClosestToPrevoius;
	delete nowDistToPrevoius;
}

void superimposeAreaDisplay::drawGeometries(bool doDelta, bool previousNotLatest, Mat image) {
	if (!doDelta) {
		vector<areaDesc *> *descPtr = &mAreaDescriptions;
		if (previousNotLatest) {
			descPtr = &mAreaDescriptionsPrevious;
		}
		for (int idx = 0; idx < descPtr->size(); idx++) {
			drawGeometry(doDelta, previousNotLatest, image, descPtr->at(idx), idx == mGeoBeingEdited);
		}
		drawGeometry(doDelta, previousNotLatest, image, &mTmpAreaDesc, false);
	}
	else {
		std::vector<Point2f> newDots;
		std::vector<Point2f> missingDots;
		std::vector<Point2f> coincidingDots;
		perspective tmpPersp = (previousNotLatest) ? mPerspPrevious : mPersp;

		float pixelsPerCellHor = image.rows / tmpPersp.fullImageCols; // NUMCELLSWIDTH;
		float pixelsPerCellHeight = image.cols / tmpPersp.fullImageRows; // NUMCELLSHEIGHTBROOD;

		intermediateDotsDelta(pixelsPerCellHor, pixelsPerCellHeight, newDots, missingDots, coincidingDots);
		Scalar areaClr;
		Scalar lineClr;
		areaClr = lineClr = Scalar(255, 0, 0); // Scalar(160, 255, 200); // Scalar(105, 175, 132)
		for (int idx = 0; idx < newDots.size(); idx++) {
			//circle(image, intermediateDots[idx], 2, Scalar(255, 255, 255), -1);
			//circle(image, newDots[idx], 8, Scalar(255, 0,0), -1);
			circle(image, newDots[idx], 4, areaClr, 2);
			circle(image, newDots[idx], 2, lineClr, -1);
		}
		areaClr = Scalar(0, 255, 0);// Scalar(255, 255, 255);
		lineClr = Scalar(0, 0, 0);
		for (int idx = 0; idx < missingDots.size(); idx++) {
			//circle(image, intermediateDots[idx], 2, Scalar(255, 255, 255), -1);
			//circle(image, missingDots[idx], 8, Scalar(0, 255, 0), -1);
			circle(image, missingDots[idx], 4, areaClr, 2);
			circle(image, missingDots[idx], 2, lineClr, -1);
		}
		areaClr = Scalar(0, 0, 255); // Scalar(160, 255, 200); // Scalar(105, 175, 132)
		lineClr = Scalar(0, 0, 0);
		for (int idx = 0; idx < coincidingDots.size(); idx++) {
			//circle(image, intermediateDots[idx], 2, Scalar(255, 255, 255), -1);
			//circle(image, coincidingDots[idx], 8, Scalar(0, 0, 255), -1);
			circle(image, coincidingDots[idx], 4, areaClr, 2);
			circle(image, coincidingDots[idx], 2, lineClr, -1);
		}
	}

}
void longLine(Mat image, Point pt0, Point pt1, Scalar clr, int sz) {
	int overshootX = pt1.x - pt0.x;
	int overshootY = pt1.y - pt0.y;
	line(image, Point(pt0.x - overshootX, pt0.y - overshootY), Point(pt1.x + overshootX, pt1.y + overshootY), clr, sz);
}

void superimposeAreaDisplay::drawGeometry(bool doDelta, bool previousNotLatest, Mat image, areaDesc *desc, bool highlight) {
	Scalar areaClr;
	Scalar lineClr;
	perspective tmpPersp = (previousNotLatest) ? mPerspPrevious : mPersp;
	switch (desc->mType) {
	case MENU_BROODIDX_EGG:
		areaClr = lineClr = Scalar(255, 255, 255);
		break;
	case MENU_BROODIDX_LARVA:
		areaClr = lineClr = Scalar(206, 206, 74);
		break;
	case MENU_BROODIDX_PUPA:
		areaClr = lineClr = Scalar(160, 255, 200); // Scalar(105, 175, 132)
		break;
	case MENU_BROODIDX_NOT_EGG:
		areaClr = Scalar(255, 255, 255);
		lineClr = Scalar(0, 0, 0);
		break;
	case MENU_BROODIDX_NOT_LARVA:
		areaClr = Scalar(206, 206, 74);
		lineClr = Scalar(0, 0, 0);
		break;
	case MENU_BROODIDX_NOT_PUPA:
		areaClr = Scalar(105, 175, 132);
		lineClr = Scalar(0, 0, 0);
		break;
	case MENU_BROODIDX_EGG_NOT_LARVA:
		areaClr = Scalar(206, 206, 74);
		lineClr = Scalar(255,255,255);
		break;
	case MENU_BROODIDX_LARVA_NOT_PUPA:
		areaClr = Scalar(105, 175, 132);
		lineClr = Scalar(206, 206, 74);
		break;
	case MENU_BROODIDX_PUPA_NOT_LARVA:
		lineClr = Scalar(105, 175, 132);
		areaClr = Scalar(206, 206, 74);
		break;

	}
	int dotScale = highlight ? 4 : 2;
	int lineScale = desc->mDrone ? dotScale * 1.5f : dotScale;
	bool showThis = (doDelta && desc->mDelta) || (!doDelta && !desc->mDelta);
	switch (desc->mShape) {
	case areaDesc::shape::point:
		if (showThis) {
			Point zPt0 = resizePoint(previousNotLatest, desc->pt0, true);
			circle(image, zPt0, dotScale * 4, areaClr, 2);
			circle(image, zPt0, dotScale * 2, lineClr, -1);

		}
		break;
	case areaDesc::shape::line: 
	{
		if (showThis) {
			vector<Point2f> intermediateDots;
			Point zPt0 = resizePoint(previousNotLatest, desc->pt0, true);
			Point zPt1 = resizePoint(previousNotLatest, desc->pt1, true);
			intermediateDots.clear();
			desc->calcArea(tmpPersp, mDispDivision, mDispCols, mDispRows, intermediateDots);
			circle(image, zPt0, dotScale * 4, areaClr, 2);
			circle(image, zPt0, dotScale * 2, lineClr, -1);
			circle(image, zPt1, dotScale * 4, areaClr, 1);
			line(image, zPt0, zPt1, lineClr, lineScale);
			for (int idx = 0; idx < intermediateDots.size(); idx++) {
				//circle(image, intermediateDots[idx], 2, Scalar(255, 255, 255), -1);
				circle(image, intermediateDots[idx], dotScale * 3, Scalar(255, 255, 255), 1);
			}
		}

	}
		break;
	case areaDesc::shape::tri: {
		if (showThis) {
			Point zPt0 = resizePoint(previousNotLatest, desc->pt0, true);
			Point zPt1 = resizePoint(previousNotLatest, desc->pt1, true);
			circle(image, zPt0, dotScale * 4, areaClr, 2);
			circle(image, zPt0, dotScale * 2, lineClr, -1);
			circle(image, resizePoint(previousNotLatest, desc->pt1, true), dotScale * 4, areaClr, 1);
			line(image, resizePoint(previousNotLatest, desc->pt0, true), resizePoint(previousNotLatest, desc->pt1, true), lineClr, lineScale);
			circle(image, resizePoint(previousNotLatest, desc->pt2, true), dotScale * 4, areaClr, 1);
			line(image, resizePoint(previousNotLatest, desc->pt0, true), resizePoint(previousNotLatest, desc->pt2, true), lineClr, lineScale);
			line(image, resizePoint(previousNotLatest, desc->pt1, true), resizePoint(previousNotLatest, desc->pt2, true), lineClr, lineScale);
		}

	}
		break;
	case areaDesc::shape::quad:
	{
		if (showThis) {
			Point zPt0 = resizePoint(previousNotLatest, desc->pt0, true);
			Point zPt1 = resizePoint(previousNotLatest, desc->pt1, true);

			circle(image, zPt0, dotScale * 4, areaClr, 2);
			circle(image, zPt0, dotScale * 2, lineClr, -1);
			circle(image, resizePoint(previousNotLatest, desc->pt1, true), dotScale * 4, areaClr, 1);
			line(image, resizePoint(previousNotLatest, desc->pt0, true), resizePoint(false, desc->pt1, true), lineClr, lineScale);
			// assume clockwise entry
			circle(image, resizePoint(previousNotLatest, desc->pt2, true), dotScale * 4, areaClr, 1);
			circle(image, resizePoint(previousNotLatest, desc->pt3, true), dotScale * 4, areaClr, 1);
			line(image, resizePoint(previousNotLatest, desc->pt0, true), resizePoint(previousNotLatest, desc->pt3, true), lineClr, lineScale);
			line(image, resizePoint(previousNotLatest, desc->pt2, true), resizePoint(previousNotLatest, desc->pt3, true), lineClr, lineScale);
			line(image, resizePoint(previousNotLatest, desc->pt1, true), resizePoint(previousNotLatest, desc->pt2, true), lineClr, lineScale);
		}
	}
		break;
	}
	//circle(image, resizePoint(desc->pt0, true), dotScale * 4, areaClr, 2);
	//if (desc->isEmpty(desc->pt1)) {
	//	circle(image, resizePoint(desc->pt0, true), dotScale * 2, lineClr, -1);
	//} else {
	//	circle(image, resizePoint(desc->pt1, true), dotScale * 4, areaClr, 1);
	//	line(image, resizePoint(desc->pt0, true), resizePoint(desc->pt1, true), lineClr, lineScale);
	//	if (!desc->isEmpty(desc->pt2) && desc->isEmpty(desc->pt3)) {
	//		circle(image, resizePoint(desc->pt2, true), dotScale * 4, areaClr, 1);
	//		line(image, resizePoint(desc->pt0, true), resizePoint(desc->pt2, true), lineClr, lineScale);
	//		line(image, resizePoint(desc->pt1, true), resizePoint(desc->pt2, true), lineClr, lineScale);
	//	}
	//	if (!desc->isEmpty(desc->pt2) && !desc->isEmpty(desc->pt3)) {
	//		// assume clockwise entry
	//		circle(image, resizePoint(desc->pt2, true), dotScale * 4, areaClr, 1);
	//		circle(image, resizePoint(desc->pt3, true), dotScale * 4, areaClr, 1);
	//		line(image, resizePoint(desc->pt0, true), resizePoint(desc->pt3, true), lineClr, lineScale);
	//		line(image, resizePoint(desc->pt2, true), resizePoint(desc->pt3, true), lineClr, lineScale);
	//		line(image, resizePoint(desc->pt1, true), resizePoint(desc->pt2, true), lineClr, lineScale);
	//	}
	//}
}

int superimposeAreaDisplay::intermediateDotsForGeometries(bool doDelta, bool previousNotLatest, int cellType, std::vector<Point2f>& outPtArray) {
	int retv = 0;
	outPtArray.clear();
	vector<areaDesc *> *descPtr = &mAreaDescriptions;
	if (previousNotLatest) {
		descPtr = &mAreaDescriptionsPrevious;
	}
	for (int idx = 0; idx < descPtr->size(); idx++) {
		if ((doDelta && descPtr->at(idx)->mDelta) || (!doDelta && !(descPtr->at(idx)->mDelta))) {
			retv += intermediateDotsForGeometry(previousNotLatest, cellType, descPtr->at(idx), outPtArray);
		}
	}
	//if (!previousNotLatest && ((doDelta && mTmpAreaDesc.mDelta) || (!doDelta && !(mTmpAreaDesc.mDelta)))) {
	//	retv += intermediateDotsForGeometry(previousNotLatest, cellType, &mTmpAreaDesc, outPtArray);
	//}
	return retv;
}
int superimposeAreaDisplay::intermediateDotsForGeometry(bool previousNotLatest, int cellType, areaDesc* desc, std::vector<Point2f>& outPtArray) {
	int retv = 0;
	if (desc->mType == cellType) {
		switch (desc->mShape) {
		case areaDesc::shape::point:
		{
			Point zPt0 = resizePoint(previousNotLatest, desc->pt0, true);
			outPtArray.push_back(zPt0);
			retv = 1;
			break;

		}
		case areaDesc::shape::line:
		{
			Point zPt0 = resizePoint(previousNotLatest, desc->pt0, true);
			Point zPt1 = resizePoint(previousNotLatest, desc->pt1, true);
			perspective tmpPersp = (previousNotLatest) ? mPerspPrevious : mPersp;
			retv += desc->calcArea(tmpPersp, mDispDivision, mDispCols, mDispRows, outPtArray);
			break;

		}
		}
	}
	return retv;
}

