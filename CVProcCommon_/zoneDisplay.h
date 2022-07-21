#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"
#include "perspective.h"
#include "zone3d.h"

using namespace cv;
using namespace std;
#define MENU_QTY 11
#define MENU_NONE -1
#define MENU_SUBZONES 0
#define MENU_CIRCLE 1
#define MENU_TRIANGLE 2
#define MENU_SQUARE 3
#define MENU_DLEFT 4
#define MENU_DRIGHT 5
#define MENU_ADD 6
#define MENU_MOVE 7
//#define MENU_RESIZE 6
#define MENU_DELETE 8
#define MENU_SETRAD 9
#define MENU_UPDATE 10

#define MENU_SUB_QTY 6
#define MENU_SUB_TOP (0 + MENU_QTY)
#define MENU_SUB_ADD (1 + MENU_QTY)
#define MENU_SUB_MOVE (2 + MENU_QTY)
#define MENU_SUB_RESIZE (3 + MENU_QTY)
#define MENU_SUB_DELETE (4 + MENU_QTY)
#define MENU_SUB_UPDATE (5 + MENU_QTY)

#define MENU_PROP_QTY 14
#define MENU_PROP_TOP (0 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_CIRCLE (1 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_SQUARE (2 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_TRIANGLE (3 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_DLEFT (4 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_DRIGHT (5 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_EXCLUDE (6 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_DUPLICATE (7 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_MOVE (8 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_USERTYPE (9 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_SETRAD (10 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_DEPTH (11 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_ANGLE (12 + MENU_QTY + MENU_SUB_QTY)
#define MENU_PROP_UPDATE (13 + MENU_QTY + MENU_SUB_QTY)

#define MENU_ALL_QTY (MENU_QTY + MENU_SUB_QTY + MENU_PROP_QTY)
#define DEFAULT_RAD 40

#define MENU_PANEL_WIDTH 120
#define MENU_PANEL_LINE_HEIGHT 20

typedef void(*zonesChangedCallback) (void);

class zoneDisplay
{
private:
	enum menuMode { topMenu, subMenu, properties };
	char *mMenuTitle[3];
	menuMode mMenuMode;
	char *mWindowName;
	char *mSliderName;
	Mat mOrgImage;
	Mat mAdjustedImage;
	Mat mDisplayImage;
	bool mPropExclude;
	int mPropDepthVal;
	int mPropAngleVal;
	int mPropRadVal;
	int mPropUserType;
	int mRadVal;
	int mRadValSub;
	int mSelectedZoneIdx;
	int mSelectedSubZoneIdx;
	zone3d *mSelectedZone;
	zonesChangedCallback mOnChange;
	int mNextZoneID;
	clkStat mClkStat;
	perspective *pPerspective;
	vector<zone3dCompound *> *mCompoundZones;
	vector<string> mUserDefinedTypes;
	int mRows;
	int mCols;
	int mTrackVal;
	char *mMenuStr[MENU_QTY + MENU_SUB_QTY + MENU_PROP_QTY];
	bool mMenuSelected[MENU_QTY + MENU_SUB_QTY + MENU_PROP_QTY];
	int mMenuSelection;
	void clearAndSetMenu(int menuSelection, menuMode whichMenu);
	void clearMenuSelections();
	void flipMenu(int menuSelection, menuMode whichMenu);
	void adjustDisplay();
	void showMenu(menuMode whichMenu);
	void showMenuLine(int lineNo, char *txt);
	void zoneDisplay::drawZoneOutline(Mat img, zone3d *pZn, Scalar clr, int width);
	int menuSelected(int x, int y, menuMode whichMenu);
	int findClickedZone(int x, int y, int minIdx);
	int zoneDisplay::findClickedSubZone(int pxlX, int pxlY);
	bool zoneDisplay::findClickedZoneAndSubZone(int pxlX, int pxlY);
	bool zoneIdxIsValid(int zIdx, int szIdx = -1, bool checkSubIdx = false);
	void deleteZone(int zIdx, int szIdx);
	void zoneDisplay::showPropertiesMenuOfZone(int zIdx, int szIdx);
	void zoneDisplay::duplicateZone(int pxlX, int pxlY, int selectedZone, int selectedSubZone);
	zone3d *zoneDisplay::findZone(const char *zoneName);
	int zoneDisplay::findZoneIdx(const char *zoneName);
	char *zoneDisplay::generateDuplicateName(const char *oldName);
	void toggleExclusion(int zIdx, int szIdx);
	bool getExclusion(int zIdx, int szIdx);

public:
	zoneDisplay(char *windowName);
	~zoneDisplay();

	void menuQtyString(char *strBuff, int menuIdx, menuMode whichMenu);
	void mouseCallback(int event, int x, int y, int flags);
	void trackCallback(int val);
	void showLocData(int pxlX, int pxlY, Mat adjustedImage);
	void adjustImage();
	void addZone(int pxlX, int pxlY);
	void addSubZone(int pxlX, int pxlY);
	void drawZone(Mat img, zone3dCompound *pZn, bool selected);
	void drawSubZone(Mat img, zone3d *pZn, bool selected);
	void zoneDisplay::drawZoneName(Mat img, zone3d *pZn);
	void setZoneCentre(int pxlX, int pxlY, int idx);
	void zoneDisplay::setSubZoneCentre(int pxlX, int pxlY, int idx, int subIdx);
	void setZoneRad(int pxlX, int pxlY, int idx);
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	static void trackCallback(int val, void* userData);
	void showImg(Mat img, perspective *pPersp, std::vector<zone3dCompound *> *zones, zonesChangedCallback onChange);
	void drawZonesOnAdjustedImage(Mat img);
	void hide();
	vector<zone3dCompound *> *getZones() { return mCompoundZones; }
	void getUserDefinedZoneTypes(vector<string> *udzOut);
	void setUserDefinedZoneTypes(vector<string> *udzIn);
};

