
#include <iostream>
#include <fstream>
#include <sstream>
#include "cvUtilities.h"
#include "csvFile.h"
#include "superimposeDisplay.h"
#include "adjustableDisplay.h"
#include "time.h"

superimposeDisplay::superimposeDisplay(char *windowName, Mat baseImage, Mat superImage, int dispRows, int dispCols, float dispDivision)
	: mWindowName(nullptr)
{
	time_t t;
	srand((unsigned)time(&t));

	for (int idx = 0; idx < geometryDesc::shape::SHAPE_QTY; idx++) {
		showGeoTypes[idx] = true;
		showGeoTypesR[idx] = true;
	}

	mAdjustableDisplay = nullptr;

	mCurrentOffsetX = mCurrentOffsetY = mOffsetX = mOffsetY = 0;
	mWindowName = new char[strlen(windowName) + 1];
	mConfigFilename1 = mConfigFilename2 = mConfigFilenameJoint = "";
	strcpy(mWindowName, windowName);
	baseImage.copyTo(mBaseImage);
	superImage.copyTo(mSuperImage);
	mCols = mBaseImage.cols;
	mRows = mBaseImage.rows;
	mDispDivision = dispDivision;
	if (dispDivision <= 0) {
		mDispDivision = 1;
	}
	mDispCols = mCols / mDispDivision;
	mDispRows = mRows / mDispDivision;
	mSuperImage.copyTo(mRescaledImage);
	mConfig.zoomCentre =  mZoomLocus = Point(2110, 1390); // Point(mDispCols / 2, mDispRows / 2);
	resizeFrame(&mRescaledImage, &mRescaledImage, 1,1, mZoomLocus);

	mDisplayedImageIdx = 0;
	mSliderName = "Contrast";
	mSliderScaleX = "ScaleX";
	mSliderScaleY = "ScaleY";
	mSliderRotate = "Rotate";
	mSliderZoom = "Zoom";
	mScaleX = mScaleY = 1;
	mMenuMode = topMenu;
	mRtClickedMenu = false;
	mMenuHeightIdx = MENU_HEIGHTIDX_2;

	mMenuHeightStr[MENU_HEIGHTIDX_0] = "0mm";
	mMenuHeightStr[MENU_HEIGHTIDX_1] = "1mm";
	mMenuHeightStr[MENU_HEIGHTIDX_2] = "2mm";
	mMenuHeightStr[MENU_HEIGHTIDX_3] = "3mm";
	mMenuHeightStr[MENU_HEIGHTIDX_4] = "4mm";

	mMenuDepositStr[MENU_DEPOSITIDX_0] = "0%";
	mMenuDepositStr[MENU_DEPOSITIDX_1] = "10%";
	mMenuDepositStr[MENU_DEPOSITIDX_2] = "20%";
	mMenuDepositStr[MENU_DEPOSITIDX_3] = "30%";
	mMenuDepositStr[MENU_DEPOSITIDX_4] = "40%";
	mMenuDepositStr[MENU_DEPOSITIDX_5] = "50%";
	mMenuDepositStr[MENU_DEPOSITIDX_6] = "60%";
	mMenuDepositStr[MENU_DEPOSITIDX_7] = "70%";
	mMenuDepositStr[MENU_DEPOSITIDX_8] = "80%";
	mMenuDepositStr[MENU_DEPOSITIDX_9] = "90%";
	mMenuDepositStr[MENU_DEPOSITIDX_10] = "100%";

	mMenuDepositIdx = MENU_DEPOSITIDX_5;

	mMenuTitle[0] = "| img0:img1 |";
	mMenuTitle[1] = "| Ref Locs. |";
	mMenuTitle[2] = "| Geometries |";
	mMenuTitle[3] = "| Cells |";

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
	mMenuStr[MENU_LOCS_STATIC] = "Satic Locs";
	mMenuSelected[MENU_LOCS_STATIC] = false;
	mMenuStr[MENU_LOCS_MOBILE] = "Mobile Locs";
	mMenuSelected[MENU_LOCS_MOBILE] = false;
	mMenuStr[MENU_CLEAR_OFFSET] = "Clear offset";
	mMenuSelected[MENU_CLEAR_OFFSET] = false;
	mMenuStr[MENU_ZOOM_CENTRE] = "Zoom Centre";
	mMenuSelected[MENU_ZOOM_CENTRE] = false;
	mMenuStr[MENU_ADJ_CELL] = "AdjCell";
	mMenuSelected[MENU_ADJ_CELL] = false;
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
	mMenuStr[MENU_SUB_DISTORT] = "Reshape";
	mMenuSelected[MENU_SUB_DISTORT] = false;
	mMenuStr[MENU_SUB_STATIC] = "Satic Locs";
	mMenuSelected[MENU_SUB_STATIC] = false;
	mMenuStr[MENU_SUB_MOBILE] = "Mobile Locs";
	mMenuSelected[MENU_SUB_MOBILE] = false;
	mMenuStr[MENU_SUB_SCALE0] = "Scale 0";
	mMenuSelected[MENU_SUB_SCALE0] = false;
	mMenuStr[MENU_SUB_SCALE1] = "Scale 1";
	mMenuSelected[MENU_SUB_SCALE1] = false;
	mMenuStr[MENU_SUB_SCALECELL] = "Scale Cell";
	mMenuSelected[MENU_SUB_SCALECELL] = false;
	mMenuStr[MENU_SUB_SCALECELLWALL] = "Scale Wall";
	mMenuSelected[MENU_SUB_SCALECELLWALL] = false;
	mMenuStr[MENU_SUB_CELLBOUNDARY] = "CellBoundary";
	mMenuSelected[MENU_SUB_CELLBOUNDARY] = false;
	mMenuStr[MENU_SUB_DONE] = "Done";
	mMenuSelected[MENU_SUB_DONE] = false;

//#define MENU_GEO_END (0 + MENU_QTY + MENU_SUB_QTY)
//#define MENU_GEO_SIDE (1 + MENU_QTY + MENU_SUB_QTY)
//#define MENU_GEO_BENT (2 + MENU_QTY + MENU_SUB_QTY)
//#define MENU_GEO_DONE (4 + MENU_QTY + MENU_SUB_QTY)
	mMenuStr[MENU_GEO_END] = "End";
	mMenuSelected[MENU_GEO_END] = false;
	mMenuStr[MENU_GEO_SIDE] = "Side";
	mMenuSelected[MENU_GEO_SIDE] = false;
	mMenuStr[MENU_GEO_BENT] = "Bent";
	mMenuSelected[MENU_GEO_BENT] = false;
	mMenuStr[MENU_GEO_BULGE] = "Bulge";
	mMenuSelected[MENU_GEO_BULGE] = false;
	mMenuStr[MENU_GEO_BULGE2] = "Bulge2";
	mMenuSelected[MENU_GEO_BULGE2] = false;
	mMenuStr[MENU_GEO_RIM1PIT] = "rim1pit";
	mMenuSelected[MENU_GEO_RIM1PIT] = false;
	mMenuStr[MENU_GEO_RIM2PITS] = "rim2pits";
	mMenuSelected[MENU_GEO_RIM2PITS] = false;
	mMenuStr[MENU_GEO_ZIGZAG] = "ZigZag";
	mMenuSelected[MENU_GEO_ZIGZAG] = false;
	mMenuStr[MENU_GEO_CURVE] = "Curve";
	mMenuSelected[MENU_GEO_CURVE] = false;
	mMenuStr[MENU_GEO_HEIGHT] = mMenuHeightStr[mMenuHeightIdx];
	mMenuSelected[MENU_GEO_HEIGHT] = false;
	mMenuStr[MENU_GEO_DEPOSIT] = mMenuDepositStr[mMenuDepositIdx];
	mMenuSelected[MENU_GEO_DEPOSIT] = false;
	mMenuStr[MENU_GEO_WALLEND0] = "WallEnd0(X)";
	mMenuSelected[MENU_GEO_WALLEND0] = false;
	mMenuStr[MENU_GEO_WALLEND1] = "WallEnd1";
	mMenuSelected[MENU_GEO_WALLEND1] = false;
	mMenuStr[MENU_GEO_WALLCENTRE] = "WallC(Blg)";
	mMenuSelected[MENU_GEO_WALLCENTRE] = false;
	mMenuStr[MENU_GEO_PIT0] = "Pit 0";
	mMenuSelected[MENU_GEO_PIT0] = false;
	mMenuStr[MENU_GEO_PIT1] = "Pit 1";
	mMenuSelected[MENU_GEO_PIT1] = false;
	mMenuStr[MENU_GEO_PIT2] = "Pit 2";
	mMenuSelected[MENU_GEO_PIT2] = false;
	mMenuStr[MENU_GEO_PITCELL1] = "PitCell 1";
	mMenuSelected[MENU_GEO_PITCELL1] = false;
	mMenuStr[MENU_GEO_PITCELL2] = "PitCell 2";
	mMenuSelected[MENU_GEO_PITCELL2] = false;
	mMenuStr[MENU_GEO_BUILTWALL00] = "Built 00";
	mMenuSelected[MENU_GEO_BUILTWALL00] = false;
	mMenuStr[MENU_GEO_BUILTWALL01] = "Built 01";
	mMenuSelected[MENU_GEO_BUILTWALL01] = false;
	mMenuStr[MENU_GEO_BUILTWALL10] = "Built 10";
	mMenuSelected[MENU_GEO_BUILTWALL10] = false;
	mMenuStr[MENU_GEO_BUILTWALL11] = "Built 11";
	mMenuSelected[MENU_GEO_BUILTWALL11] = false;

	mMenuStr[MENU_GEO_CELLS] = "Cells";
	mMenuSelected[MENU_GEO_CELLS] = false;
	mMenuStr[MENU_GEO_CELLCENTRE] = "Cell";
	mMenuSelected[MENU_GEO_CELLCENTRE] = false;
	mMenuStr[MENU_GEO_CELLCONN] = "Connect";
	mMenuSelected[MENU_GEO_CELLCONN] = false;
	mMenuStr[MENU_GEO_SHOWCELL] = "ShowC";
	mMenuSelected[MENU_GEO_SHOWCELL] = false;

	mMenuStr[MENU_GEO_EDIT] = "Edit";
	mMenuSelected[MENU_GEO_EDIT] = false;
	mMenuStr[MENU_GEO_DELETE] = "Delete";
	mMenuSelected[MENU_GEO_DELETE] = false;

	mMenuStr[MENU_GEO_DONE] = "Done";
	mMenuSelected[MENU_GEO_DONE] = false;

	mMenuStr[MENU_ADJ_REDO] = "Redo";
	mMenuSelected[MENU_ADJ_REDO] = false;
	mMenuStr[MENU_ADJ_WALL] = "Wall";
	mMenuSelected[MENU_ADJ_WALL] = false;
	mMenuStr[MENU_ADJ_SELCELL] = "Cell";
	mMenuSelected[MENU_ADJ_SELCELL] = false;
	mMenuStr[MENU_ADJ_SHOWCELLS] = "ShowAll";
	mMenuSelected[MENU_ADJ_SHOWCELLS] = false;
	mMenuStr[MENU_ADJ_DONE] = "Done";
	mMenuSelected[MENU_ADJ_DONE] = false;

	scaleDistanceMM = SCALE_BETWEEN_SIDEBARS;
	mScalePt0 = Point(0, 0);
	mScalePt1 = Point(0, 0);
	mDispContrastTrackVal = 75;
	mTrackValZoom =  mTrackValRotate = mTrackValScaleX = mTrackValScaleY = 50;
	mZoom = mScaleX = mScaleY = 1;
	mRotate = 0;
	mGeoBeingEdited = -1;
	mCellBoundaryType = "NN";
	mGeometryDescriptions.push_back(new geometryDesc());
	mCellsNeedUpdate = true;

//	namedWindow(mWindowName, WINDOW_NORMAL);
//	resizeWindow(mWindowName, mDispCols, mDispRows);
	showImg();
	setMouseCallback(mWindowName, superimposeDisplay::mouseCallback, this);
	namedWindow("mCWindowName", WINDOW_AUTOSIZE);
	int desiredWidth = 640, desiredheight = 250;
	resizeWindow("mCWindowName", desiredWidth, desiredheight);

	createTrackbar(mSliderName, "mCWindowName", &mDispContrastTrackVal, 100, superimposeDisplay::trackCallback, this);
	createTrackbar(mSliderScaleX, "mCWindowName", &mTrackValScaleX, 100, superimposeDisplay::trackCallbackScaleX, this);
	createTrackbar(mSliderScaleY, "mCWindowName", &mTrackValScaleY, 100, superimposeDisplay::trackCallbackScaleY, this);
	createTrackbar(mSliderRotate, "mCWindowName", &mTrackValRotate, 100, superimposeDisplay::trackCallbackRotate, this);
	createTrackbar(mSliderZoom, "mCWindowName", &mTrackValZoom, 100, superimposeDisplay::trackCallbackZoom, this);

}
superimposeDisplay::~superimposeDisplay()
{
	if (mWindowName != nullptr) delete mWindowName;
	destroyWindow(mWindowName);
}
void superimposeDisplay::setConfigFile(const char* pConfigFilename1, const char* pConfigFilename2, const char* pConfigFilenameJoint, const char *experimentID) {
	mConfigFilename1 = string(pConfigFilename1);
	mConfigFilename2 = string(pConfigFilename2);
	mConfigFilenameJoint = string(pConfigFilenameJoint);
	mExperimentID = string(experimentID);
	loadConfigFile();
	rescaleSuperImage();
	mCellsNeedUpdate = true;
	showImg();
}

void superimposeDisplay::setAdjustableDisplay(adjustableDisplay *pAdjustableDisplay) {
	mAdjustableDisplay = pAdjustableDisplay;
}

void superimposeDisplay::loadConfigFile() {
	string incomingCustomSettings = "";
	mConfig.read(mConfigFilename1.c_str(),mConfigFilename2.c_str(),mConfigFilenameJoint.c_str(), &mGeometryDescriptions, incomingCustomSettings);
	mConfigCellFilename1 = mConfigFilename1 + ".cell.txt";
	parseCellData(mConfigCellFilename1, incomingCustomSettings, 1);
	mConfigCellFilename2 = mConfigFilename2 + ".cell.txt";
	parseCellData(mConfigCellFilename2, incomingCustomSettings, 0);
	mHive = mConfig.hive;
	mOffsetX = mCurrentOffsetX = mConfig.offsetX;
	mOffsetY = mCurrentOffsetY = mConfig.offsetY;
	mScaleX = mConfig.scaleX;
	mScaleY = mConfig.scaleY;
	mRotate = mConfig.rotation;
	mZoom = mConfig.zoom;
	mZoomLocus = mConfig.zoomCentre;
	mScalePt0 = mConfig.scalePt0;
	mScalePt1 = mConfig.scalePt1;

	scaleDistanceMM = mConfig.scaleDistanceMM;
	mPerspectiveMobile.clone(mConfig.mobileRefs);
	mPerspectiveStatic.clone(mConfig.staticRefs);
	float tmpScale = mScaleX;
	if (mScaleX < 1)
		tmpScale = (mScaleX - 0.5) * 2;
	mTrackValScaleX = tmpScale * 50;
	tmpScale = mScaleY;
	if (mScaleY < 1)
		tmpScale = (mScaleY - 0.5) * 2;
	mTrackValScaleY = tmpScale * 50;
	mTrackValRotate = ((mRotate / 180) * 50) + 50;
	mImageLocAtTopLeftOfdisplay = Point(0, 0);

	//if (mTrackValZoom < 50) {
	//	mZoom = mTrackValZoom;
	//	mZoom *= 0.8f / 50;
	//	mZoom += 0.2f; // 0.2-1
	//}
	//else {
	//	mZoom = mTrackValZoom - 50;
	//	mZoom /= 50;
	//	mZoom *= 4;
	//	mZoom += 1; // 1-5
	//}
	if (mZoom > 1) {
		tmpScale = mZoom;
		tmpScale -= 1;
		tmpScale *= (50 / 4);
		mTrackValZoom = tmpScale + 50;
	}
	else {
		tmpScale = mZoom;
		tmpScale -= 0.2f;
		mTrackValZoom = (tmpScale * 50) / 0.8f;
	}
	cvSetTrackbarPos(mSliderName, "mCWindowName", mTrackValRotate);
	cvSetTrackbarPos(mSliderScaleX, "mCWindowName", mTrackValScaleX);
	cvSetTrackbarPos(mSliderScaleY, "mCWindowName", mTrackValScaleY);
	cvSetTrackbarPos(mSliderZoom, "mCWindowName", mTrackValZoom);
}
void superimposeDisplay::saveConfigFile() {
	string serialisedCellData0 = serialiseCellData(0);
	string serialisedCellData1 = serialiseCellData(1);
	mConfig.hive = mHive;
	mConfig.offsetX = mOffsetX;
	mConfig.offsetY = mOffsetY;
	mConfig.scaleX = mScaleX;
	mConfig.scaleY = mScaleY;
	mConfig.rotation = mRotate;
	mConfig.zoom = mZoom;
	mConfig.zoomCentre = mZoomLocus;
	mConfig.scalePt0 = mScalePt0;
	mConfig.scalePt1 = mScalePt1;
	mConfig.scaleDistanceMM = scaleDistanceMM;
	mConfig.staticRefs.clone(mPerspectiveStatic);
	mConfig.mobileRefs.clone(mPerspectiveMobile);
	mConfig.write(mConfigFilename1.c_str(), mConfigFilename2.c_str(), mConfigFilenameJoint.c_str(), mExperimentID.c_str(), &mGeometryDescriptions);
	ofstream locFile(mConfigCellFilename1.c_str(), ios::out);
	locFile << serialisedCellData1 << "\n";
	locFile.close();
	ofstream locFile1(mConfigCellFilename2.c_str(), ios::out);
	locFile1 << serialisedCellData0 << "\n";
	locFile1.close();
}
std::string superimposeDisplay::serialiseSelectedCellsHeader() {
	std::stringstream ss;
	Point2f cellBoundaryCorner00;
	Point2f cellBoundaryCorner01;
	Point2f cellBoundaryCorner10;
	Point2f cellBoundaryCorner11;
	float pixelsScale = pixelsPerMM();
	calcSelectedBoundary(cellBoundaryCorner00, cellBoundaryCorner01, cellBoundaryCorner10, cellBoundaryCorner11);
	ss << mCellBoundaryType << "," << mCellBoundary0.x << "," << mCellBoundary0.y << "," << mCellBoundary1.x << "," << mCellBoundary1.y;
	ss  << ", " << cellBoundaryCorner00.x << "," << cellBoundaryCorner00.y ;
	ss  << ", " << cellBoundaryCorner01.x << "," << cellBoundaryCorner01.y ;
	ss  << ", " << cellBoundaryCorner10.x << "," << cellBoundaryCorner10.y ;
	ss  << ", " << cellBoundaryCorner11.x << "," << cellBoundaryCorner11.y ;
	ss << ", " << pixelsScale;
	ss << ",\nimageIdx, offcentreP, offcentreMM, " << cvCell::serialisedHeadings();
	return ss.str();
}
std::string superimposeDisplay::serialiseSelectedCells(int imageIdx) {
	std::stringstream ss;
	float pixelsScale = pixelsPerMM();
	float boundaryAngle = angleFromVector(mCellBoundary1 - mCellBoundary0);
	for (int idxC = 0; idxC < selectedCells[imageIdx].size(); idxC++) {
		float offCentre = minDistPointToLine(allCells[imageIdx][selectedCells[imageIdx][idxC]].mCentre, mCellBoundary0, mCellBoundary1);
		float angleToLine = boundaryAngle - angleFromVector(allCells[imageIdx][selectedCells[imageIdx][idxC]].mCentre - mCellBoundary0);
		offCentre = (angleToLine > 0) ? offCentre : -offCentre;
		ss << "\n"  << imageIdx << ","  << offCentre<< ","  << offCentre/pixelsScale << ","  << allCells[imageIdx][selectedCells[imageIdx][idxC]].serialise(pixelsScale) ;
	}
	return ss.str();
}
std::string superimposeDisplay::serialiseCellData(int imageIdx) {
	std::stringstream ss;
	ss << "autoCellContours,\n" << autoCellContours[imageIdx].size();
	for (int idx = 0; idx < autoCellContours[imageIdx].size(); idx++) {
		ss << ",\n" << (autoCellContours[imageIdx][idx]).size();
		for (int idxC = 0; idxC < autoCellContours[imageIdx][idx].size(); idxC++) {
			ss  << "," << (autoCellContours[imageIdx][idx])[idxC].x << "," << (autoCellContours[imageIdx][idx])[idxC].y;

		}
	}
	ss << "\n,autoCellCentres,\n" << autoCellCentres[imageIdx].size();
	for (int idx = 0; idx < autoCellCentres[imageIdx].size(); idx++) {
		ss  << "," << (autoCellCentres[imageIdx][idx]).x << "," << (autoCellCentres[imageIdx][idx]).y;
	}
	ss << "\n,autoCellConnections,\n" << autoCellConnections[imageIdx].size();
	for (int idx = 0; idx < autoCellConnections[imageIdx].size(); idx++) {
		ss << ",\n" << autoCellConnections[imageIdx][idx].size();
		for (int idxC = 0; idxC < autoCellConnections[imageIdx][idx].size(); idxC++) {
			ss << "," << (autoCellConnections[imageIdx][idx])[idxC] ;
		}
	}
	ss << "\n,addCellCentres,\n" << addCellCentres[imageIdx].size();
	for (int idx = 0; idx < addCellCentres[imageIdx].size(); idx++) {
		ss  << "," << (addCellCentres[imageIdx][idx]).x << "," << (addCellCentres[imageIdx][idx]).y;
	}
	ss << "\n,addCellConnections,\n" << addCellConnections[imageIdx].size();
	for (int idx = 0; idx < addCellConnections[imageIdx].size(); idx++) {
		ss  << "," << (addCellConnections[imageIdx][idx]).x << "," << (addCellConnections[imageIdx][idx]).y;
	}
	ss << "\n,minusCellCentres,\n" << minusCellCentres[imageIdx].size();
	for (int idx = 0; idx < minusCellCentres[imageIdx].size(); idx++) {
		ss  << "," << (minusCellCentres[imageIdx][idx]);
	}
	ss << "\n,minusCellConnections,\n" << minusCellConnections[imageIdx].size();
	for (int idx = 0; idx < minusCellConnections[imageIdx].size(); idx++) {
		ss  << "," << (minusCellConnections[imageIdx][idx]).x << "," << (minusCellConnections[imageIdx][idx]).y;
	}
	ss << persistantCellData[imageIdx].serialise();
	return ss.str();
}

void superimposeDisplay::parseCellData(string cellDataFilename, string configStr, int imageIdx) {
	std::ifstream str;
	if (cellDataFilename.length() > 2) {
		str.open(cellDataFilename);
		std::string allCustomLines;
		std::string oneLine;
		while (std::getline(str, oneLine))
		{
			if (oneLine == CUSTOM_FLAG) {
				break;
			}
			allCustomLines += oneLine;
		}
		configStr = allCustomLines;
		str.close();
	}
	parseCellData(configStr, imageIdx);
}

void superimposeDisplay::parseCellData(string configStr, int imageIdx) {
	std::vector<std::string>   tokenised;
	std::stringstream          lineStream(configStr);
	std::string                cell;

	while (std::getline(lineStream, cell, ','))
	{
		tokenised.push_back(cell);
	}
	int tokenQty = tokenised.size();
	int tokenIdx = 0;
	int typeQty;

	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			int subTypeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			vector<Point> *oneContour = new vector<Point>();
			while (subTypeQty-- > 0) {
				int x = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
				int y = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
				oneContour->push_back(Point(x, y));
			}
			autoCellContours[imageIdx].push_back(*oneContour);
		}

	}
	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			float x = csvFile::parseLine1fSafely(tokenised[tokenIdx++], 0);
			float y = csvFile::parseLine1fSafely(tokenised[tokenIdx++], 0);
			autoCellCentres[imageIdx].push_back(Point2f(x, y));
		}

	}
	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			int subTypeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			vector<int> *oneConnection = new vector<int>();
			while (subTypeQty-- > 0) {
				int x = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
				oneConnection->push_back(x);
			}
			autoCellConnections[imageIdx].push_back(*oneConnection);
		}

	}
	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			float x = csvFile::parseLine1fSafely(tokenised[tokenIdx++], 0);
			float y = csvFile::parseLine1fSafely(tokenised[tokenIdx++], 0);
			addCellCentres[imageIdx].push_back(Point2f(x, y));
		}

	}
	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			int x = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			int y = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			addCellConnections[imageIdx].push_back(Point(x, y));
		}

	}
	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			int x = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			minusCellCentres[imageIdx].push_back(x);
		}

	}
	tokenIdx++;
	if (tokenIdx < tokenQty) {
		typeQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
		while (typeQty-- > 0) {
			int x = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			int y = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			minusCellConnections[imageIdx].push_back(Point(x, y));
		}

	}
	tokenIdx = persistantCellData[imageIdx].deserialise(tokenised, tokenIdx);
}

float superimposeDisplay::getScaleDistanceMM() {
	return scaleDistanceMM;
}
void superimposeDisplay::setScaleDistanceMM(float dist) {
	scaleDistanceMM = dist;
}

float superimposeDisplay::pixelsPerMM() {
	float retv = DEFAULT_PIXELS_PER_MM;
	float scalePixels = magnitude(mScalePt0 - mScalePt1);
	if (scalePixels > 0) {
		retv = scalePixels / scaleDistanceMM;
	}
	return retv;
}
string superimposeDisplay::getHive() {
	return mHive;
}
void superimposeDisplay::setHive(string hive) {
	mHive = hive;
}

string superimposeDisplay::getCellData(std::string boundaryType) {
	string retv;
	
	std::vector<std::string>   tokenised;
	std::stringstream          lineStream(boundaryType);
	while (std::getline(lineStream, retv, ','))
	{
		tokenised.push_back(retv);
	}
	mCellBoundaryType = tokenised[0];
	int tokenQty = tokenised.size();
	if (tokenQty == 5) {
		mCellBoundary0 = Point(csvFile::parseLine1iSafely(tokenised[1], 0), csvFile::parseLine1iSafely(tokenised[2], 0));
		mCellBoundary1 = Point(csvFile::parseLine1iSafely(tokenised[3], 0), csvFile::parseLine1iSafely(tokenised[4], 0));
	}
	showImg();
	retv = serialiseSelectedCellsHeader() + serialiseSelectedCells(0) + serialiseSelectedCells(1) + "\n";
	return retv;
}
void superimposeDisplay::showMenuLine(int lineNo, char *txt) {
	int y = lineNo * MENU_PANEL_LINE_HEIGHT;
	rectangle(mDisplayImage, cv::Point(0, y), cv::Point(0 + MENU_PANEL_WIDTH, y + MENU_PANEL_LINE_HEIGHT),
		cv::Scalar(255, 255, 255), -1);
	putText(mDisplayImage, txt, cv::Point(5, y + 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

}

void superimposeDisplay::menuQtyString(char *strBuf, int menuIdx, menuMode whichMenu) {
	switch (menuIdx) {
	default:
		*strBuf = '\0';
		break;
	//case MENU_NULL:
	//	break;
	}
}
void superimposeDisplay::showMenu(menuMode whichMenu) {
	mAdjustedImage.copyTo(mDisplayImage);
	char menuLine[64];
	char menuVal0[32];
	int y = 0;
	switch (whichMenu) {
	case topMenu:
		showMenuLine(0, mMenuTitle[0]);
		while (y < MENU_QTY) {
			menuQtyString(&menuVal0[0], y, whichMenu);
			sprintf(menuLine, "%c%s %s", mMenuSelected[y] ? '*' : ' ', mMenuStr[y], &menuVal0[0]);
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
			int menuOffset = y + MENU_GEO_START;
			menuQtyString(&menuVal0[0], menuOffset, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[menuOffset], &menuVal0[0], mMenuSelected[menuOffset] ? '*' : ' ');
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	case cells:
		showMenuLine(0, mMenuTitle[3]);
		while (y < MENU_ADJ_QTY) {
			int menuOffset = y + MENU_ADJ_START;
			menuQtyString(&menuVal0[0], menuOffset, whichMenu);
			sprintf(menuLine, "%s %s %c", mMenuStr[menuOffset], &menuVal0[0], mMenuSelected[menuOffset] ? '*' : ' ');
			showMenuLine(y + 1, menuLine);
			y++;
		}
		break;
	}
	imshow(mWindowName, mDisplayImage);
}


void superimposeDisplay::mouseCallback(int event, int x, int y, int flags, void* userdata) {
	superimposeDisplay* psuperimposeDisplay = (superimposeDisplay*)userdata;
	psuperimposeDisplay->mouseCallback(event, x, y, flags);
}
void superimposeDisplay::trackCallback(int val, void* userdata) {
	superimposeDisplay* psuperimposeDisplay = (superimposeDisplay*)userdata;
	psuperimposeDisplay->trackCallback(val);
}
void superimposeDisplay::trackCallbackScaleX(int val, void* userdata) {
	superimposeDisplay* psuperimposeDisplay = (superimposeDisplay*)userdata;
	psuperimposeDisplay->trackCallbackScaleX(val);
}
void superimposeDisplay::trackCallbackScaleY(int val, void* userdata) {
	superimposeDisplay* psuperimposeDisplay = (superimposeDisplay*)userdata;
	psuperimposeDisplay->trackCallbackScaleY(val);
}
void superimposeDisplay::trackCallbackRotate(int val, void* userdata) {
	superimposeDisplay* psuperimposeDisplay = (superimposeDisplay*)userdata;
	psuperimposeDisplay->trackCallbackRotate(val);
}
void superimposeDisplay::trackCallbackZoom(int val, void* userdata) {
	superimposeDisplay* psuperimposeDisplay = (superimposeDisplay*)userdata;
	psuperimposeDisplay->trackCallbackZoom(val);
}

void superimposeDisplay::clearAndSetMenu(int menuSelection, menuMode whichMenu) {
	if (mMenuSelection != MENU_NONE) {
		mMenuSelected[mMenuSelection] = false;
	}
	mMenuSelection = menuSelection;
	if (menuSelection != MENU_NONE) {
		mMenuSelected[menuSelection] = true;
	}
}
void superimposeDisplay::flipMenu(int menuSelection, menuMode whichMenu) {
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
void superimposeDisplay::clearMenuSelections() {
	for (int idx = 0; idx < MENU_ALL_QTY; idx++) {
		mMenuSelected[idx] = false;
	}
}
void superimposeDisplay::clearMenuLocSelections() {
	for (int idx = MENU_SUB_LOC0; idx <= MENU_SUB_LOC3; idx++) {
		mMenuSelected[idx] = false;
	}
	for (int idx = MENU_GEO_WALLEND0; idx < MENU_GEO_DONE; idx++) {
		mMenuSelected[idx] = false;
	}


}
int distanceBetween(Point2f pt0, Point2f pt1) {
	int distX = pt0.x - pt1.x;
	int distY = pt0.y - pt1.y;
	return distX * distX + distY * distY;
}
int superimposeDisplay::findClosestGeometry(Point testLoc) {
	int retv = -1;
	int minDist = -1;
	for (int idx = 0; idx < mGeometryDescriptions.size(); idx++) {
		if (!(mGeometryDescriptions[idx]->isEmpty())) {
			int tmpDist = distanceBetween(testLoc, mGeometryDescriptions[idx]->getRefLoc());
			if (minDist < 0 || minDist > tmpDist) {
				minDist = tmpDist;
				retv = idx;
			}
		}
	}
	return retv;
}
bool superimposeDisplay::isErasedCell(int whichCell, int imageIdx) {
	bool retv = false;
	for (int idx = 0; idx < minusCellCentres[imageIdx].size(); idx++) {
		if (minusCellCentres[imageIdx][idx] == whichCell) {
			retv = true;
			break;
		}
	}
	return retv;
}
bool superimposeDisplay::isErasedConnection(int whichCell1, int whichCell2, int imageIdx) {
	bool retv = false;
	for (int idx = 0; idx < minusCellConnections[imageIdx].size(); idx++) {
		if ((minusCellConnections[imageIdx][idx].x == whichCell1 && minusCellConnections[imageIdx][idx].y == whichCell2) ||
			(minusCellConnections[imageIdx][idx].x == whichCell2 && minusCellConnections[imageIdx][idx].y == whichCell1)) {
			retv = true;
			break;
		}
	}
	return retv;
}
bool superimposeDisplay::alreadyAddedCellConnection(int whichCell1, int whichCell2, int imageIdx) {
	bool retv = false;
	for (int idx = 0; idx < addCellConnections[imageIdx].size(); idx++) {
		if ((addCellConnections[imageIdx][idx].x == whichCell1 && addCellConnections[imageIdx][idx].y == whichCell2) ||
			(addCellConnections[imageIdx][idx].x == whichCell2 && addCellConnections[imageIdx][idx].y == whichCell1)) {
			retv = true;
			break;
		}
	}
	return retv;

}
int superimposeDisplay::findCloseCell(Point testLoc, int imageIdx) {
	int retv = -1;
	int minDist = DEFAULT_RAD * 4;
	for (int idx = 0; idx < autoCellCentres[imageIdx].size(); idx++) {
		if (!isErasedCell(idx, imageIdx)) {
			int tmpDist = distanceBetween(testLoc, autoCellCentres[imageIdx][idx]);
			if (minDist > tmpDist) {
				minDist = tmpDist;
				retv = idx;
			}
		}
	}
	return retv;
}
int superimposeDisplay::findCloseCellCell(Point testLoc, int imageIdx) {

	int retv = -1;
	int minDist = DEFAULT_RAD * 4;
	for (int idx = 0; idx < allCells[imageIdx].size(); idx++) {
		int tmpDist = distanceBetween(testLoc, allCells[imageIdx][idx].mCentre);
		if (minDist > tmpDist) {
			minDist = tmpDist;
			retv = idx;
		}
	}
	return retv;
}
int superimposeDisplay::findCloseCellWall(Point testLoc, int imageIdx, int cellSelected) {

	int retv = -1;
	int minDist = DEFAULT_RAD * 4;
	for (int idx = 0; idx < allCells[imageIdx][cellSelected].mWallCentres.size(); idx++) {
		int tmpDist = distanceBetween(testLoc, allCells[imageIdx][cellSelected].mWallCentres[idx]);
		if (minDist > tmpDist) {
			minDist = tmpDist;
			retv = idx;
		}
	}
	return retv;
}
int superimposeDisplay::findCloseAddCell(Point testLoc, int imageIdx) {
	int retv = -1;
	int minDist = DEFAULT_RAD * 4;
	for (int idx = 0; idx < addCellCentres[imageIdx].size(); idx++) {
		int tmpDist = distanceBetween(testLoc, addCellCentres[imageIdx][idx]);
		if (minDist > tmpDist) {
			minDist = tmpDist;
			retv = idx;
		}
	}
	return retv;
}
int superimposeDisplay::findCloseConnection(Point testLoc, int *idxC1, int *idxC2, int imageIdx) {
	int retv = -1;
	int retv2 = -1;
	int minDist = DEFAULT_RAD * 2;
	for (int idx = 0; idx < autoCellConnections[imageIdx].size(); idx++) {
		for (int idxC = 0; idxC < autoCellConnections[imageIdx][idx].size(); idxC++) {
			int c2 = (autoCellConnections[imageIdx][idx])[idxC];
			if (!isErasedConnection(idx, c2, imageIdx)) {
				Point midway = (autoCellCentres[imageIdx][idx] + autoCellCentres[imageIdx][c2]) / 2;
				int tmpDist = distanceBetween(testLoc, midway);
				if (minDist > tmpDist) {
					minDist = tmpDist;
					retv = idx;
					retv2 = c2;
				}
			}
		}
	}
	*idxC1 = retv;
	*idxC2 = retv2;
	return retv;
}
int superimposeDisplay::findClosePotentialConnection(Point testLoc, int *idxC1, int *idxC2, int imageIdx) {
	int found = -1;
	int retv = -1;
	int retv2 = -1;
	int closeDist = DEFAULT_RAD * DEFAULT_RAD / 2;
	int minDist = DEFAULT_RAD * DEFAULT_RAD / 2;
	for (int idx = 0; idx < autoCellCentres[imageIdx].size(); idx++) {
		if (!isErasedCell(idx, imageIdx)) {
			Point2f loc0 = autoCellCentres[imageIdx][idx];
			int tmpDist = distanceBetween(testLoc, loc0);
			if (closeDist > tmpDist) {
				for (int idxC = 0; idxC < autoCellCentres[imageIdx].size(); idxC++) {
					if (idx != idxC) {
						if (!isErasedCell(idxC, imageIdx)) {
							Point midway = (loc0 + autoCellCentres[imageIdx][idxC]) / 2;
							tmpDist = distanceBetween(testLoc, midway);
							if (minDist > tmpDist) {
								minDist = tmpDist;
								found = 0;
								retv = idx;
								retv2 = idxC;
							}
						}
					}
				}
				for (int idxC = 0; idxC < addCellCentres[imageIdx].size(); idxC++) {
					Point midway = (autoCellCentres[imageIdx][idx] + addCellCentres[imageIdx][idxC]) / 2;
					tmpDist = distanceBetween(testLoc, midway);
					if (minDist > tmpDist) {
						minDist = tmpDist;
						found = 0;
						retv = idx;
						retv2 = -(idxC + 1);
					}
				}
			}
		}
	}
	for (int idx = 0; idx < addCellCentres[imageIdx].size(); idx++) {
		Point2f loc0 = addCellCentres[imageIdx][idx];
		int tmpDist = distanceBetween(testLoc, loc0);
		if (closeDist > tmpDist) {
			for (int idxC = 0; idxC < addCellCentres[imageIdx].size(); idxC++) {
				if (idx != idxC) {
					Point midway = (addCellCentres[imageIdx][idx] + addCellCentres[imageIdx][idxC]) / 2;
					tmpDist = distanceBetween(testLoc, midway);
					if (minDist > tmpDist) {
						minDist = tmpDist;
						found = 0;
						retv = -(idx + 1);
						retv2 = -(idxC + 1);
					}
				}
			}
		}
	}
	*idxC1 = retv;
	*idxC2 = retv2;
	return found;
}

void superimposeDisplay::removeGeometry(int whichOne) {
	if (mGeometryDescriptions.size() > 1) {
		if (whichOne >= 0 && whichOne < mGeometryDescriptions.size()) {
			mGeometryDescriptions.erase(mGeometryDescriptions.begin() + whichOne);
		}
	}
	else {
		mGeometryDescriptions[0]->emptyEntry();
	}
}
void superimposeDisplay::editGeometry(int whichOne) {
	mGeoBeingEdited = -1;
	if (whichOne >= 0 && whichOne < mGeometryDescriptions.size()) {
		mGeoBeingEdited = whichOne;
		mMenuHeightIdx = getCurrentGeoLocation()->mHeight;
		mMenuDepositIdx = getCurrentGeoLocation()->mDeposit;
		mMenuStr[MENU_GEO_HEIGHT] = mMenuHeightStr[mMenuHeightIdx];
		mMenuStr[MENU_GEO_DEPOSIT] = mMenuDepositStr[mMenuDepositIdx];

	}
}
void superimposeDisplay::geoEntryDone() {
	if (mGeoBeingEdited >= 0) {
		mGeoBeingEdited = -1;

	}
	else { // otherwise, this is an entry
		// nothing to do yet
	}
}

int superimposeDisplay::getClosestRefLocation(bool staticNotMobile, int x, int y) {
	if (!staticNotMobile && mApplyDistortion) {
		Point2f adjustedLoc = mPersp.adjustedPerspective(Point2f(x, y), true, true);
		x = adjustedLoc.x;
		y = adjustedLoc.y;
	}
	perspectiveImageReference *relevantPerspective = (staticNotMobile) ? &mPerspectiveStatic : &mPerspectiveMobile;
	int retv = 0;
	float trySq = (relevantPerspective->orgTL.x - x);
	trySq *= trySq;
	float trySqy = (relevantPerspective->orgTL.y - y);
	trySq += trySqy * trySqy;
	float minSq = trySq;
	trySq = (relevantPerspective->orgTR.x - x);
	trySq *= trySq;
	trySqy = (relevantPerspective->orgTR.y - y);
	trySq += trySqy * trySqy;
	if (trySq < minSq) {
		retv = 1;
		minSq = trySq;
	}
	trySq = (relevantPerspective->orgBR.x - x);
	trySq *= trySq;
	trySqy = (relevantPerspective->orgBR.y - y);
	trySq += trySqy * trySqy;
	if (trySq < minSq) {
		retv = 2;
		minSq = trySq;
	}
	trySq = (relevantPerspective->orgBL.x - x);
	trySq *= trySq;
	trySqy = (relevantPerspective->orgBL.y - y);
	trySq += trySqy * trySqy;
	if (trySq < minSq) {
		retv = 3;
		minSq = trySq;
	}
	return retv;
}

void superimposeDisplay::setRefLocation(int whichPoint, bool staticNotMobile, int x, int y) {
	Point2f rescaledPoint = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
	if (!staticNotMobile && mApplyDistortion) {
		rescaledPoint = mPersp.adjustedPerspective(rescaledPoint, true, true);
	}
	x = rescaledPoint.x;
	y = rescaledPoint.y;
	perspectiveImageReference *relevantPerspective = (staticNotMobile)? &mPerspectiveStatic : &mPerspectiveMobile;
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
void superimposeDisplay::flagRefLocationAsSet(int whichPoint, bool staticNotMobile) {
	bool *relevantFlags = (staticNotMobile)? &mHaveStaticLocs[0] : &mHaveMobileLocs[0];
	relevantFlags[whichPoint] = true;
	bool allAreSet = relevantFlags[0] && relevantFlags[1] && relevantFlags[2] && relevantFlags[3];
	if (staticNotMobile) {
		mHaveAllStaticLocs = allAreSet;
	}
	else {
		mHaveAllMobileLocs = allAreSet;
	}
}
/*
#define MENU_GEO_CURVEENDSTART       wallEnd0;
#define MENU_GEO_CURVEENDEND MENU_GEO_WALLEND1 wallEnd1
#define MENU_GEO_CURVE_THIRD_OR_HALF MENU_GEO_WALLENTRE bentCentre
#define MENU_GEO_CURVE_2THIRDS MENU_GEO_PIT0
#define MENU_GEO_CURVE_2THIRDS_SKIP MENU_GEO_PIT1 pit1
#define MENU_GEO_CURVE_THIRD_WALL MENU_GEO_BUILTWALL00 builtWall00
#define MENU_GEO_CURVE_HALF_WALL MENU_GEO_BUILTWALL01 builtWall01
#define MENU_GEO_CURVE_HALF_STOP MENU_GEO_BUILTWALL10
#define MENU_GEO_CURVE_2THIRDS_WALL MENU_GEO_BUILTWALL11 builtWall11
#define MENU_GEO_CURVE_STOP MENU_GEO_DONE
*/

int superimposeDisplay::getCurveNextPoint(int putativeNextPoint) {
	int retv = -1;
	geometryDesc *pGeo = getCurrentGeoLocation();
	switch (putativeNextPoint + MENU_GEO_WALLEND0) {
	case MENU_GEO_CURVEENDEND:
	case MENU_GEO_CURVE_THIRD_OR_HALF:
		retv = putativeNextPoint + MENU_GEO_WALLEND0;
		break;
	case MENU_GEO_CURVE_2THIRDS:
		{
			Point2f oneThird = (pGeo->wallEnd0 + pGeo->wallEnd0 + pGeo->wallEnd1) / 3;
			Point2f oneHalf = (pGeo->wallEnd0 + pGeo->wallEnd1) / 2;
			float dist1thrid = magnitude(oneThird - pGeo->bentCentre);
			float disthalf = magnitude(oneHalf - pGeo->bentCentre);
			if (disthalf < dist1thrid) {
				retv = MENU_GEO_CURVE_HALF_WALL;
			}
			else {
				retv = MENU_GEO_CURVE_2THIRDS;
			}
		}
		break;
	case MENU_GEO_CURVE_2THIRDS_SKIP:
		retv = MENU_GEO_CURVE_THIRD_WALL;
		break;
	case MENU_GEO_CURVE_HALF_WALL:
		if (pGeo->isEmpty(pGeo->builtWall00)) {
			retv = -1;
		}
		else {
			retv = MENU_GEO_CURVE_HALF_WALL;

		}
		break;
	case MENU_GEO_CURVE_HALF_STOP:
		if (pGeo->isEmpty(pGeo->bentCentre)) {
			retv = -1;
		}
		else {
			retv = MENU_GEO_CURVE_2THIRDS_WALL;
		}
		break;
	}
	if (retv >= 0) retv -= MENU_GEO_WALLEND0;
	return retv;
}
geometryDesc::shape superimposeDisplay::getGeometryTypeBeingEdited() {
	geometryDesc::shape retv;
	retv = getCurrentGeoLocation()->mShape;
	return retv;
}
geometryDesc* superimposeDisplay::getCurrentGeoLocation() {
	geometryDesc* retv = mGeometryDescriptions.back();
	if (mGeoBeingEdited >= 0) {
		retv = mGeometryDescriptions.at(mGeoBeingEdited);
	}
	return retv;
}

void	superimposeDisplay::setGeoLocation(Mat baseImage, geometryDesc *pCurrentGeo, int whichPoint, int x, int y) {
	Point2f adjustedPt = Point(0, 0);
	bool clearPt = true;
	if (x > 0 && y > 0) {
		adjustedPt = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
		clearPt = false;
	}
	switch (whichPoint) {
		case MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0:
			pCurrentGeo->wallEnd0 = adjustedPt;
			break;
		case MENU_GEO_WALLEND1 - MENU_GEO_WALLEND0:
			pCurrentGeo->wallEnd1 = adjustedPt;
			break;
		case MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0:
			pCurrentGeo->bentCentre = adjustedPt;
			pCurrentGeo->builtWall1Used = clearPt;
			break;
		case MENU_GEO_PIT0 - MENU_GEO_WALLEND0:
			pCurrentGeo->pit0 = adjustedPt;
			break;
		case MENU_GEO_PIT1 - MENU_GEO_WALLEND0:
			pCurrentGeo->pit1 = adjustedPt;
			pCurrentGeo->pit1Used = clearPt;
			break;
		case MENU_GEO_PIT2 - MENU_GEO_WALLEND0:
			pCurrentGeo->pit2 = adjustedPt;
			pCurrentGeo->pit2Used = clearPt;
			break;
		case MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0:
			pCurrentGeo->builtWall00 = adjustedPt;
			break;
		case MENU_GEO_BUILTWALL01 - MENU_GEO_WALLEND0:
			pCurrentGeo->builtWall01 = adjustedPt;
			break;
		case MENU_GEO_BUILTWALL10 - MENU_GEO_WALLEND0:
			pCurrentGeo->builtWall10 = adjustedPt;
			pCurrentGeo->builtWall1Used = clearPt;
			break;
		case MENU_GEO_BUILTWALL11 - MENU_GEO_WALLEND0:
			pCurrentGeo->builtWall11 = adjustedPt;
			pCurrentGeo->builtWall1Used = clearPt;
			break;
	}
}
float superimposeDisplay::randomOffset(float maxMM) {
	float scale = mConfig.getScale();
	float maxPixels = scale * maxMM;
	float r = rand();
	r /= (RAND_MAX / 2);
	r -= 1;
	return r * maxPixels;
}

float superimposeDisplay::randomRange(float min, float max) {
	float r = rand();
	r /= (RAND_MAX);
	r *= (max - min);
	r += min;
	return r;
}

void superimposeDisplay::mouseCallback(int event, int x, int y, int flags) {
	int imageIdx = mDisplayedImageIdx;
	bool redisplay = false;

	bool adviseOfChange = false;
	if (x < 0) x = 0;
	if (x >= mCols) x = mCols - 1;
	if (y < 0) y = 0;
	if (y >= mRows) y = mRows - 1;

	int tmpMenu = menuSelected(x, y, mMenuMode);
	if (tmpMenu != MENU_NONE) {
		if (event == EVENT_LBUTTONDOWN) {
			mRtClickedMenu = false;

		} else if (event == EVENT_RBUTTONDOWN)
		{
			mRtClickedMenu = true;
			event = EVENT_LBUTTONDOWN;
		}
	}

	if (event == EVENT_LBUTTONDOWN)
	{
		int tmpSliderVal = -1; // mTrackVal;
		if (tmpMenu != MENU_NONE) {
			switch (tmpMenu) {
			case MENU_0:
				tmpSliderVal = 1;
				break;
			case MENU_25:
				tmpSliderVal = 25;
				break;
			case MENU_50:
				tmpSliderVal = 50;
				break;
			case MENU_75:
				tmpSliderVal = 75;
				break;
			case MENU_100:
				tmpSliderVal = 100;
				break;
			case MENU_DISTORT:
			case MENU_SUB_DISTORT:
				mMenuSelected[MENU_DISTORT] = mMenuSelected[MENU_SUB_DISTORT] = mApplyDistortion = !mApplyDistortion;
				mCellsNeedUpdate = true;
				break;
			case MENU_CLEAR_OFFSET:
				mCurrentOffsetX = 0;
				mCurrentOffsetY = 0;
				break;
			case MENU_LOCS_STATIC:
				tmpSliderVal = 100;
				clearMenuSelections();
				mMenuMode = menuMode::reflocs;
				mWhichPoint = -2;
				mStaticPointsNotMobile = (tmpMenu == MENU_LOCS_STATIC);
				mMenuSelection = MENU_NONE;
				break;
			case MENU_LOCS_MOBILE:
				tmpSliderVal = 0;
				clearMenuSelections();
				mMenuMode = menuMode::reflocs;
				mWhichPoint = -2;
				mStaticPointsNotMobile = (tmpMenu == MENU_LOCS_STATIC);
				mMenuSelection = MENU_NONE;
				break;
			case MENU_SAVE:
				saveConfigFile();
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
			case MENU_SUB_SCALE0:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				mWhichPoint = 0;
				break;
			case MENU_SUB_SCALE1:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				mWhichPoint = 1;
				break;
			case MENU_SUB_SCALECELL:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				break;
			case MENU_SUB_SCALECELLWALL:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				break;
			case MENU_GEO_BENT:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::bent;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_WALLEND0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLEND0;
				mWhichPoint = 0;
				break;
			case MENU_GEO_BULGE:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::bulge;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_WALLCENTRE] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLCENTRE;
				mWhichPoint = MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_BULGE2:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::bulge2;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_WALLCENTRE] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLCENTRE;
				mWhichPoint = MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_RIM1PIT: 	// rim1pit , centre of pit and up to 3 intersections start/end in wallEnd0-wallEnd1, builtWall00-01, 10-11 .. each assumed to run clockwise
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::rim1pit;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_PIT0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_PIT0;
				mWhichPoint = MENU_GEO_PIT0 - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_RIM2PITS:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::rim2pits;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_PIT0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_PIT0;
				mWhichPoint = MENU_GEO_PIT0 - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_PITCELL1:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::pitcell1;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_PIT0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_PIT0;
				mWhichPoint = MENU_GEO_PIT0 - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_PITCELL2:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::pitcell2;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_PIT0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_PIT0;
				mWhichPoint = MENU_GEO_PIT0 - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_ZIGZAG:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::zigzag;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_PIT0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLEND0;
				mWhichPoint = MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_CURVE:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::curve;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_WALLEND0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLEND0;
				mWhichPoint = MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0;
				break;
			case MENU_GEO_END:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::wallEnd;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_WALLEND0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLEND0;
				mWhichPoint = 0;
				break;
			case MENU_GEO_SIDE:
				if (!mGeometryDescriptions.back()->isEmpty()) {
					mGeometryDescriptions.push_back(new geometryDesc());
				}
				mGeometryDescriptions.back()->mShape = geometryDesc::shape::straight;
				clearMenuLocSelections();
				mMenuSelected[MENU_GEO_WALLEND0] = true;
				primaryClickStatus.menuChoice = MENU_GEO_WALLEND0;
				mWhichPoint = 0;
				break;
			case MENU_GEO_HEIGHT:
				mMenuHeightIdx++;
				if (mMenuHeightIdx >= MENU_HEIGHTIDX_QTY) {
					mMenuHeightIdx = 0;
				}
				mMenuStr[MENU_GEO_HEIGHT] = mMenuHeightStr[mMenuHeightIdx];
				getCurrentGeoLocation()->mHeight = mMenuHeightIdx;
				break;
			case MENU_GEO_DEPOSIT:
				mMenuDepositIdx++;
				if (mMenuDepositIdx >= MENU_DEPOSITIDX_QTY) {
					mMenuDepositIdx = 0;
				}
				mMenuStr[MENU_GEO_DEPOSIT] = mMenuDepositStr[mMenuDepositIdx];
				getCurrentGeoLocation()->mDeposit = mMenuDepositIdx;
				break;
			case MENU_GEO_WALLEND0:
			case MENU_GEO_WALLEND1:
			case MENU_GEO_WALLCENTRE:
			case MENU_GEO_PIT0:
			case MENU_GEO_PIT1:
			case MENU_GEO_PIT2:
			case MENU_GEO_BUILTWALL00:
			case MENU_GEO_BUILTWALL01:
			case MENU_GEO_BUILTWALL10:
			case MENU_GEO_BUILTWALL11:
				if (mMenuSelected[tmpMenu]) {
					setGeoLocation(mBaseImage, getCurrentGeoLocation(), mWhichPoint, 0, 0);
				}
				else {
					clearMenuLocSelections();
					mMenuSelected[tmpMenu] = true;
					primaryClickStatus.menuChoice = tmpMenu;
					mWhichPoint = tmpMenu - MENU_GEO_WALLEND0;
				}
				break;
			case MENU_GEO_EDIT:
			case MENU_GEO_DELETE:
			case MENU_ZOOM_CENTRE:
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
			case MENU_SUB_STATIC:
			case MENU_SUB_MOBILE:
				clearMenuSelections();
				mWhichPoint = -1;
				mMenuMode = menuMode::reflocs;
				mStaticPointsNotMobile = (tmpMenu == MENU_SUB_STATIC);
				mMenuSelection = MENU_NONE;
				break;
			case MENU_GEO:
				clearMenuSelections();
				mWhichPoint = -1;
				mMenuMode = menuMode::geometries;
				mMenuSelection = MENU_NONE;
				break;
			case MENU_ADJ_CELL:
				clearMenuSelections();
				mWhichPoint = -1;
				mMenuMode = menuMode::cells;
				mMenuSelection = MENU_NONE;
				break;
			case MENU_SUB_CELLBOUNDARY:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = true;
				primaryClickStatus.menuChoice = tmpMenu;
				break;
			case MENU_ADJ_REDO:
			case MENU_ADJ_WALL:
			case MENU_ADJ_SELCELL:
			case MENU_ADJ_SHOWCELLS:
			case MENU_GEO_SHOWCELL:
			case MENU_GEO_CELLCENTRE:
			case MENU_GEO_CELLCONN:
				clearMenuLocSelections();
				mMenuSelected[tmpMenu] = !mMenuSelected[tmpMenu];
				primaryClickStatus.menuChoice = (mMenuSelected[tmpMenu])? tmpMenu : MENU_NONE;
				mWhichPoint = -1;
				break;
			case MENU_GEO_CELLS:
				if (mAdjustableDisplay != nullptr) {
					vector<vector<int>> *pCellConnections;
					vector<vector<Point>> *pCellContours;
					vector<Point2f> *pCellCentres;

					mAdjustableDisplay->getCellContoursCentresAndConnections(&pCellContours, &pCellCentres, &pCellConnections);
					autoCellContours[imageIdx].clear();
					autoCellCentres[imageIdx].clear();
					autoCellConnections[imageIdx].clear();
					for (int idx = 0; idx < pCellContours->size(); idx++) {
						vector<Point> *pTmpContour;
						pTmpContour = new vector<Point>();
						for (int idxC = 0; idxC < pCellContours->at(idx).size(); idxC++) {
							pTmpContour->push_back(pCellContours->at(idx)[idxC] * mDispDivision);
						}
						autoCellContours[imageIdx].push_back(*pTmpContour);
					}
					for (int idx = 0; idx < pCellCentres->size(); idx++) {
						autoCellCentres[imageIdx].push_back(pCellCentres->at(idx) * mDispDivision);
					}
					for (int idx = 0; idx < pCellConnections->size(); idx++) {
						autoCellConnections[imageIdx].push_back(pCellConnections->at(idx));
					}
					mCellsNeedUpdate = true;
				}
				
			case MENU_GEO_DONE:
			case MENU_SUB_DONE:
			case MENU_ADJ_DONE:
				clearMenuSelections();
				mMenuMode = menuMode::topMenu;
				mMenuSelection = MENU_NONE;
				break;
			}
			if (tmpSliderVal >= 0) { //mMenuMode == menuMode::topMenu) {
				mDispContrastTrackVal = tmpSliderVal;
				cvSetTrackbarPos(mSliderName, "mCWindowName", tmpSliderVal);
			}
			primaryClickStatus.clickedOnMenuItem = true;
		}
		else {
			primaryClickStatus.clickedOnMenuItem = false;
			if (mMenuMode == menuMode::reflocs) {
				if (primaryClickStatus.menuChoice == MENU_SUB_SCALE0) {
					mScalePt0 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus); 
					mMenuSelected[MENU_SUB_SCALE0] = false;
					mMenuSelected[MENU_SUB_SCALE1] = true;
					primaryClickStatus.menuChoice = MENU_SUB_SCALE1;
				}
				else if (primaryClickStatus.menuChoice == MENU_SUB_SCALE1) {
					mScalePt1 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus); 
					mMenuSelected[MENU_SUB_SCALE1] = false;
					primaryClickStatus.menuChoice = MENU_NONE;
				}
				else if (primaryClickStatus.menuChoice == MENU_SUB_SCALECELL) {
					mCellScale0 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
				}
				else if (primaryClickStatus.menuChoice == MENU_SUB_SCALECELLWALL) {
					mCellScale0 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
				}
				else if (primaryClickStatus.menuChoice == MENU_SUB_CELLBOUNDARY) {
					mCellBoundary0 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
				}
				else {
					bool presetPoint = true;
					//if (!(mWhichPoint >= 0 && mWhichPoint <= 3)) {
					//	mWhichPoint = getClosestRefLocation(mStaticPointsNotMobile, x, y);
					//	presetPoint = false;
					//}
					setRefLocation(mWhichPoint, mStaticPointsNotMobile, x, y);
					flagRefLocationAsSet(mWhichPoint, mStaticPointsNotMobile);
					mCellsNeedUpdate = true;
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

				}
				redisplay = true;
			}
			else if (mMenuMode == menuMode::geometries) {
				if (primaryClickStatus.menuChoice == MENU_GEO_DELETE) {
					int whichGeoDesc = findClosestGeometry(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus));
					if (whichGeoDesc >= 0) {
						removeGeometry(whichGeoDesc);
					}
					mMenuSelected[MENU_GEO_DELETE] = false;
					mMenuSelection = MENU_NONE;
				}
				else if (primaryClickStatus.menuChoice == MENU_GEO_EDIT) {
					int whichGeoDesc = findClosestGeometry(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus));
					if (whichGeoDesc >= 0) {
						editGeometry(whichGeoDesc);
					}
					else {
						mMenuSelected[MENU_GEO_EDIT] = false;
						mGeoBeingEdited = -1;
					}
					mMenuSelection = MENU_NONE;
				}
				else if (primaryClickStatus.menuChoice == MENU_GEO_CELLCENTRE && !mMenuSelected[MENU_GEO_CELLCONN]) {
					int whichCell = findCloseCell(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx);
					if (whichCell >= 0) {
						minusCellCentres[imageIdx].push_back(whichCell);
						mCellsNeedUpdate = true;
					}
					else {
						whichCell = findCloseAddCell(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx);
						if (whichCell < 0) {
							addCellCentres[imageIdx].push_back(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus));
							mCellsNeedUpdate = true;
						}

					}
				}
				else if (primaryClickStatus.menuChoice == MENU_GEO_CELLCONN && !mMenuSelected[MENU_GEO_CELLCENTRE]) {
					int whichCell1, whichCell2;
					int found = findCloseConnection(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), &whichCell1, &whichCell2, imageIdx);
					if (found >= 0) {
						Point2i loc0, loc1;
						minusCellConnections[imageIdx].push_back(Point2i(whichCell1, whichCell2));
						mCellsNeedUpdate = true;
					}
					else {
						int found = findClosePotentialConnection(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), &whichCell1, &whichCell2, imageIdx);
						if (found >= 0 && !alreadyAddedCellConnection(whichCell1, whichCell2, imageIdx)) {
							addCellConnections[imageIdx].push_back(Point2i(whichCell1, whichCell2));
							mCellsNeedUpdate = true;
						}
					}

				}
				else if (primaryClickStatus.menuChoice == MENU_GEO_SHOWCELL) {
					int whichCell = findCloseCellCell(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx);
					if (whichCell >= 0) {
						showCell = whichCell; //new cvCell(autoCellContours[whichCell], whichCell , autoCellCentres, autoCellConnections, addCellCentres, addCellConnections, minusCellCentres, minusCellConnections);
					//}
					//else {
					//	whichCell = findCloseAddCell(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus));
					//	if (whichCell >= 0) {
					//		showCell = new cvCell(-(whichCell + 1), autoCellCentres, addCellCentres, addCellConnections);
					//	}
					}
				}
				else {
					if (mRtClickedMenu) {
						if (getCurrentGeoLocation()->mShape == geometryDesc::shape::bent) {
							x += randomOffset(CELL_SCALE_RAND_PLACEMENT_MM );
							y += randomOffset(CELL_SCALE_RAND_PLACEMENT_MM);
							getCurrentGeoLocation()->mRandom = mRtClickedMenu;
							float angle0 = randomRange(0, Math::PI * 2);
							float angle1 = randomRange(Math::PI / 2, Math::PI * 4 / 5);
							setGeoLocation(mBaseImage, getCurrentGeoLocation(), (MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0), x, y);
							float x0 = x + Math::Sin(angle0) * 2 * CELL_SAMPLE_RANGE_MM;
							float y0 = y + Math::Cos(angle0) * 2 * CELL_SAMPLE_RANGE_MM;
							setGeoLocation(mBaseImage, getCurrentGeoLocation(), (MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0), x0, y0);
							x0 = x + Math::Sin(angle0 + angle1) * 2 * CELL_SAMPLE_RANGE_MM;
							y0 = y + Math::Cos(angle0 + angle1) * 2 * CELL_SAMPLE_RANGE_MM;
							setGeoLocation(mBaseImage, getCurrentGeoLocation(), (MENU_GEO_WALLEND1 - MENU_GEO_WALLEND0), x0, y0);
							mWhichPoint = (MENU_GEO_PIT0 - MENU_GEO_WALLEND0);

						}
						if (getCurrentGeoLocation()->mShape == geometryDesc::shape::rim2pits) {
							x += randomRange(-0.8f, 0.8f) * CELL_SAMPLE_RANGE_MM * mZoom;
							y += randomRange(-0.8f, 0.8f) * CELL_SAMPLE_RANGE_MM * mZoom;
							getCurrentGeoLocation()->mRandom = mRtClickedMenu;
							float angle0 = randomRange(0, Math::PI * 2);
							setGeoLocation(mBaseImage, getCurrentGeoLocation(), (MENU_GEO_PIT0 - MENU_GEO_WALLEND0), x, y);
							float pitgap = randomRange(1, 1.25f) * CELL_SAMPLE_RANGE_MM * mZoom / 2;
							float x0 = x + Math::Sin(angle0) * pitgap ;
							float y0 = y + Math::Cos(angle0) * pitgap;
							setGeoLocation(mBaseImage, getCurrentGeoLocation(), (MENU_GEO_PIT1 - MENU_GEO_WALLEND0), x0, y0);
							mWhichPoint = (MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0) - 1;
						}
						else {
							x += randomOffset(CELL_SCALE_RAND_PLACEMENT_MM);
							y += randomOffset(CELL_SCALE_RAND_PLACEMENT_MM);
							getCurrentGeoLocation()->mRandom = mRtClickedMenu;
							setGeoLocation(mBaseImage, getCurrentGeoLocation(), mWhichPoint, x, y);
						}
						mRtClickedMenu = false;
					}
					else {
						setGeoLocation(mBaseImage, getCurrentGeoLocation(), mWhichPoint, x, y);
					}
				}
				redisplay = true;
			}
			else if (mMenuMode == menuMode::cells) {
				if (primaryClickStatus.menuChoice == MENU_ADJ_REDO) {
					int whichCell = findCloseCellCell(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx);
					if (whichCell >= 0) {
						showCell = whichCell;
						clearStoredWallCentres(imageIdx, allCells[imageIdx][ whichCell].mID);
					}
				}
				if (primaryClickStatus.menuChoice == MENU_ADJ_SELCELL) {
					int whichCell = findCloseCellCell(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx);
					if (whichCell >= 0) {
						showCell = whichCell;
					}
				}
				if (primaryClickStatus.menuChoice == MENU_ADJ_WALL) {
					if (showCell >= 0) {
						int whichWall = findCloseCellWall(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx, showCell);
						selectedCellSelectedWall = whichWall;
					}
				}
			} else {
				if (primaryClickStatus.menuChoice == MENU_ZOOM_CENTRE) {
					mZoomLocus = unResizePoint( Point(x, y), mZoom, mZoom, mZoomLocus);
					clearMenuSelections();
					mMenuMode = menuMode::topMenu;
					mMenuSelection = MENU_NONE;
				}
			}
		}
		primaryClickStatus.mousedown = true;
		primaryClickStatus.startX = x;
		primaryClickStatus.startY = y;
		mOffsetX = mCurrentOffsetX;
		mOffsetY = mCurrentOffsetY;
		redisplay = true;
	}
	if (event == EVENT_RBUTTONDOWN)
	{
		if (mMenuMode == menuMode::reflocs) {
			if (!(mWhichPoint >= 0 && mWhichPoint <= 3)) {
				mWhichPoint = getClosestRefLocation(true, x, y);
				setRefLocation(mWhichPoint, true, x, y);
				flagRefLocationAsSet(mWhichPoint, true);
				setRefLocation(mWhichPoint, false, x, y);
				flagRefLocationAsSet(mWhichPoint, false);
				mWhichPoint = -1;
				redisplay = true;
			}
		}
		else {
			rightClick(x, y);
		}
	}
	if (event == EVENT_MOUSEWHEEL) {
		float wheelIncr = 1.0f;
		int wheelData = getMouseWheelDelta(flags);
		if (wheelData < 0) {
			mDispContrastTrackVal = 0;
		}
		else {
			mDispContrastTrackVal = 100;
		}
		mDisplayedImageIdx = (mDispContrastTrackVal > 50) ? 0 : 1;
		showCell = -1;
		cvSetTrackbarPos(mSliderName, "mCWindowName", mDispContrastTrackVal);
		showImg();
	}
	if (event == EVENT_LBUTTONUP)
	{
		primaryClickStatus.mousedown = false;

		if (mMenuMode == menuMode::geometries) {
			if (!primaryClickStatus.clickedOnMenuItem) {
				int tmpMenu = mWhichPoint + MENU_GEO_WALLEND0;
				if (mWhichPoint >= 0 && mWhichPoint < (MENU_GEO_DONE - MENU_GEO_WALLEND0) - 1) {
					mMenuSelected[tmpMenu] = false;
					if (mWhichPoint < (MENU_GEO_DONE - MENU_GEO_WALLEND0) - 1) {
						mWhichPoint++;
						geometryDesc::shape curShape = superimposeDisplay::getGeometryTypeBeingEdited();
						switch (curShape) {
						case geometryDesc::shape::bent:
							if (mWhichPoint == (MENU_GEO_PIT2 - MENU_GEO_WALLEND0))
								mWhichPoint++; // skip 2nd pit and go to built wall0
							//else if (mWhichPoint == (MENU_GEO_BUILTWALL10 - MENU_GEO_WALLEND0))
							//	mWhichPoint = -1; // finsh
							break;
						case geometryDesc::shape::bulge:
							if (mWhichPoint == (MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0))
								mWhichPoint = -1; // skip after 3 pits pit and finsh
							else if (mWhichPoint == (MENU_GEO_PIT0 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_WALLEND1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_PIT0 - MENU_GEO_WALLEND0;
							break;
						case geometryDesc::shape::bulge2:
							if (mWhichPoint == (MENU_GEO_BUILTWALL11 - MENU_GEO_WALLEND0))
								mWhichPoint = -1; // skip after 1 pits pit and 2 walls
							else if (mWhichPoint == (MENU_GEO_PIT1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0;
							break;
						case geometryDesc::shape::rim1pit:
							// rim1pit , centre of pit and up to 3 intersections start/end in wallEnd0-wallEnd1, builtWall00-01, 10-11 .. each assumed to run clockwise
							if (mWhichPoint == (MENU_GEO_PIT1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0;
							break;
						case geometryDesc::shape::rim2pits:
							if (mWhichPoint == (MENU_GEO_PIT2 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0;
							break;
//#define MENU_GEO_WALLEND0 (1 + MENU_GEO_DEPOSIT)
//#define MENU_GEO_WALLEND1 (1 + MENU_GEO_WALLEND0)
//#define MENU_GEO_WALLCENTRE (1 + MENU_GEO_WALLEND1)
//#define MENU_GEO_PIT0 (1 + MENU_GEO_WALLCENTRE)
//#define MENU_GEO_PIT1 (1 + MENU_GEO_PIT0)
//#define MENU_GEO_PIT2 (1 + MENU_GEO_PIT1)
//#define MENU_GEO_BUILTWALL00 (1 + MENU_GEO_PIT2)
//#define MENU_GEO_BUILTWALL01 (1 + MENU_GEO_BUILTWALL00)
//#define MENU_GEO_BUILTWALL10 (1 + MENU_GEO_BUILTWALL01)
//#define MENU_GEO_BUILTWALL11 (1 + MENU_GEO_BUILTWALL10)
						case geometryDesc::shape::pitcell1:
							if (mWhichPoint == (MENU_GEO_PIT1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_WALLEND1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_BUILTWALL10 - MENU_GEO_WALLEND0))
								mWhichPoint = -1;
							break;
						case geometryDesc::shape::pitcell2:
							if (mWhichPoint == (MENU_GEO_PIT1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_WALLEND0 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_WALLEND1 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_PIT1 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_PIT2 - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_WALLEND1 - MENU_GEO_WALLEND0;
							else if (mWhichPoint == (MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0))
								mWhichPoint = MENU_GEO_BUILTWALL00 - MENU_GEO_WALLEND0;
							break;
						//case geometryDesc::shape::zigzag:
						//	mWhichPoint = -1;
						//	break;
						case geometryDesc::shape::curve:
							mWhichPoint = getCurveNextPoint(mWhichPoint);
							break;
						case geometryDesc::shape::straight:
							if (mWhichPoint == (MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0))
								mWhichPoint++; // skip wall centre for straight
							break;
						case geometryDesc::shape::wallEnd:
							if (mWhichPoint == (MENU_GEO_WALLCENTRE - MENU_GEO_WALLEND0))
								mWhichPoint++; // skip wall centre for end
							break;
						}
						mMenuSelected[mWhichPoint + MENU_GEO_WALLEND0] = true;
						primaryClickStatus.menuChoice = mWhichPoint + MENU_GEO_WALLEND0;
						primaryClickStatus.clickedOnMenuItem = true; // spoof the effect of having clicked on the next menu item
					}
					 else 
					{
						mWhichPoint = -1;
					}
				}
				else if (mWhichPoint >= 0 && mWhichPoint == (MENU_GEO_DONE - MENU_GEO_WALLEND0) - 1) {
					mMenuSelected[tmpMenu] = false;
				}
				redisplay = true;
			}
		}
		else if (mMenuMode == menuMode::cells) {
			if (primaryClickStatus.menuChoice == MENU_ADJ_WALL && showCell != -1 && selectedCellSelectedWall != -1) {
				int cellID = allCells[imageIdx][showCell].mID;
				int peerID = allCells[imageIdx][showCell].mConnectionIDs[selectedCellSelectedWall];
				updateCellWall(unResizePoint(Point(x, y), mZoom, mZoom, mZoomLocus), imageIdx, cellID, peerID);
				selectedCellSelectedWall = -1;
				redisplay = true;
			}
		}
		else {
			mCurrentOffsetX = mOffsetX;
			mCurrentOffsetY = mOffsetY;
			if (mWhichPoint == -2) {
				mWhichPoint = 0;
			}
			if (!primaryClickStatus.clickedOnMenuItem) {
				if (primaryClickStatus.menuChoice == MENU_SUB_SCALECELL) {
					mCellScale1 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
					mConfig.cellScaleRefs.setCell(mCellScale0, mCellScale1);
					mConfig.cellScaleRefs.increment(false);
					clearMenuSelections();
					primaryClickStatus.menuChoice = MENU_NONE;
					redisplay = true;
				} else if (primaryClickStatus.menuChoice == MENU_SUB_SCALECELLWALL) {
					mCellScale1 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
					mConfig.cellScaleRefs.setCellWall(mCellScale0, mCellScale1);
					mConfig.cellScaleRefs.increment(true);
					clearMenuSelections();
					primaryClickStatus.menuChoice = MENU_NONE;
					redisplay = true;
				} else if (primaryClickStatus.menuChoice == MENU_SUB_CELLBOUNDARY) {
					mCellBoundary1 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
					clearMenuSelections();
					primaryClickStatus.menuChoice = MENU_NONE;
					redisplay = true;
				}
				clearMenuLocSelections();
				mWhichPoint = -1;
			}
			primaryClickStatus.clickedOnMenuItem = false;

		}

	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (primaryClickStatus.mousedown) {
			if ((mMenuMode == menuMode::topMenu)) {
				mOffsetX = mCurrentOffsetX + (x - primaryClickStatus.startX);
				mOffsetY = mCurrentOffsetY + (y - primaryClickStatus.startY);
				redisplay = true;
			}
			if ((mMenuMode == menuMode::reflocs)) {
			//	if ((mWhichPoint >= 0 && mWhichPoint <= 3)) {
			//		setRefLocation(mWhichPoint, true, x, y);
			//		showImg();
			//	}
				if (primaryClickStatus.menuChoice == MENU_SUB_SCALECELL) {
					mCellScale1 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
					mConfig.cellScaleRefs.setCell(mCellScale0, mCellScale1);
					redisplay = true;
				}
				else if (primaryClickStatus.menuChoice == MENU_SUB_SCALECELLWALL) {
					mCellScale1 = unResizePoint(Point2f(x, y), mZoom, mZoom, mZoomLocus);
					mConfig.cellScaleRefs.setCellWall(mCellScale0, mCellScale1);
					redisplay = true;
				}
			}
			if (mMenuMode == menuMode::geometries) {
				if (!primaryClickStatus.clickedOnMenuItem) {
					if (mWhichPoint >= 0 && mWhichPoint <= (MENU_GEO_DONE - MENU_GEO_WALLEND0)) {
						setGeoLocation(mBaseImage, mGeometryDescriptions.back(), mWhichPoint, x, y);
					}
					redisplay = true;
				}
			}
		}
	}
	if (redisplay)
		showImg();

}
void superimposeDisplay::rightClick(int x, int y) {
	Mat tmpSTaticImage;
	float mag = 16 / mDispDivision;
	int orgX = x * mDispDivision;
	int orgY = y * mDispDivision;
	float dispDims = 768;
	float extractDims = 2 * ( dispDims / mag);
	int left = 0;
	if (orgX > extractDims / 2) {
		left = orgX - (extractDims / 2);
	}
	int top = 0;
	if (orgY > extractDims / 2) {
		top = orgY - (extractDims / 2);
	}
	tmpSTaticImage = mBaseImage.rowRange(top, top + extractDims).colRange(left, left + extractDims);
	resize(tmpSTaticImage, tmpSTaticImage, cv::Size(dispDims, dispDims), 0, 0, CV_INTER_LINEAR);
	imshow("protractor", tmpSTaticImage);
}
void superimposeDisplay::trackCallback(int val) {
	mDispContrastTrackVal = val;
	mDisplayedImageIdx = (mDispContrastTrackVal > 50) ? 0 : 1;
	showImg();
}
void superimposeDisplay::rescaleSuperImage() {
	mSuperImage.copyTo(mRescaledImage);
	Point center = Point(mRescaledImage.cols / 2, mRescaledImage.rows / 2);
	// You can try more different parameters
	Mat rot = getRotationMatrix2D(center, mRotate, 1);
	//Size dsize = Size(mDispCols, mDispRows);
	Size dsize = Size(mRescaledImage.cols, mRescaledImage.rows);
	warpAffine(mRescaledImage, mRescaledImage, rot, dsize, CV_INTER_LINEAR);
	resizeFrame(&mRescaledImage, &mRescaledImage, mScaleX * mZoom, mScaleY * mZoom, mZoomLocus);

}
void superimposeDisplay::trackCallbackScaleX(int val) {
	mTrackValScaleX = val;
	mScaleX = mTrackValScaleX;
	mScaleX /= 50;
	if (mScaleX < 1)
		mScaleX = 0.5f + mScaleX / 2;
	rescaleSuperImage();
	showImg();
}
void superimposeDisplay::trackCallbackScaleY(int val) {
	mTrackValScaleY = val;
	mScaleY = mTrackValScaleY;
	mScaleY /= 50;
	if (mScaleY < 1)
		mScaleY = 0.5f + mScaleY / 2;
	rescaleSuperImage();
	showImg();
}
void superimposeDisplay::trackCallbackRotate(int val) {
	mTrackValRotate = val;
	mRotate = mTrackValRotate - 50;
	mRotate /= 50;
	mRotate *= 180;
	rescaleSuperImage();
	showImg();
}
void superimposeDisplay::trackCallbackZoom(int val) {
	mTrackValZoom = val;
	mZoom = zoomCalc(val);
	rescaleSuperImage();
	showImg();
}
float superimposeDisplay::zoomCalc(int val) {
	float lclZoom = 1;
	if (val < 50) {
		lclZoom = val;
		lclZoom *= 0.8f / 50;
		lclZoom += 0.2f; // 0.2-1
	}
	else {
		lclZoom = val - 50;
		lclZoom /= 50;
		lclZoom *= 4;
		lclZoom += 1; // 1-5
	}
	return lclZoom;
}

int superimposeDisplay::menuSelected(int x, int y, menuMode whichMenu) {
	int retv = MENU_NONE;
	int maxy = (whichMenu == topMenu) ?  MENU_QTY : (whichMenu == reflocs)? MENU_SUB_QTY : (whichMenu == geometries)? MENU_GEO_QTY : MENU_ADJ_QTY;
	int offsety = (whichMenu == topMenu) ? 0  : (whichMenu == reflocs)? MENU_QTY : (whichMenu == geometries)? MENU_QTY + MENU_SUB_QTY : MENU_ADJ_START;
	maxy += 1; // title allowance
	maxy *= MENU_PANEL_LINE_HEIGHT;
	if (x <= MENU_PANEL_WIDTH && y > MENU_PANEL_LINE_HEIGHT && y <= maxy) {
		retv = y / MENU_PANEL_LINE_HEIGHT;
		retv -= 1; // title allowance
		retv += offsety;
	}
	return retv;
}
void superimposeDisplay::showImg() {
	adjustImage(true); // mMenuMode == menuMode::reflocs);
	imshow(mWindowName, mAdjustedImage);
	showMenu(mMenuMode);
}
void superimposeDisplay::hide() {
	destroyWindow(mWindowName);
}

void superimposeDisplay::resizeFrame(Mat* inputMat, Mat* outputMat, float scaleX, float scaleY, Point locus) {
	Mat tmp = inputMat->clone();
	Mat tmpOut = Mat::zeros(mDispRows, mDispCols, inputMat->type());
	Point centre = locus; // Point(locus.x * mDispDivision, locus.y * mDispDivision);
	if (centre.x < 0 || centre.x >= tmp.cols) {
		centre = Point(tmp.cols / 2, centre.y);
	}
	if (centre.y < 0 || centre.y >= tmp.rows) {
		centre = Point(centre.x, tmp.rows / 2);
	}
	mImageLocAtTopLeftOfdisplay = Point(0, 0);
	int minCol = 0;
	int maxCol = (mDispCols * mDispDivision);
	if (scaleX > 1) {
		int sourceCols = (mDispCols * mDispDivision) / scaleX; // image pixels needed to fill the display
		minCol = centre.x - sourceCols / 2;
		if (minCol < 0) minCol = 0;
		maxCol = minCol + sourceCols;
		if (maxCol >= tmp.cols) {
			maxCol = tmp.cols - 1;
			minCol = (maxCol - sourceCols) + 1;
		}
		mImageLocAtTopLeftOfdisplay = Point(minCol, 0);

	}
	int minRow = 0;
	int maxRow = (mDispRows * mDispDivision);
	if(scaleY > 1) {
		int sourceRows = (mDispRows * mDispDivision) / scaleY;
		minRow = centre.y - sourceRows / 2;
		if (minRow < 0) minRow = 0;
		maxRow = minRow + sourceRows;
		if (maxRow >= tmp.rows) {
			maxRow = tmp.rows - 1;
			minRow = (maxRow - sourceRows) + 1;
		}
		mImageLocAtTopLeftOfdisplay = Point(mImageLocAtTopLeftOfdisplay.x, minRow);

	}
	int destCols = mDispCols * scaleX;
	int destRows = mDispRows * scaleY;
	//resize(tmp, tmp, cv::Size(mDispCols * scaleX, mDispRows * scaleY), 0, 0, CV_INTER_LINEAR);
	//resize(tmpOut, tmpOut, cv::Size(mDispCols, mDispRows), 0, 0, CV_INTER_LINEAR);
	if (scaleX > 1) {
		if (scaleY > 1) { // x and y need cropping
			resize(tmp.rowRange(minRow, maxRow).colRange(minCol, maxCol), tmpOut, cv::Size(mDispCols, mDispRows), 0, 0, CV_INTER_LINEAR);
			//tmp.rowRange(0, mDispRows).colRange(0, mDispCols).copyTo(tmpOut);

		}
		else { // x only needs cropping
			resize(tmp.colRange(minCol, maxCol), tmp, cv::Size(mDispCols, destRows), 0, 0, CV_INTER_LINEAR);
			tmp.colRange(0, mDispCols).copyTo(tmpOut.rowRange(0, tmp.rows));

		}
	}
	else {
		if (scaleY > 1) { // y only needs cropping
			resize(tmp.rowRange(minRow, maxRow), tmp, cv::Size(destCols, mDispRows), 0, 0, CV_INTER_LINEAR);
			tmp.rowRange(0, mDispRows).copyTo(tmpOut.colRange(0, tmp.cols));

		}
		else { // X <= 1 && y <= 1
			resize(tmp, tmp, cv::Size(destCols, destRows), 0, 0, CV_INTER_LINEAR);
			tmp.copyTo(tmpOut.rowRange(0, tmp.rows).colRange(0, tmp.cols));

		}

	}

	*outputMat = tmpOut;
}

Point superimposeDisplay::resizePoint(Point2f inputLoc, float scaleX, float scaleY, Point locus) {
	Point2f retv = Point2f(((inputLoc.x - mImageLocAtTopLeftOfdisplay.x) * scaleX) / mDispDivision, ((inputLoc.y - mImageLocAtTopLeftOfdisplay.y) * scaleY) / mDispDivision);
	return retv;
}
Point superimposeDisplay::resizePoint(Point2f inputLoc, float imageDivision, float scaleX, float scaleY, Point locus) {
	return resizePoint(inputLoc * imageDivision, scaleX, scaleY, locus);
}
Point superimposeDisplay::unResizePoint(Point2f inputLoc, float scaleX, float scaleY, Point locus) {
	Point2f retv = Point2f(((inputLoc.x * mDispDivision) /scaleX) + mImageLocAtTopLeftOfdisplay.x, ((inputLoc.y * mDispDivision) / scaleY ) + mImageLocAtTopLeftOfdisplay.y);
	return retv;
}

//void superimposeDisplay::resizeFrame(Mat* inputMat, Mat* outputMat, float scaleX, float scaleY) {
//	Mat tmp = inputMat->clone();
//	Mat tmpOut = Mat::zeros(mDispRows, mDispCols, inputMat->type());
//	resize(tmp, tmp, cv::Size(mDispCols * scaleX, mDispRows * scaleY), 0, 0, CV_INTER_LINEAR);
//	//resize(tmpOut, tmpOut, cv::Size(mDispCols, mDispRows), 0, 0, CV_INTER_LINEAR);
//	if (scaleX > 1) {
//		if (scaleY > 1) { // x and y need cropping
//			tmp.rowRange(0, mDispRows).colRange(0, mDispCols).copyTo(tmpOut);
//
//		}
//		else { // x only needs cropping
//			tmp.colRange(0, mDispCols).copyTo(tmpOut.rowRange(0, tmp.rows));
//
//		}
//	}
//	else {
//		if (scaleY > 1) { // y only needs cropping
//			tmp.rowRange(0, mDispRows).copyTo(tmpOut.colRange(0, tmp.cols));
//
//		}
//		else { // X <= 1 && y <= 1
//			tmp.copyTo(tmpOut.rowRange(0, tmp.rows).colRange(0, tmp.cols));
//
//		}
//
//	}
//
//	*outputMat = tmpOut;
//}

void superimposeDisplay::adjustImage(bool bDrawCrosses) {
	Mat tmpSTaticImage;
	mBaseImage.copyTo(tmpSTaticImage);
	resizeFrame(&tmpSTaticImage, &tmpSTaticImage, mZoom, mZoom, mZoomLocus);
	mRescaledImage.copyTo(mAdjustedImage);
	if (bDrawCrosses) {
		drawCrosses(tmpSTaticImage, &mPerspectiveStatic, true);
		drawCrosses(mAdjustedImage, &mPerspectiveMobile, false);
		//drawGeometries(mAdjustedImage, mZoom, mZoom, mZoomLocus);
	}
	if (mApplyDistortion && mHaveAllStaticLocs && mHaveAllMobileLocs) {
		perspectiveImageReference rescaledMobile;
		perspectiveImageReference rescaledStatic;
		rescaledMobile.clone(mPerspectiveMobile);
		rescaledStatic.clone(mPerspectiveStatic);
		rescaledMobile.orgTL = resizePoint(rescaledMobile.orgTL, mZoom, mZoom, mZoomLocus);
		rescaledMobile.orgTR = resizePoint(rescaledMobile.orgTR, mZoom, mZoom, mZoomLocus);
		rescaledMobile.orgBL = resizePoint(rescaledMobile.orgBL, mZoom, mZoom, mZoomLocus);
		rescaledMobile.orgBR = resizePoint(rescaledMobile.orgBR, mZoom, mZoom, mZoomLocus);
		rescaledStatic.orgTL = resizePoint(rescaledStatic.orgTL, mZoom, mZoom, mZoomLocus);
		rescaledStatic.orgTR = resizePoint(rescaledStatic.orgTR, mZoom, mZoom, mZoomLocus);
		rescaledStatic.orgBL = resizePoint(rescaledStatic.orgBL, mZoom, mZoom, mZoomLocus);
		rescaledStatic.orgBR = resizePoint(rescaledStatic.orgBR, mZoom, mZoom, mZoomLocus);
		mPersp.calcPerspectiveLambda(rescaledMobile, rescaledStatic);
		mPersp.warp(mAdjustedImage, mAdjustedImage, true);
	}
	float contrast = mDispContrastTrackVal;
	contrast /= 100;
	//drawCells(tmpSTaticImage, mZoom, mZoom, mZoomLocus, 0);
	//drawCells(mAdjustedImage, mZoom, mZoom, mZoomLocus, 1);

	mAdjustedImage = tmpSTaticImage * contrast + translateImg(mAdjustedImage, mOffsetX, mOffsetY) * (1 - contrast);
	drawCells(mAdjustedImage, mZoom, mZoom, mZoomLocus, (contrast < 0.5f) ? 1 : 0);
	drawGeometries(mAdjustedImage, mZoom, mZoom, mZoomLocus, contrast < 0.5f); // only draw the comb significant stuff if the mobile image dominates
	drawScalePoints(mAdjustedImage, mZoom, mZoom, mZoomLocus);
	//if (mTrackVal > 50) {
	//	contrast -= 50;
	//	contrast /= 50;
	//	mAdjustedImage = tmpSTaticImage * contrast + translateImg(mAdjustedImage, mOffsetX, mOffsetY) * (1 - contrast);
	//}
	//else {
	//	contrast /= 50;
	//	mAdjustedImage = tmpSTaticImage * contrast  - translateImg(mAdjustedImage, mOffsetX, mOffsetY) * (1 - contrast);
	//}
}
Mat superimposeDisplay::translateImg(Mat &img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	Mat retv;
	warpAffine(img, retv, trans_mat, img.size());
	return retv;
}
void superimposeDisplay::drawCross(Mat image, Point whereOnImage, char *name, bool plusNotTimes) {
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
void superimposeDisplay::drawCrosses(Mat image, perspectiveImageReference *relevantPerspective, bool plusNotTimes) {
	drawCross(image, resizePoint(relevantPerspective->orgTL, mZoom, mZoom, mZoomLocus), "ref 1", plusNotTimes);
	drawCross(image, resizePoint(relevantPerspective->orgTR, mZoom, mZoom, mZoomLocus), "ref 2", plusNotTimes);
	drawCross(image, resizePoint(relevantPerspective->orgBR, mZoom, mZoom, mZoomLocus), "ref 3", plusNotTimes);
	drawCross(image, resizePoint(relevantPerspective->orgBL, mZoom, mZoom, mZoomLocus), "ref 4", plusNotTimes);
//	drawCross(image, resizePoint((relevantPerspective->orgTL + relevantPerspective->orgTR + relevantPerspective->orgBR + relevantPerspective->orgBL) / 4, mZoom, mZoom, mZoomLocus), "CoG", plusNotTimes);
}
void superimposeDisplay::setGeoTypesShown(bool *setValsNative, bool *setValsRandom, int arrySize) {
	for (int idx = 0; idx < arrySize; idx++) {
		showGeoTypes[idx] = *setValsNative++;
		showGeoTypesR[idx] = *setValsRandom++;
	}
}
void superimposeDisplay::drawGeometries(Mat image, float scaleX, float scaleY, Point locus, bool drawCombFeatures) {
	for (int idx = 0; idx < mGeometryDescriptions.size(); idx++) {
		if ((showGeoTypes[(int)(mGeometryDescriptions[idx]->mShape)] && !mGeometryDescriptions[idx]->mRandom )|| (showGeoTypesR[(int)(mGeometryDescriptions[idx]->mShape)] && mGeometryDescriptions[idx]->mRandom)) {
			bool highlight = (idx == (mGeometryDescriptions.size() - 1)) || (idx == mGeoBeingEdited);
			drawGeometry(image, mGeometryDescriptions[idx], scaleX, scaleY, locus, highlight, drawCombFeatures);

		}
	}
}

void superimposeDisplay::drawCells(Mat image, float scaleX, float scaleY, Point locus, int imageIdx) {
	Scalar PtClr = Scalar(255, 255, 255);
	Scalar addCellClr = Scalar(0,0, 255);
	Scalar ConnClr = Scalar(255, 0, 0);
	Scalar addConnClr = Scalar( 0, 0, 255);
	bool applyPerspective = mApplyDistortion && (imageIdx == 1);
	for (int idxC = 0; idxC < autoCellCentres[imageIdx].size(); idxC++) {
		bool excluded = false;
		for (int idxEx = 0; idxEx < minusCellCentres[imageIdx].size(); idxEx++) {
			if (idxC == minusCellCentres[imageIdx][idxEx]) {
				excluded = true;
				break;
			}
		}
		if (!excluded)
			circle(image, mPersp.adjustedPerspective(resizePoint(autoCellCentres[imageIdx][idxC], scaleX, scaleY, locus), false, applyPerspective), 3, PtClr, -1);

	}
	for (int idxC = 0; idxC < addCellCentres[imageIdx].size(); idxC++) {
		circle(image, mPersp.adjustedPerspective(resizePoint(addCellCentres[imageIdx][idxC], scaleX, scaleY, locus), false, applyPerspective), 3, addCellClr, -1);

	}
	for (int idxC = 0; idxC < autoCellConnections[imageIdx].size(); idxC++) {
		bool excluded = false;
		for (int idxEx = 0; idxEx < minusCellCentres[imageIdx].size(); idxEx++) {
			if (idxC == minusCellCentres[imageIdx][idxEx]) {
				excluded = true;
				break;
			}
		}
		if (!excluded) {
			for (int idxConn = 0; idxConn < autoCellConnections[imageIdx][idxC].size(); idxConn++) {
				bool excludedConn = false;
				int idxC2 = (autoCellConnections[imageIdx][idxC])[idxConn];
				for (int idxEx = 0; idxEx < minusCellConnections[imageIdx].size(); idxEx++) {
					if ((idxC == minusCellConnections[imageIdx][idxEx].x && idxC2 == minusCellConnections[imageIdx][idxEx].y) ||
						(idxC == minusCellConnections[imageIdx][idxEx].y && idxC2 == minusCellConnections[imageIdx][idxEx].x) ){
						excludedConn = true;
						break;
					}
				}
				if (!excludedConn)
					line(image, mPersp.adjustedPerspective(resizePoint(autoCellCentres[imageIdx][idxC], scaleX, scaleY, locus), false, applyPerspective),
						mPersp.adjustedPerspective(resizePoint(autoCellCentres[imageIdx][idxC2], scaleX, scaleY, locus), false, applyPerspective), ConnClr, 1);
			}
		}
	}
	for (int idxC = 0; idxC < addCellConnections[imageIdx].size(); idxC++) {
		int cell0 = addCellConnections[imageIdx][idxC].x;
		int cell1 = addCellConnections[imageIdx][idxC].y;
		Point2f loc0;
		Point2f loc1;
		if (cell0 < 0) {
			loc0 = addCellCentres[imageIdx][Math::Abs(cell0) -1];
		}
		else {
			loc0 = autoCellCentres[imageIdx][cell0];
		}
		if (cell1 < 0) {
			loc1 = addCellCentres[imageIdx][Math::Abs(cell1) - 1];
		}
		else {
			loc1 = autoCellCentres[imageIdx][cell1];
		}
		line(image, mPersp.adjustedPerspective(resizePoint(loc0, scaleX, scaleY, locus), false, applyPerspective),
			mPersp.adjustedPerspective(resizePoint(loc1, scaleX, scaleY, locus), false, applyPerspective), addConnClr, 1);

	}
	recreateCells(0);
	recreateCells(1); 
	mCellsNeedUpdate = false; //recrete both sets if either is necessary before the need is cleared

	drawSelectedCells( image,  scaleX,  scaleY,  locus, ((mMenuSelected[MENU_GEO_CELLCENTRE] && mMenuSelected[MENU_GEO_CELLCONN]) || mMenuSelected[MENU_ADJ_SHOWCELLS]), imageIdx);
	if (showCell >= 0) {
		char cellID[48];
		sprintf(cellID, "%d %f", allCells[imageIdx][showCell].mID, allCells[imageIdx][showCell].getAverageWallThickness());
		putText(image, cellID, resizePoint(allCells[imageIdx][showCell].mCentre, scaleX, scaleY, locus), FONT_HERSHEY_SIMPLEX, 0.5, PtClr);
		circle(image, resizePoint(allCells[imageIdx][showCell].mCentre, scaleX, scaleY, locus), 3, ConnClr, -1);
		for (int idxC = 0; idxC < allCells[imageIdx][showCell].mConnections.size(); idxC++) {
			circle(image, resizePoint(allCells[imageIdx][showCell].mConnections[idxC], scaleX, scaleY, locus), 2, ConnClr, -1);
		}
		for (int idxC = 0; idxC < allCells[imageIdx][showCell].mContours.size(); idxC++) {
			circle(image, resizePoint(allCells[imageIdx][showCell].mContours[idxC], scaleX, scaleY, locus), 1, ConnClr, -1);
		}
		for (int idxC = 0; idxC < allCells[imageIdx][showCell].mWallCentres.size(); idxC++) {
			circle(image, resizePoint(allCells[imageIdx][showCell].mWallCentres[idxC], scaleX, scaleY, locus), 4, PtClr, 1);
		}
		int qtyCorners = allCells[imageIdx][showCell].mCorners.size();
		for (int idxC = 0; idxC < qtyCorners; idxC++) {
			int idxPlus1 = (idxC + 1) % qtyCorners;

			line(image, resizePoint(allCells[imageIdx][showCell].mCorners[idxC], scaleX, scaleY, locus), resizePoint(allCells[imageIdx][showCell].mCorners[idxPlus1], scaleX, scaleY, locus), PtClr, 1);
		}

	}
}
void superimposeDisplay::calcSelectedBoundary(Point2f &b00, Point2f &b01, Point2f &b10, Point2f &b11) {
	float boxRangePixels = CELL_SAMPLE_RANGE_MM * pixelsPerMM();
	Point2f cellBoundaryVect = mCellBoundary1 - mCellBoundary0;
	cellBoundaryVect /= magnitude(cellBoundaryVect);
	Point2f cellBoundaryOthogonalVect = Point2f(-cellBoundaryVect.y, cellBoundaryVect.x);
	b00 = mCellBoundary0 - cellBoundaryOthogonalVect * boxRangePixels;
	b01 = mCellBoundary0 + cellBoundaryOthogonalVect * boxRangePixels;
	b10 = mCellBoundary1 - cellBoundaryOthogonalVect * boxRangePixels;
	b11 = mCellBoundary1 + cellBoundaryOthogonalVect * boxRangePixels;

}

void superimposeDisplay::drawSelectedCells(Mat image, float scaleX, float scaleY, Point locus, bool showAll, int imageIdx) {

	Scalar addCellClr = Scalar(0, 0, 255);
	Scalar PtClr = Scalar(255, 255, 255);
	Scalar ConnClr = Scalar(255, 0, 0);
	Scalar boundaryClr = Scalar(0, 0, 255);
	Point2f cellBoundaryCorner00 ;
	Point2f cellBoundaryCorner01 ;
	Point2f cellBoundaryCorner10 ;
	Point2f cellBoundaryCorner11 ;
	calcSelectedBoundary(cellBoundaryCorner00, cellBoundaryCorner01, cellBoundaryCorner10, cellBoundaryCorner11);
	circle(image, resizePoint(mCellBoundary0, scaleX, scaleY, locus), 4, PtClr, -1);
	line(image, resizePoint(mCellBoundary0, scaleX, scaleY, locus), resizePoint(mCellBoundary1, scaleX, scaleY, locus), PtClr, 2);
	line(image, resizePoint(cellBoundaryCorner00, scaleX, scaleY, locus), resizePoint(cellBoundaryCorner01, scaleX, scaleY, locus), boundaryClr, 2);
	line(image, resizePoint(cellBoundaryCorner01, scaleX, scaleY, locus), resizePoint(cellBoundaryCorner11, scaleX, scaleY, locus), boundaryClr, 2);
	line(image, resizePoint(cellBoundaryCorner10, scaleX, scaleY, locus), resizePoint(cellBoundaryCorner11, scaleX, scaleY, locus), boundaryClr, 2);
	line(image, resizePoint(cellBoundaryCorner00, scaleX, scaleY, locus), resizePoint(cellBoundaryCorner10, scaleX, scaleY, locus), boundaryClr, 2);
	putText(image, mCellBoundaryType.substr(0,1), resizePoint((mCellBoundary0 + cellBoundaryCorner00) / 2, scaleX, scaleY, locus), FONT_HERSHEY_SIMPLEX, 0.5, PtClr);
	putText(image, mCellBoundaryType.substr(1,1), resizePoint((mCellBoundary0 + cellBoundaryCorner01) / 2, scaleX, scaleY, locus), FONT_HERSHEY_SIMPLEX, 0.5, PtClr);
	selectCellsWithinBoundary(cellBoundaryCorner00, cellBoundaryCorner01, cellBoundaryCorner10, cellBoundaryCorner11, showAll, imageIdx);
	for (int idx = 0; idx < selectedCells[imageIdx].size(); idx++) {
		cvCell *oneCell = &(allCells[imageIdx][selectedCells[imageIdx][idx]]);
		Scalar cellClr = (oneCell->mID >= 0) ? ConnClr : addCellClr;
		circle(image, resizePoint(oneCell->mCentre, scaleX, scaleY, locus), 3, cellClr, -1);
		//for (int idxC = 0; idxC < cells[idx].mConnections.size(); idxC++) {
		//	circle(image, resizePoint(cells[idx].mConnections[idxC], scaleX, scaleY, locus), 2, ConnClr, -1);
		//}
		//for (int idxC = 0; idxC < cells[idx].mContours.size(); idxC++) {
		//	circle(image, resizePoint(cells[idx].mContours[idxC], scaleX, scaleY, locus), 1, ConnClr, -1);
		//}
		//for (int idxC = 0; idxC < cells[idx].mWallCentres.size(); idxC++) {
		//	circle(image, resizePoint(cells[idx].mWallCentres[idxC], scaleX, scaleY, locus), 4, PtClr, 1);
		//}
		int qtyCorners = oneCell->mCorners.size();
		for (int idxC = 0; idxC < qtyCorners; idxC++) {
			int idxPlus1 = (idxC + 1) % qtyCorners;

			line(image, resizePoint(oneCell->mCorners[idxC], scaleX, scaleY, locus), resizePoint(oneCell->mCorners[idxPlus1], scaleX, scaleY, locus), PtClr, 1);
		}
	}

}
void superimposeDisplay::clearStoredWallCentres(int imageID, int cellID)
{
	persistantCellData[imageID].clearWallCentres(cellID);
	mCellsNeedUpdate = true;
}
void superimposeDisplay::updateCellWall(Point2f loc, int imageID, int cellID, int peerID) {
	persistantCellData[imageID].setWallCentre(cellID, peerID, loc, cvCellPoint::flexability::fixed);
	mCellsNeedUpdate = true;
}
superimposeDisplay *pGetWallCentreSupportObj = nullptr;
bool gGetWallCentre(int imageID, int cellID0, int cellID1, Point2f *pPoint) {
	return pGetWallCentreSupportObj->getWallCentre(imageID, cellID0, cellID1, pPoint);
}
bool superimposeDisplay::getWallCentre(int imageID, int cellID0, int cellID1, Point2f *pPoint) {
	bool retv = true;
	Point2f tmpPoint;
	cvCellPoint::flexability currentFlex = persistantCellData[imageID].getWallCentre(cellID0, cellID1, &tmpPoint);
	if (cvCellPoint::flexability::fixed == currentFlex) {
		*pPoint = tmpPoint;
		retv = true;
	} else {
		Point2f c0;
		Point2f c1;
		int possitivedID = cellID0;
		if (cellID0 < 0) {
			possitivedID = -(cellID0 + 1);
			c0 = addCellCentres[imageID][possitivedID];
		}
		else {
			c0 = autoCellCentres[imageID][possitivedID];
		}
		possitivedID = cellID1;
		if (cellID1 < 0) {
			possitivedID = -(cellID1 + 1);
			c1 = addCellCentres[imageID][possitivedID];
		}
		else {
			c1 = autoCellCentres[imageID][possitivedID];
		}
		float wallPoint0, wallPoint1;
		if (mAdjustableDisplay!= nullptr && mAdjustableDisplay->getWallIntersection(c0, c1, &wallPoint0, &wallPoint1)) {
			// replace with a supplied point
			*pPoint = c0 + wallPoint0 * (c1 - c0);
			persistantCellData[imageID].setWallCentre(cellID0, cellID1, *pPoint, cvCellPoint::flexability::supplied);
		}
		else if (cvCellPoint::flexability::supplied != currentFlex) {
			// only recalculate if the current point wasnt supplied
			persistantCellData[imageID].setWallCentre(cellID0, cellID1, *pPoint, cvCellPoint::flexability::calc);
		}
		else {
			*pPoint = tmpPoint;

		}
	}
	return retv;
}

void gSetWallCentre(int imageID, int cellID0, int cellID1, Point2f point) {
	pGetWallCentreSupportObj->setWallCentre(imageID, cellID0, cellID1, point, cvCellPoint::flexability::calc);
}
void superimposeDisplay::setWallCentre(int imageID, int cellID0, int cellID1, Point2f point, cvCellPoint::flexability flex) {
	persistantCellData[imageID].setWallCentre(cellID0, cellID1, point, flex);
}



void superimposeDisplay::recreateCells(int imageIdx) {
	pGetWallCentreSupportObj = this;
	if (mCellsNeedUpdate) {
		allCells[imageIdx].clear();
		for (int idxC = 0; idxC < autoCellCentres[imageIdx].size(); idxC++) {
			if (cvCell::cellHasConnections(idxC, autoCellConnections[imageIdx], addCellConnections[imageIdx], minusCellConnections[imageIdx])) {
				cvCell *tmpCell = new cvCell(imageIdx, &allCells[imageIdx], autoCellContours[imageIdx][idxC], idxC, autoCellCentres[imageIdx], autoCellConnections[imageIdx], addCellCentres[imageIdx], addCellConnections[imageIdx], minusCellCentres[imageIdx], minusCellConnections[imageIdx], gGetWallCentre );

//				cvCell *tmpCell = new cvCell(&allCells[imageIdx], autoCellContours[imageIdx][idxC], idxC, autoCellCentres[imageIdx], autoCellConnections[imageIdx], addCellCentres[imageIdx], addCellConnections[imageIdx], minusCellCentres[imageIdx], minusCellConnections[imageIdx], mAdjustableDisplay);
				allCells[imageIdx].push_back(*tmpCell);
			}
		}
		for (int idxC = 0; idxC < addCellCentres[imageIdx].size(); idxC++) {
			int negativisedID = -(idxC + 1);
			if (cvCell::cellHasConnections(negativisedID, autoCellConnections[imageIdx], addCellConnections[imageIdx], minusCellConnections[imageIdx])) {
				cvCell *tmpCell = new cvCell(imageIdx, &allCells[imageIdx], negativisedID, autoCellCentres[imageIdx], addCellCentres[imageIdx], addCellConnections[imageIdx], gGetWallCentre);
//				cvCell *tmpCell = new cvCell(&allCells[imageIdx], negativisedID, autoCellCentres[imageIdx], addCellCentres[imageIdx], addCellConnections[imageIdx], mAdjustableDisplay);
				allCells[imageIdx].push_back(*tmpCell);
			}
		}
		for (int idxC = 0; idxC < allCells[imageIdx].size(); idxC++) {
			allCells[imageIdx][idxC].reCalcWithPeerDependencies(imageIdx, 0, gSetWallCentre);
		}
		for (int idxC = 0; idxC < allCells[imageIdx].size(); idxC++) {
			allCells[imageIdx][idxC].reCalcWithPeerDependencies(imageIdx, 1, gSetWallCentre);
		}
		if (imageIdx == 1 && mApplyDistortion) {
			for (int idxC = 0; idxC < allCells[imageIdx].size(); idxC++) {
				Point2f tmpPoint = resizePoint(allCells[imageIdx][idxC].mCentre, mZoom, mZoom, mZoomLocus);
				allCells[imageIdx][idxC].mCentre = unResizePoint( mPersp.adjustedPerspective(tmpPoint, false, true), mZoom, mZoom, mZoomLocus);
				for (int Cidx = 0; Cidx < allCells[imageIdx][idxC].mCorners.size(); Cidx++) {
					tmpPoint = resizePoint(allCells[imageIdx][idxC].mCorners[Cidx], mZoom, mZoom, mZoomLocus);
					allCells[imageIdx][idxC].mCorners[Cidx] = unResizePoint(mPersp.adjustedPerspective(tmpPoint, false, true), mZoom, mZoom, mZoomLocus);

				}
				for (int Cidx = 0; Cidx < allCells[imageIdx][idxC].mWallCentres.size(); Cidx++) {
					tmpPoint = resizePoint(allCells[imageIdx][idxC].mWallCentres[Cidx], mZoom, mZoom, mZoomLocus);
					allCells[imageIdx][idxC].mWallCentres[Cidx] = unResizePoint(mPersp.adjustedPerspective(tmpPoint, false, true), mZoom, mZoom, mZoomLocus);

				}
			}

		}

	}

}

void superimposeDisplay::selectCellsWithinBoundary(Point2f b00, Point2f b01, Point2f b10, Point2f b11, bool selectAll, int imageIdx) {
	selectedCells[imageIdx].clear();
	for (int idxC = 0; idxC < allCells[imageIdx].size(); idxC++) {
		if (selectAll || (isWithin(b00, b01, b10, allCells[imageIdx][idxC].mCentre) && isWithin(b11, b10, b01, allCells[imageIdx][idxC].mCentre))) {
			selectedCells[imageIdx].push_back(idxC);
		}
	}
}


void superimposeDisplay::drawScalePoints(Mat image, float scaleX, float scaleY, Point locus) {
	Scalar cellClr = Scalar(255, 255, 255);
	drawScalePoint(image, mScalePt0, scaleX, scaleY, locus);
	drawScalePoint(image, mScalePt1, scaleX, scaleY, locus);
	for (int idx = 0; idx < mConfig.cellScaleRefs.arraySize; idx++) {
		line(image, resizePoint(mConfig.cellScaleRefs.sampleCells0[idx],scaleX, scaleY, locus), resizePoint(mConfig.cellScaleRefs.sampleCells1[idx],scaleX, scaleY, locus) , cellClr, 1);
		drawCircleFromDiameter(image, scaleX, scaleY, locus, mConfig.cellScaleRefs.sampleCells0[idx], mConfig.cellScaleRefs.sampleCells1[idx], cellClr, 1);
		line(image, resizePoint(mConfig.cellScaleRefs.sampleCellWalls0[idx],scaleX, scaleY, locus), resizePoint(mConfig.cellScaleRefs.sampleCellWalls1[idx],scaleX, scaleY, locus) , cellClr, 1);
		//drawCircleFromDiameter(image, scaleX, scaleY, locus, mConfig.cellScaleRefs.sampleCellWalls0[idx], mConfig.cellScaleRefs.sampleCellWalls1[idx], cellClr, 1);
	}
}
void longLine(Mat image, Point pt0, Point pt1, Scalar clr, int sz) {
	int overshootX = pt1.x - pt0.x;
	int overshootY = pt1.y - pt0.y;
	line(image, Point(pt0.x - overshootX, pt0.y - overshootY), Point(pt1.x + overshootX, pt1.y + overshootY), clr, sz);
}
void superimposeDisplay::drawScalePoint(Mat image, Point2f pt, float scaleX, float scaleY, Point locus) {
	Scalar PtClr = Scalar(255, 255, 255);
	circle(image, resizePoint(pt, scaleX, scaleY, locus), 10, PtClr, 1);
	circle(image, resizePoint(pt, scaleX, scaleY, locus), 2, PtClr, -1);

}
//float magnitude(Point2f pt) {
//	return Math::Sqrt(pt.x * pt.x + pt.y * pt.y);
//}
void superimposeDisplay::drawCircleFromDiameter(Mat image, float scaleX, float scaleY, Point locus, Point dia0, Point dia1, Scalar wallClr, int weight) {
	Point2f mDia0 = resizePoint(dia0, scaleX, scaleY, locus);
	Point2f mDia1 = resizePoint(dia1, scaleX, scaleY, locus);
	Point circCentre = (mDia0 + mDia1) / 2;
	float rad = magnitude(mDia0 - mDia1) / 2;
	circle(image, circCentre, rad, wallClr, weight);
}
void superimposeDisplay::drawGeometry(Mat image, geometryDesc *geoDesc, float scaleX, float scaleY, Point locus, bool highlight, bool drawCombFeatures) {
	Scalar bulgeClr = Scalar(0, 0, 0);
	Scalar wallClr = Scalar(255, 0, 0);
	Scalar PitClr = Scalar(0, 255, 0);
	Scalar wallClr2 = Scalar(255, 127, 0);
	Scalar PitClr2 = Scalar(170, 200, 0);
	Scalar PitLineClr = Scalar(0, 0, 255);
	Scalar builtWallClr = Scalar(255, 255, 255);
	int pitSize = 7 * mZoom;
	int dotScale = highlight ? 2 : 1;
	if (geoDesc->mShape == geometryDesc::rim1pit || geoDesc->mShape == geometryDesc::rim2pits ) {
		if (!geoDesc->isEmpty(geoDesc->pit0)) {
			circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale * 4, wallClr, (geoDesc->mRandom)?-1 : 1);
			circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), pitSize, wallClr, 1);
		}
		if (!geoDesc->isEmpty(geoDesc->pit1)) {
			circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), dotScale * 4, wallClr, 1);
			circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), pitSize, wallClr, 1);
		}
	} else if (geoDesc->mShape == geometryDesc::pitcell1 || geoDesc->mShape == geometryDesc::pitcell2 ) {
		if (!geoDesc->isEmpty(geoDesc->pit0)) {
			if (!geoDesc->isEmpty(geoDesc->wallEnd0)) {
				drawCircleFromDiameter(image, scaleX, scaleY, locus, geoDesc->pit0, geoDesc->wallEnd0, PitClr, 1);
			}
			else {
				circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale * 2, PitClr, 1);
			}
		}
		if (!geoDesc->isEmpty(geoDesc->pit1)) {
			if (!geoDesc->isEmpty(geoDesc->wallEnd1)) {
				drawCircleFromDiameter(image, scaleX, scaleY, locus, geoDesc->pit1, geoDesc->wallEnd1, PitClr2, 1);
			}
			else {
				circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), dotScale * 2, PitClr2, 1);
			}
		}
	} else if (geoDesc->mShape == geometryDesc::zigzag ) {
		drawCombFeatures = false;
		/*
			Point2f wallEnd0;
	Point2f wallEnd1;
	Point2f bentCentre;
	Point2f pit0; //  wallend may have only one of these set
	Point2f pit1;
	bool pit1Used;
	Point2f pit2;
	bool pit2Used;
	Point2f builtWall00;
	Point2f builtWall01;
	Point2f builtWall10; // only used for wallend second wall of fork
	Point2f builtWall11;

		*/
		if (!geoDesc->isEmpty(geoDesc->wallEnd0)) {
			circle(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, (geoDesc->mRandom) ? -1 : 1);
			if (!geoDesc->isEmpty(geoDesc->wallEnd1)) {
				line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
				circle(image, resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
				if (!geoDesc->isEmpty(geoDesc->bentCentre)) {
					circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, (geoDesc->mRandom) ? -1 : 1);
					line(image, resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), wallClr, 1);
					if (!geoDesc->isEmpty(geoDesc->pit0)) {
						circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
						line(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), resizePoint(geoDesc->pit0, scaleX, scaleY, locus), wallClr, 1);
						if (!geoDesc->isEmpty(geoDesc->pit1)) {
							circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
							line(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), resizePoint(geoDesc->pit1, scaleX, scaleY, locus), wallClr, 1);
							if (!geoDesc->isEmpty(geoDesc->pit2)) {
								circle(image, resizePoint(geoDesc->pit2, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
								line(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), resizePoint(geoDesc->pit2, scaleX, scaleY, locus), wallClr, 1);
								if (!geoDesc->isEmpty(geoDesc->builtWall00)) {
									circle(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
									line(image, resizePoint(geoDesc->pit2, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), wallClr, 1);
									if (!geoDesc->isEmpty(geoDesc->builtWall01)) {
										circle(image, resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
										line(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), wallClr, 1);
										if (!geoDesc->isEmpty(geoDesc->builtWall10)) {
											circle(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
											line(image, resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), wallClr, 1);
											if (!geoDesc->isEmpty(geoDesc->builtWall11)) {
												circle(image, resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), TRANSP_SAMPLE_RADIUS_PIXELS  * scaleX / mDispDivision, PitClr, 1);
												line(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), wallClr, 1);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (geoDesc->mShape == geometryDesc::curve ) {
		//circle(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), dotScale, wallClr, 1);
		//circle(image, resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), dotScale, wallClr, 1);
		//circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), dotScale, wallClr, 1);
		//circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale, wallClr, 1);
		//circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), dotScale, wallClr, 1);
		//if (!geoDesc->isEmpty(geoDesc->pit2)) {
		//	circle(image, resizePoint(geoDesc->pit2, scaleX, scaleY, locus), dotScale, wallClr, 1);
		//	Point2f WallCentre0 = (geoDesc->wallEnd0 + geoDesc->wallEnd1) / 2;
		//	Point2f WallCentreMid = (geoDesc->bentCentre + geoDesc->pit0) / 2;
		//	Point2f WallCentre1 = (geoDesc->pit1 + geoDesc->pit2) / 2;
		//	line(image, resizePoint(WallCentre0, scaleX, scaleY, locus), resizePoint(WallCentreMid, scaleX, scaleY, locus), wallClr, 1);
		//	line(image, resizePoint(WallCentre1, scaleX, scaleY, locus), resizePoint(WallCentreMid, scaleX, scaleY, locus), wallClr, 1);
		//}
		//else {
		//	Point2f WallCentre0 = (geoDesc->wallEnd0 + geoDesc->wallEnd1) / 2;
		//	Point2f WallCentreMid = (geoDesc->bentCentre + geoDesc->pit0) / 2;
		//	Point2f WallCentre1 = (geoDesc->pit1 + geoDesc->pit2) / 2;
		//	line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
		//	line(image, resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), wallClr, 1);
		//	line(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), resizePoint(geoDesc->pit0, scaleX, scaleY, locus), wallClr, 1);
		//	line(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), resizePoint(geoDesc->pit1, scaleX, scaleY, locus), wallClr, 1);
		//}

		/*
#define MENU_GEO_CURVEENDSTART       wallEnd0;
#define MENU_GEO_CURVEENDEND MENU_GEO_WALLEND1 wallEnd1
#define MENU_GEO_CURVE_THIRD_OR_HALF MENU_GEO_WALLENTRE bentCentre
#define MENU_GEO_CURVE_2THIRDS MENU_GEO_PIT0
#define MENU_GEO_CURVE_2THIRDS_SKIP MENU_GEO_PIT1 pit1
#define MENU_GEO_CURVE_THIRD_WALL MENU_GEO_BUILTWALL00 builtWall00
#define MENU_GEO_CURVE_HALF_WALL MENU_GEO_BUILTWALL01 builtWall01
#define MENU_GEO_CURVE_HALF_STOP MENU_GEO_BUILTWALL10
#define MENU_GEO_CURVE_2THIRDS_WALL MENU_GEO_BUILTWALL11 builtWall11
#define MENU_GEO_CURVE_STOP MENU_GEO_DONE
*/
		circle(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), dotScale, wallClr, 1);
		circle(image, resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), dotScale, wallClr, 1);
		if (!geoDesc->isEmpty(geoDesc->bentCentre)) {
			bool bentCentreIsThird = false;
			circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), dotScale, wallClr, 1);
			// mid or 1/3 is present
			Point2f oneThird = (geoDesc->wallEnd0 + geoDesc->wallEnd0 + geoDesc->wallEnd1) / 3;
			Point2f oneHalf = (geoDesc->wallEnd0 + geoDesc->wallEnd1) / 2;
			float dist1thrid = magnitude(oneThird - geoDesc->bentCentre);
			float disthalf = magnitude(oneHalf - geoDesc->bentCentre);
			circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale, wallClr, 1);
			if (disthalf > dist1thrid) {
				bentCentreIsThird = true;
			}
			if (!geoDesc->isEmpty(geoDesc->pit0)) {
				// so is 2/3rds
				line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), wallClr, 1);
				circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), dotScale, wallClr, 1);
				line(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), resizePoint(geoDesc->pit0, scaleX, scaleY, locus), wallClr, 1);
				line(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
				circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale, wallClr, 1);
				if (!geoDesc->isEmpty(geoDesc->builtWall00)) {
					// 1/3rd wall is present
					circle(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), dotScale, PitClr, 1);
				}
				if (!geoDesc->isEmpty(geoDesc->builtWall01)) {
					// 1/2 wall is present
					circle(image, resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), dotScale, PitClr, 1);
				}
				if (!geoDesc->isEmpty(geoDesc->builtWall11)) {
					// 2/3rd wall is present
					circle(image, resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), dotScale, PitClr, 1);
				}
			}
			else {
				// only 1/2 or 1/3
				if (bentCentreIsThird && geoDesc->isEmpty(geoDesc->builtWall01)) {
					//line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), wallClr, 1);
					//line(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
					// end points and 1/3rd point, so drawn base line and 2nd third 
					line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
//						circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), dotScale, wallClr, 1);
					Point2f twoThirds = (geoDesc->wallEnd0 + geoDesc->wallEnd1 + geoDesc->wallEnd1) / 3;
					circle(image, resizePoint(twoThirds, scaleX, scaleY, locus), dotScale, wallClr, 1);
				}
				else {
					// end points and mid bend...so draw cranked line
					line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), wallClr, 1);
					line(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
					if (!geoDesc->isEmpty(geoDesc->builtWall01)) {
						// end points and mid bend with 1st built point...so add built point(s)
						circle(image, resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), dotScale, PitClr, 1);
						if (!geoDesc->isEmpty(geoDesc->builtWall11)) {
							circle(image, resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), dotScale, PitClr, 1);
						}
					}
					else {
						// end points and mid bend...so draw cranked line with halfway points on each

					}
				}
			}
		}
		else {
			// so far, base lines only...but has mid wax wall been defined?
			line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
			if (!geoDesc->isEmpty(geoDesc->builtWall01)) {
				// end points and  built point
				circle(image, resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), dotScale, PitClr, 1);
			}
			else {
				// no, so this is th ebase line for all options
				Point2f oneThird = (geoDesc->wallEnd0 + geoDesc->wallEnd0 + geoDesc->wallEnd1) / 3;
				Point2f oneHalf = (geoDesc->wallEnd0 + geoDesc->wallEnd1) / 2;
				Point2f twoThirds = (geoDesc->wallEnd0 + geoDesc->wallEnd1 + geoDesc->wallEnd1) / 3;
				circle(image, resizePoint(oneThird, scaleX, scaleY, locus), dotScale, wallClr, 1);
				circle(image, resizePoint(oneHalf, scaleX, scaleY, locus), dotScale, wallClr, 1);
				circle(image, resizePoint(twoThirds, scaleX, scaleY, locus), dotScale, wallClr, 1);
			}

		}
	}
	else {
		circle(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), dotScale * 4, wallClr, (geoDesc->mRandom) ? -1 : 1);
		circle(image, resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), dotScale * 4, wallClr, 1);
		if (geoDesc->isEmpty(geoDesc->bentCentre)) {
			line(image, resizePoint( geoDesc->wallEnd0 , scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
		}
		else {
			if (geoDesc->mShape == geometryDesc::bulge) {
				circle(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), dotScale * 4, wallClr, 1);
				circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), dotScale * 4, bulgeClr, 1);
			}
			else {
				if (!geoDesc->isEmpty(geoDesc->wallEnd0)) {
					line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), wallClr, 1);
					line(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), wallClr, 1);
				}
				circle(image, resizePoint(geoDesc->bentCentre, scaleX, scaleY, locus), dotScale * 4, wallClr, (geoDesc->mRandom) ? -1 : 1);
				if (geoDesc->mShape == geometryDesc::bent && !geoDesc->isEmpty(geoDesc->pit0 ) && !geoDesc->isEmpty(geoDesc->pit1)){
					circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale * 3, PitClr, 1);
					circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), dotScale * 3, PitClr, 1);
				}
			}
		}

	}
	if (drawCombFeatures) {
		int circleSize = 8 * mZoom;
		if (geoDesc->mShape == geometryDesc::bulge2) {
			if (!geoDesc->isEmpty(geoDesc->pit0)) {
				circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale * 3, PitClr, 1);
				circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), circleSize, PitClr, 1);
			}
			line(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), builtWallClr, 1);
			line(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), builtWallClr, 1);
		}
		else if (geoDesc->mShape == geometryDesc::rim1pit) {
			//if (!geoDesc->isEmpty(geoDesc->pit0)) {
			//	circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), pitSize, wallClr, 1);
			//}
			//if (!geoDesc->isEmpty(geoDesc->pit1)) {
			//	circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), pitSize, wallClr, 1);
			//}
			circle(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), dotScale * 4, PitClr, 1);
			line(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->wallEnd1, scaleX, scaleY, locus), builtWallClr, 1);
			circle(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), dotScale * 4, PitClr, 1);
			line(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), builtWallClr, 1);
			circle(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), dotScale * 4, PitClr, 1);
			line(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), builtWallClr, 1);
		}
		else if (geoDesc->mShape == geometryDesc::rim2pits) {
			//if (!geoDesc->isEmpty(geoDesc->pit0)) {
			//	circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), pitSize, wallClr, 1);
			//}
			//if (!geoDesc->isEmpty(geoDesc->pit1)) {
			//	circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), pitSize, wallClr, 1);
			//}
			line(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), builtWallClr, 1);
			line(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), builtWallClr, 1);
		}
		else if (geoDesc->mShape == geometryDesc::pitcell1 || geoDesc->mShape == geometryDesc::pitcell2) {
			if (!geoDesc->isEmpty(geoDesc->builtWall00)) {
				if (!geoDesc->isEmpty(geoDesc->builtWall01)) {
					drawCircleFromDiameter(image, scaleX, scaleY, locus, geoDesc->builtWall00, geoDesc->builtWall01, wallClr, 1);
				}
				else {
					circle(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), dotScale * 2, wallClr, 1);
				}
			}
			if (geoDesc->mShape == geometryDesc::pitcell2 && (!geoDesc->isEmpty(geoDesc->builtWall10))) {
				if (geoDesc->mShape == geometryDesc::pitcell2 && (!geoDesc->isEmpty(geoDesc->builtWall11))) {
					drawCircleFromDiameter(image, scaleX, scaleY, locus, geoDesc->builtWall10, geoDesc->builtWall11, wallClr2, 1);
				}
				else {
					circle(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), dotScale * 2, wallClr2, 1);
				}
			}
		}
		else if (geoDesc->mShape == geometryDesc::curve) {
	}
	else {
			line(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), builtWallClr, 1);
			circle(image, resizePoint(geoDesc->builtWall00, scaleX, scaleY, locus), dotScale * 2, builtWallClr, -1);
			circle(image, resizePoint(geoDesc->builtWall01, scaleX, scaleY, locus), dotScale * 2, builtWallClr, -1);
			if (!geoDesc->isEmpty(geoDesc->builtWall10) && !geoDesc->isEmpty(geoDesc->builtWall11)) {
				circle(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), dotScale * 2, builtWallClr, -1);
				circle(image, resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), dotScale * 2, builtWallClr, -1);
				line(image, resizePoint(geoDesc->builtWall10, scaleX, scaleY, locus), resizePoint(geoDesc->builtWall11, scaleX, scaleY, locus), builtWallClr, 1);
			}
			if (!geoDesc->isEmpty(geoDesc->pit0)) {
				circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), dotScale * 3, PitClr, 1);
				if (geoDesc->isEmpty(geoDesc->pit1)) {
					longLine(image, resizePoint(geoDesc->wallEnd0, scaleX, scaleY, locus), resizePoint(geoDesc->pit0, scaleX, scaleY, locus), PitLineClr, 1);
				}
				else {
					circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), dotScale * 3, PitClr, 1);
					longLine(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), resizePoint(geoDesc->pit1, scaleX, scaleY, locus), PitLineClr, 1);
					if (!geoDesc->isEmpty(geoDesc->pit2)) {
						circle(image, resizePoint(geoDesc->pit2, scaleX, scaleY, locus), dotScale * 3, PitClr, 1);
						longLine(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), resizePoint(geoDesc->pit2, scaleX, scaleY, locus), PitLineClr, 1);
						longLine(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), resizePoint(geoDesc->pit2, scaleX, scaleY, locus), PitLineClr, 1);
						if (geoDesc->mShape == geometryDesc::bulge) {
							circle(image, resizePoint(geoDesc->pit0, scaleX, scaleY, locus), circleSize, PitClr, 1);
							circle(image, resizePoint(geoDesc->pit1, scaleX, scaleY, locus), circleSize, PitClr, 1);
							circle(image, resizePoint(geoDesc->pit2, scaleX, scaleY, locus), circleSize, PitClr, 1);

						}
					}
				}
			}
		}
	}
}
