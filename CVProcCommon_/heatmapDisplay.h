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
#define MENU_QTY 5
#define MENU_NONE -1
#define MENU_SETGRID 0
#define MENU_BLUR 1
#define MENU_THRESHOLD 2
#define MENU_CONTRAST 3
#define MENU_SAVE 4

#define MENU_ALL_QTY (MENU_QTY)
#define DEFAULT_GRID 4
#define DEFAULT_BLUR 2
#define DEFAULT_THRESHOLD 0
#define DEFAULT_CONTRAST 200

#define MENU_PANEL_WIDTH 120
#define MENU_PANEL_LINE_HEIGHT 20


class heatmapDisplay
{
private:
	enum menuMode { topMenu };
	std::vector<point3dMm> *pAllLocations;
	string mBaseFilename;
	float *pHeatMap;
	char *mMenuTitle[3];
	menuMode mMenuMode;
	char *mWindowName;
	char *mSliderName;
	Mat mOrgImage;
	Mat mAdjustedImage;
	Mat mDisplayImage;
	int mGridVal;
	int mBlurVal;
	int mThresholdVal;
	int mContrastVal;
	int mTrackVal;
	float mGapAllowed;
	vector3f mColdColour;
	vector3f mHotColour;
	clkStat mClkStat;
	perspective *pPerspective;
	bool mHaveImageAndData;
	int mRows;
	int mCols;
	float mBLxMm;
	float mBLyMm;
	float mTRxMm;
	float mTRyMm;
	int mGridCols;
	int mGridRows;
	char *mMenuStr[MENU_ALL_QTY];
	bool mMenuSelected[MENU_ALL_QTY];
	int mMenuSelection;
	void clearAndSetMenu(int menuSelection, menuMode whichMenu);
	void clearMenuSelections();
	void flipMenu(int menuSelection, menuMode whichMenu);
	void adjustDisplay();
	void showMenu(menuMode whichMenu);
	void showMenuLine(int lineNo, char *txt);
	int menuSelected(int x, int y, menuMode whichMenu);

public:
	heatmapDisplay(char *windowName);
	~heatmapDisplay();

	void setHotColour(int r, int g, int b);
	void setColdColour(int r, int g, int b);
	void menuQtyString(char *strBuff, int menuIdx, menuMode whichMenu);
	void mouseCallback(int event, int x, int y, int flags);
	void trackCallback(int val);
	void showLocData(int pxlX, int pxlY, Mat adjustedImage);
	void adjustImage();
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	static void trackCallback(int val, void* userData);
	void showImg(string baseFilename, Mat img, perspective *pPersp, std::vector<point3dMm> *allLocations);
	void saveImg();
	void showRevisedPoints(std::vector<point3dMm> *allLocations);
	void drawHeatmapOnAdjustedImage(Mat img);
	void adjustImage(Mat img, Mat *outImage, perspective *pPersp, std::vector<point3dMm> *allLocations, float refPxlX, float refPxlY, float blurPxls, float blurVal);
	void drawHeatmapOnAdjustedImage(Mat img, perspective *pPersp, std::vector<point3dMm> *allLocations, float refPxlX, float refPxlY, float blurPxls, float blurVal);
	void heatmapDisplay::addHeatmapForAdjustedImage(int cols, int rows, float *pOldHeatmap, perspective *pPersp, std::vector<point3dMm> *allLocations, float refPxlX, float refPxlY);
	void heatmapDisplay::heatmapImage(Mat *imgInOut, int cols, int rows, float *oldHeatmap, int blurRange);
	void heatmapDisplay::heatmapImageAlpha(Mat *imgInOut, Mat *foregroundInOut, int cols, int rows, float *oldHeatmap, int blurRange);
	void hide();
};

