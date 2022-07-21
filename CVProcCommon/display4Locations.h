#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"
#include "perspective.h"

using namespace cv;
using namespace std;
#define MENU_QTY 6
#define MENU_NONE -1
#define MENU_SETTL 0
#define MENU_SETTR 1
#define MENU_SETBR 2
#define MENU_SETBL 3
#define MENU_SETALL 4
#define MENU_UPDATE 5

#define MENU_SUB_QTY 0

#define MENU_PROP_QTY 0

#define MENU_ALL_QTY (MENU_QTY + MENU_SUB_QTY + MENU_PROP_QTY)
#define DEFAULT_RAD 40

#define MENU_PANEL_WIDTH 120
#define MENU_PANEL_LINE_HEIGHT 20

typedef void(*updateCallback) (int callerSuppliedData);


class display4Locations
{
private:
	clickStatus primaryClickStatus;
	perspectiveImageReference mPerspectiveGeometry;
	updateCallback mCallBackMethod;
	int mCallerSuppliedData;
	enum menuMode { topMenu, subMenu, properties };
	char *mMenuTitle[3];
	menuMode mMenuMode;
	char *mWindowName;
	Mat mOrgImage;
	Mat mDisplayImage;
	int mRows;
	int mCols;
	char *mMenuStr[MENU_QTY + MENU_SUB_QTY + MENU_PROP_QTY];
	bool mMenuSelected[MENU_QTY + MENU_SUB_QTY + MENU_PROP_QTY];
	int mMenuSelection;
	void clearAndSetMenu(int menuSelection, menuMode whichMenu);
	void clearMenuSelections();
	void flipMenu(int menuSelection, menuMode whichMenu);
	void setRefLocation(int menuChoiceIdx, int x, int y);
	void adjustDisplay();
	void showMenu(menuMode whichMenu, Mat image);
	void showMenuLine(Mat image, int lineNo, char *txt);
	int menuSelected(int x, int y, menuMode whichMenu);
	Mat translateImg(Mat &img, int offsetx, int offsety);
	int mWhichPoint;
	int mRefAllIdx;
	void drawCross(Mat image, Point whereOnImage, char *name);
	void drawCrosses(Mat image);
	void menuQtyString(char *strBuff, int menuIdx, menuMode whichMenu);
	void mouseCallback(int event, int x, int y, int flags);
	void adjustImage();
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	void showImg();

public:
	display4Locations(char *windowName, Mat baseImage, perspectiveImageReference perspectiveGeometry, updateCallback notification, int callerData);
	~display4Locations();

	void getCopyOfPerspectiveData(perspectiveImageReference *pPerspectiveGeometry);
	void hide();
};

