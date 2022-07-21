#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"
#include "csvAreaConfig.h"
#include "perspective.h"
#include "areaDesc.h"

using namespace cv;
using namespace std;
#define MENU_NONE -1
#define MENU_0 0
#define MENU_25 1
#define MENU_50 2
#define MENU_75 3
#define MENU_100 4
#define MENU_DISTORT 5
#define MENU_LOCS 6
#define MENU_MENU_CORNER 7
#define MENU_GEO 8
#define MENU_SAVE 9
#define MENU_QTY (MENU_SAVE + 1)

#define MENU_SUB_LOC0 (0  + MENU_QTY)
#define MENU_SUB_LOC1 (1  + MENU_QTY)
#define MENU_SUB_LOC2 (2  + MENU_QTY)
#define MENU_SUB_LOC3 (3  + MENU_QTY)
#define MENU_SUB_DONE (4  + MENU_QTY)
#define MENU_SUB_QTY (MENU_SUB_DONE - MENU_SAVE)

#define SCALE_BETWEEN_SIDEBARS 335

#define MENU_GEO_0 (0 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_25 (1 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_50 (2 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_75 (3 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_100 (4 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_SCALE (5 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_TYPE (6 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_DRONE (7 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_POINT (8 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_LINE (9 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_TRI (10 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_QUAD (11 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_PT0 (12 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_PT1 (13 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_PT2 (14 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_PT3 (15 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_DONE (16 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_COUNT (17 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_DELTA (18 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_PREVIOUS (19 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_DELETE (20 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_QTY (MENU_GEO_DELETE - MENU_SUB_DONE)


#define MENU_ALL_QTY (MENU_QTY + MENU_SUB_QTY + MENU_GEO_QTY)
#define DEFAULT_RAD 40



#define MENU_PANEL_WIDTH 135
#define MENU_PANEL_LINE_HEIGHT 20



typedef void(*zonesChangedCallback) (void);


class superimposeAreaDisplay
{
private:
	csvAreaConfig mConfig;
	csvAreaConfig mConfigPrevious;
	bool mHavePrevious;
	clickStatus primaryClickStatus;
	enum menuMode { topMenu, reflocs, geometries };
	char *mMenuTitle[3];
	menuMode mMenuMode;
	char *mWindowName;
	std::string mConfigFilename;
	std::string mConfigFilenamePrevious;
	char *mSliderContrast;
	Point mMenuOrg;
	Mat mOrgImage;
	Mat mCombImage;
	Mat mAdjustedImage;
	Mat mRescaledCombImage;
	Mat mCombImagePrevious;
	float mContrast;
	bool mShowDelta;
	bool mPreviousCells;
	Mat mAdjustedImagePrevious;
	Mat mRescaledCombImagePrevious;
	Mat mDisplayImage;
	int mRows;
	int mCols;
	int mDispRows;
	int mDispCols;
	perspective mPersp;
	perspective mPerspPrevious;
	vector<areaDesc *> mAreaDescriptions;
	vector<areaDesc *> mAreaDescriptionsPrevious;
	areaDesc mTmpAreaDesc;
	int mGeoBeingEdited;
	int mScaleDistanceCells;
	float mDetectionRangeSq;
	Point mScaleCorner;
	Point mScaleHorizntal;
	Point mScaleDiagonal;
	int whichGeoCount = -1;

	bool mApplyDistortion = false;
	bool mHaveMobileLocs[4];
	bool mHaveAllMobileLocs;
	perspectiveImageReference mPerspectiveMobile;
	perspectiveImageReference mPerspectiveMobilePrevious;
	int mWhichPoint;
	bool mStaticPointsNotMobile;
	char *mMenuStr[MENU_QTY + MENU_SUB_QTY + MENU_GEO_QTY];
	bool mMenuSelected[MENU_QTY + MENU_SUB_QTY + MENU_GEO_QTY];
	char* mMenuTypeStr[MENU_BROODIDX_QTY];
	int mMenuSelection;
	int mMenuTypeIdx;

	void clearAndSetMenu(int menuSelection, menuMode whichMenu);
	void clearMenuSelections();
	void clearMenuLocSelections();
	void flipMenu(int menuSelection, menuMode whichMenu);
	areaDesc* getCurrentAreaLocation();
	void setAreaLocation(areaDesc *pCurrentArea, int whichPoint, int x, int y);
	void setRefLocation(int whichPoint, bool staticNotMobile, int x, int y);
	void flagRefLocationAsSet(int whichPoint);
	void drawCross(Mat image, Point whereOnImage, char *name, bool plusNotTimes);
	void drawCrosses(Mat image, perspectiveImageReference *relevantPerspective, bool plusNotTimes);
	void drawGeometries(bool doDelta, bool previousNotLatest, Mat image);
	void drawGeometry(bool doDelta, bool previousNotLatest, Mat image, areaDesc* geoDesc, bool highlight);
	int intermediateDotsForGeometries(bool doDelta, bool previousNotLatest, int cellType, std::vector<Point2f>& outPtArray);
	int intermediateDotsForGeometry(bool previousNotLatest, int cellType, areaDesc* geoDesc, std::vector<Point2f>& outPtArray);
	void intermediateDotsDelta(float pixelsPerCellHor, float pixelsPerCellHeight, std::vector<Point2f>& newPtArray, std::vector<Point2f>& missingPtArray, std::vector<Point2f>& coincidePtArray);
	int findClosestGeometry(Point loc);
	void removeGeometry(int whichOne);
	void geoEntryDone();
	areaDesc::shape getGeometryTypeBeingEdited();

	void resizeFrame(Mat* inputMat, Mat* outputMat);
	Point resizePoint(bool previousNotLatest, Point2f inputLoc, bool andPerspective);
	Point unResizePoint(bool previousNotLatest, Point2f inputLoc, bool andPerspective);
	void adjustImage(bool bDrawCrosses);
	void rightClick(int x, int y);
	void showMenu(menuMode whichMenu);
	void showMenuLine(int lineNo, char *txt);
	int menuSelected(int x, int y, menuMode whichMenu);
	void loadConfigFile();
	int mTrackVal;
	float mDispDivision;
	string mHive;
	void saveConfigFile(int *totals);
	int calcCount(int whichGeo);
	void superimposeAreaDisplayCommon(char *windowName, Mat combImage, int dispRows, int dispCols, float dispDivision);

public:
	superimposeAreaDisplay(char *windowName, Mat combImage, int dispRows = 768, int dispCols = 1024, float dispDivision = 1);
	superimposeAreaDisplay(char *windowName, Mat combImage, Mat combImagePrevious, int dispRows = 768, int dispCols = 1024, float dispDivision = 1);
	~superimposeAreaDisplay();
	void setScaleDistance(float dist);
	void setDetectionRange(float dist);
	float getScaleDistance();
	void setConfigFile(const char* pConfigFilename);
	void setConfigFile(const char* pConfigFilename, const char* pConfigFilenamePrevious);
	void menuQtyString(char *strBuff, int menuIdx, menuMode whichMenu);
	void mouseCallback(int event, int x, int y, int flags);
	void trackCallback(int val);
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	static void trackCallback(int val, void* userData);
	void rescaleSuperImage();
	void showImg();
	void hide();
	string getHive();
	void setHive(string hive);
	void saveConfigFileReq(int *totals);
};

