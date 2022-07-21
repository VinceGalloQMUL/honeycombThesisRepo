#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>


using namespace cv;
using namespace std;
#define MENU_NONE -1
#define MENU_BASE 0
#define MENU_CONTRAST 1
#define MENU_ERODE 2
#define MENU_DILATE 3
#define MENU_THRESHOLD 4
#define MENU_CONTOURS 5
#define MENU_AREA_MIN 6
#define MENU_AREA_MAX 7
#define MENU_BLUR 8
#define MENU_ISOLATION 9
#define MENU_HIST 10
#define MENU_INVERT 11
#define MENU_EDGE 12
#define MENU_RECYCLE 13
#define MENU_QTY 14

#define MENU_PANEL_WIDTH 120
#define MENU_PANEL_LINE_HEIGHT 20

class adjustableDisplay
{
private:
	char *mWindowName;
	char *mWindowHistName;
	char *mWindowStatsName;
	char *mWindowContoursName;
	char *mSliderName;
	Mat mOrgImage;
	Mat mRecycleImage;
	Mat mAdjustedImage;
	Mat mDisplayImage;
	float mDivisionFactor;
	int mRows;
	int mCols;
	int mContrast;
	int mBaseLevel;
	int mErodeQty;
	int mDilateQty;
	int mContourMin;
	int mContourMax;
	int mThreshold;
	int mIsolation;
	int mBlur;
	int mTrackVal;
	char *mMenuStr[MENU_QTY];
	bool mMenuSelected[MENU_QTY];
	int mMenuSelection;
	void adjustableDisplay::clearAndSetMenu(int menuSelection);
	void adjustDisplay();
	void showMenu();
	void showMenuLine(int lineNo, char *txt);
	int adjustableDisplay::menuSelected(int x, int y);
	bool mHistogramDisplayed;
	bool mHistogramComputed;
	int ml1[256];
	int ml2[256];
	int mDoLog;
public:
	adjustableDisplay(char *windowName);
	~adjustableDisplay();

	static void adjustableDisplay::imageIsolation(Mat srce, Mat *dest, int isolationLevel);
	static void adjustableDisplay::imageIsolationTalestNeighbour(Mat srce, int *statsTalestNeighbour, int *statsAmTalest, int isolationLevel);
	static void adjustableDisplay::image2ndTallest(Mat srce, int *stats2nTallest, int isolationLevel);
	static void adjustableDisplay::imageSectionStats(Mat srce, int *stats, int cx, int cy, int sectionSize);
	static void adjustableDisplay::imageHistOfAve(Mat srce, int *stats, int isolationLevel);
	static float adjustableDisplay::image2ndTallestThreshold(Mat srce, int scope, float varMultiple);

	void adjustableDisplay::showHistOfAve(Mat bwImage, int scope);
	void adjustableDisplay::showHist2ndTallest(Mat bwImage, int doLog);
	void adjustableDisplay::showHistTallestNeighbour(Mat bwImage, int doLog);
	void adjustableDisplay::showHistFlex(Mat bwImage, int doLog);

	void adjustableDisplay::thresholdHist(Mat inputM, int scope);
	void adjustableDisplay::showGraph(int *l0, int *l1, int *l2, int len);
	void adjustableDisplay::showGraph(Mat canvas, int *l0, int len, float scaleFactor, Scalar colour);
	int adjustableDisplay::arrayMax(int *l0, int len);


	void getCellContoursCentresAndConnections(vector<vector<Point>> **retContours, vector<Point2f>**retCentres, vector<vector<int>> **retConnections);

	bool getWallIntersection(Point2i cell0Centre, Point2i cell1Centre, float * wallSide0, float *wallSide1);
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	static void trackCallback(int val, void* userData);
	void showImg(Mat img, float divisionFactor);
	//void showHist(Mat bwImage, int doLog);
	void showSectionStats(int x, int y, Mat bwImage, int scope);
	void adjustImage();
	void mouseCallback(int event, int x, int y, int flags);
	void trackCallback(int val);
	void setDisplayParams(float contrast, int baseLevel, int erodeQty, int dilateQty, int threshold);
	void getDisplayParams(float *pContrast, int *pBaseLevel, int *pErodeQty, int *pDilateQty, int *pThreshold);
	void showOrHideHistogram(Mat srce);
};

