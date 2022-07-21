#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"
#include "csvCompareConfig.h"
#include "perspective.h"
#include "geometryDesc.h"
#include "adjustableDisplay.h"

using namespace cv;
using namespace std;

#define CELL_SCALE_RAND_PLACEMENT_MM 10

#define MENU_NONE -1
#define MENU_0 0
#define MENU_25 1
#define MENU_50 2
#define MENU_75 3
#define MENU_100 4
#define MENU_DISTORT 5
#define MENU_LOCS_STATIC 6
#define MENU_LOCS_MOBILE 7
#define MENU_CLEAR_OFFSET 8
#define MENU_ZOOM_CENTRE 9
#define MENU_GEO (MENU_ZOOM_CENTRE + 1)
#define MENU_SAVE (MENU_GEO + 1)
#define MENU_QTY (MENU_SAVE + 1)

#define MENU_SUB_LOC0 (0  + MENU_QTY)
#define MENU_SUB_LOC1 (1  + MENU_QTY)
#define MENU_SUB_LOC2 (2  + MENU_QTY)
#define MENU_SUB_LOC3 (3  + MENU_QTY)
#define MENU_SUB_DISTORT (4  + MENU_QTY)
#define MENU_SUB_STATIC (5  + MENU_QTY)
#define MENU_SUB_MOBILE (6  + MENU_QTY)
#define MENU_SUB_SCALE0 (7  + MENU_QTY)
#define MENU_SUB_SCALE1 (8  + MENU_QTY)
#define MENU_SUB_SCALECELL (9  + MENU_QTY)
#define MENU_SUB_SCALECELLWALL (10  + MENU_QTY)
#define MENU_SUB_CELLBOUNDARY (11  + MENU_QTY)
#define MENU_SUB_DONE (12  + MENU_QTY)
#define MENU_SUB_QTY (MENU_SUB_DONE - MENU_SAVE)

#define SCALE_BETWEEN_SIDEBARS 335
#define DEFAULT_PIXELS_PER_MM 10

#define MENU_GEO_END (0 + MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_SIDE (1 + MENU_GEO_END)
#define MENU_GEO_BENT (1 + MENU_GEO_SIDE)
#define MENU_GEO_BULGE (1 + MENU_GEO_BENT)
#define MENU_GEO_BULGE2 (1 + MENU_GEO_BULGE)
#define MENU_GEO_RIM1PIT (1 + MENU_GEO_BULGE2)
#define MENU_GEO_RIM2PITS (1 + MENU_GEO_RIM1PIT)
#define MENU_GEO_PITCELL1 (1 + MENU_GEO_RIM2PITS)
#define MENU_GEO_PITCELL2 (1 + MENU_GEO_PITCELL1)
#define MENU_GEO_ZIGZAG (1 + MENU_GEO_PITCELL2)
#define MENU_GEO_CURVE (1 + MENU_GEO_ZIGZAG)
#define MENU_GEO_HEIGHT (1 + MENU_GEO_CURVE)
#define MENU_GEO_DEPOSIT (1 + MENU_GEO_HEIGHT)
#define MENU_GEO_WALLEND0 (1 + MENU_GEO_DEPOSIT)
#define MENU_GEO_CURVEENDSTART MENU_GEO_WALLEND0
#define MENU_GEO_WALLEND1 (1 + MENU_GEO_WALLEND0)
#define MENU_GEO_CURVEENDEND MENU_GEO_WALLEND1
#define MENU_GEO_WALLCENTRE (1 + MENU_GEO_WALLEND1)
#define MENU_GEO_CURVE_THIRD_OR_HALF MENU_GEO_WALLCENTRE
#define MENU_GEO_PIT0 (1 + MENU_GEO_WALLCENTRE)
#define MENU_GEO_CURVE_2THIRDS MENU_GEO_PIT0
#define MENU_GEO_PIT1 (1 + MENU_GEO_PIT0)
#define MENU_GEO_CURVE_2THIRDS_SKIP MENU_GEO_PIT1
#define MENU_GEO_PIT2 (1 + MENU_GEO_PIT1)
#define MENU_GEO_BUILTWALL00 (1 + MENU_GEO_PIT2)
#define MENU_GEO_CURVE_THIRD_WALL MENU_GEO_BUILTWALL00
#define MENU_GEO_BUILTWALL01 (1 + MENU_GEO_BUILTWALL00)
#define MENU_GEO_CURVE_HALF_WALL MENU_GEO_BUILTWALL01
#define MENU_GEO_BUILTWALL10 (1 + MENU_GEO_BUILTWALL01)
#define MENU_GEO_CURVE_HALF_STOP MENU_GEO_BUILTWALL10
#define MENU_GEO_BUILTWALL11 (1 + MENU_GEO_BUILTWALL10)
#define MENU_GEO_CURVE_2THIRDS_WALL MENU_GEO_BUILTWALL11
#define MENU_GEO_DONE (1 + MENU_GEO_BUILTWALL11)
#define MENU_GEO_CURVE_STOP MENU_GEO_DONE
#define MENU_GEO_EDIT (1 + MENU_GEO_DONE)
#define MENU_GEO_DELETE (1 + MENU_GEO_EDIT)

#define MENU_GEO_START (MENU_QTY + MENU_SUB_QTY)
#define MENU_GEO_QTY (MENU_GEO_DELETE - MENU_SUB_DONE)

#define MENU_ALL_QTY (MENU_QTY + MENU_SUB_QTY + MENU_GEO_QTY)
#define DEFAULT_RAD 40

#define TRANSP_SAMPLE_RADIUS_PIXELS 10

#define MENU_PANEL_WIDTH 135
#define MENU_PANEL_LINE_HEIGHT 20

#define MENU_HEIGHTIDX_0 0
#define MENU_HEIGHTIDX_1 1
#define MENU_HEIGHTIDX_2 2
#define MENU_HEIGHTIDX_3 3
#define MENU_HEIGHTIDX_4 4
#define MENU_HEIGHTIDX_QTY 5

#define MENU_DEPOSITIDX_0 0
#define MENU_DEPOSITIDX_1 1
#define MENU_DEPOSITIDX_2 2
#define MENU_DEPOSITIDX_3 3
#define MENU_DEPOSITIDX_4 4
#define MENU_DEPOSITIDX_5 5
#define MENU_DEPOSITIDX_6 6
#define MENU_DEPOSITIDX_7 7
#define MENU_DEPOSITIDX_8 8
#define MENU_DEPOSITIDX_9 9
#define MENU_DEPOSITIDX_10 10
#define MENU_DEPOSITIDX_QTY 11

typedef void(*zonesChangedCallback) (void);


class superimposeDisplayDance
{
private:
	csvCompareConfig mConfig;
	clickStatus primaryClickStatus;
	enum menuMode { topMenu, reflocs, geometries, cells };
	char *mMenuTitle[4];
	menuMode mMenuMode;
	char *mWindowName;
	std::string mConfigFilename1;
	std::string mConfigFilename2;
	std::string mConfigFilenameJoint;
	std::string mConfigCellFilename1;
	std::string mConfigCellFilename2;
	std::string mExperimentID;
	int mDisplayedImageIdx;
	bool mCellsNeedUpdate;
	char *mSliderName;
	char* mSliderScaleX;
	char* mSliderScaleY;
	char* mSliderRotate;
	char* mSliderZoom;
	Mat mOrgImage;
	Mat mBaseImage;
	Mat mSuperImage;
	Mat mAdjustedImage;
	Mat mRescaledImage;
	Mat mDisplayImage;
	int mRows;
	int mCols;
	int mDispRows;
	int mDispCols;
	float mScaleX;
	float mScaleY;
	float mRotate;
	float mZoom;
	Point mZoomLocus;
	Point mImageLocAtTopLeftOfdisplay;
	perspective mPersp;
	vector<geometryDesc *> mGeometryDescriptions;
	int mGeoBeingEdited;
	bool showGeoTypes[geometryDesc::shape::SHAPE_QTY];
	bool showGeoTypesR[geometryDesc::shape::SHAPE_QTY];

	bool mApplyDistortion = false;
	bool mHaveStaticLocs[4];
	bool mHaveMobileLocs[4];
	bool mHaveAllStaticLocs;
	bool mHaveAllMobileLocs;
	perspectiveImageReference mPerspectiveStatic;
	perspectiveImageReference mPerspectiveMobile;
	int mWhichPoint;
	bool mStaticPointsNotMobile;
	char *mMenuStr[MENU_ALL_QTY];
	bool mMenuSelected[MENU_ALL_QTY];
	char* mMenuHeightStr[MENU_HEIGHTIDX_QTY];
	char* mMenuDepositStr[MENU_DEPOSITIDX_QTY];
	int mMenuHeightIdx;
	int mMenuSelection;
	int mMenuDepositIdx;
	bool mRtClickedMenu;
	void clearAndSetMenu(int menuSelection, menuMode whichMenu);
	void clearMenuSelections();
	void superimposeDisplayDance::clearMenuLocSelections();
	void flipMenu(int menuSelection, menuMode whichMenu);
	geometryDesc* getCurrentGeoLocation();
	void setGeoLocation(Mat baseImage, geometryDesc *pCurrentGeo, int whichPoint, int x, int y);
	void setRefLocation(int whichPoint, bool staticNotMobile, int x, int y);
	void flagRefLocationAsSet(int whichPoint, bool staticNotMobile);
	int getClosestRefLocation(bool staticNotMobile, int x, int y);
	void drawCross(Mat image, Point whereOnImage, char *name, bool plusNotTimes);
	void drawCrosses(Mat image, perspectiveImageReference *relevantPerspective, bool plusNotTimes);
	void drawScalePoints(Mat image, float scaleX, float scaleY, Point locus);
	void drawGeometries(Mat image, float scaleX, float scaleY, Point locus, bool drawCombFeatures);
	void drawScalePoint(Mat image, Point2f pt, float scaleX, float scaleY, Point locus);
	void drawGeometry(Mat image, geometryDesc* geoDesc, float scaleX, float scaleY, Point locus, bool highlight, bool drawCombFeatures);
	void drawCircleFromDiameter(Mat image, float scaleX, float scaleY, Point locus, Point dia0, Point dia1, Scalar wallClr, int weight);

	float randomOffset(float maxMM);
	int findClosestGeometry(Point loc);
	void removeGeometry(int whichOne);
	void editGeometry(int whichOne);
	void geoEntryDone();
	int superimposeDisplayDance::getCurveNextPoint(int putativeNextPoint);
	geometryDesc::shape getGeometryTypeBeingEdited();
	float superimposeDisplayDance::zoomCalc(int val);
	float superimposeDisplayDance::pixelsPerMM();

	void adjustImage(bool bDrawCrosses);
	void rightClick(int x, int y);
	void showMenu(menuMode whichMenu);
	void showMenuLine(int lineNo, char *txt);
	int menuSelected(int x, int y, menuMode whichMenu);
	Mat translateImg(Mat &img, int offsetx, int offsety);
	void loadConfigFile();
	int mCurrentOffsetX;
	int mCurrentOffsetY;
	int mDispContrastTrackVal;
	int mTrackValScaleX;
	int mTrackValScaleY;
	int mTrackValRotate;
	int mTrackValZoom;
	float mDispDivision;
	Point2f mScalePt0;
	Point2f mScalePt1;
	float scaleDistanceMM;
	Point2f mCellScale0;
	Point2f mCellScale1;
	string mHive;

#define CELL_SAMPLE_RANGE_MM 30
	adjustableDisplay *mAdjustableDisplay = nullptr;

public:
	int mOffsetX;
	int mOffsetY;
	superimposeDisplayDance(char *windowName, Mat baseImage, Mat superImage, int dispRows = 768, int dispCols = 1024, float dispDivision = 1);
	~superimposeDisplayDance();
	void redisplayDance(Mat baseImage, Mat superImage);
	void setAdjustableDisplay(adjustableDisplay *pAdjustableDisplay);
	void setConfigFile(const char* pConfigFilename1, const char* pConfigFilename2, const char* pConfigFilenameJoint, const char *experimentID);
	void menuQtyString(char *strBuff, int menuIdx, menuMode whichMenu);
	void mouseCallback(int event, int x, int y, int flags);
	void trackCallback(int val);
	void trackCallbackScaleX(int val);
	void trackCallbackScaleY(int val);
	void trackCallbackRotate(int val);
	void trackCallbackZoom(int val);
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	static void trackCallback(int val, void* userData);
	static void trackCallbackScaleX(int val, void* userData);
	static void trackCallbackScaleY(int val, void* userData);
	static void trackCallbackRotate(int val, void* userData);
	static void trackCallbackZoom(int val, void* userData);
	void superimposeDisplayDance::rescaleSuperImage();
	void resizeFrame(Mat* inputMat, Mat* outputMat, float scaleX, float scaleY, Point locus);
	Point resizePoint(Point2f inputLoc, float scaleX, float scaleY, Point locus);
	Point resizePoint(Point2f inputLoc, float imageDivision, float scaleX, float scaleY, Point locus);
	Point unResizePoint(Point2f inputLoc, float scaleX, float scaleY, Point locus);
	void showImg();
	void hide();
	float getScaleDistanceMM();
	void setScaleDistanceMM(float dist);
	string superimposeDisplayDance::getHive();
	void superimposeDisplayDance::setHive(string hive);
	void saveConfigFile();
	void setGeoTypesShown(bool *setValsNative, bool *setValsRandom, int arrySize);

};

