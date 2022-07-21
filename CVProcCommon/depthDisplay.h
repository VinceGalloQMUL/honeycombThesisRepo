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
#define MENU_TOP 0
#define MENU_BOTTOM 1
#define MENU_PROBE_0 2
#define MENU_PROBE_1 3
#define MENU_PROBE_2 4
#define MENU_PROBE_3 5
#define MENU_PROBE_4 6
#define MENU_PROBE_5 7
#define MENU_PROBE_6 8
#define MENU_ZOOM_CENTRE 9
#define MENU_USE_CALIBRATION 10
#define MENU_SAVE 11
#define MENU_QTY 12

#define MENU_PANEL_WIDTH 120
#define MENU_PANEL_LINE_HEIGHT 20
#define MENU_METRIC_WIDTH 100
#define MENU_METRIC_BASE 40
#define MENU_CURVE_ZERO 250
#define MENU_CURVE_MM 100

#define MAX_FLAT_DEPTH 0.3f
#define MIN_CURVED_DEPTH 0.4f
#define MAX_CURVED_DEPTH 1.0f
#define MIN_FACET_DEPTH 0.8f
#define MIN_DOMED_DEPTH -1.0f
#define MAX_DOMED_DEPTH -0.4f

class depthDisplay
{
private:
	char *mWindowName;
	char *mConfigFileName;
	char *mSliderName;
	Mat mOrgImage;
	Mat mAdjustedImage;
	Mat mDisplayImage;
	int mRows;
	int mCols;
	int mDispRows;
	int mDispCols;
	float mDispDivision;
	float mZoom;
	Point mZoomCentre;
	Point mTopLine;
	Point mBottomLine;
	Point mProbes[7];
	float mProbeMm[7];
	float mCalibMm[7];
	bool mUseCalibration;

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
	void depthDisplay::clearAndSetMenu(int menuSelection);
	void adjustDisplay();
	void showMenu();
	void showMenuLine(int lineNo, char *txt);
	void depthDisplay::showMetric(int lineNo, int SlotNo, char *txt);
	void depthDisplay::showCurve(float *candidates, bool flattened);
	int depthDisplay::menuSelected(int x, int y);
	void saveClickPosition(int x, int y, int menuSelection);
	void depthDisplay::drawCross(Mat image, Point whereOnImage, char *name, bool plusNotTimes);
	void depthDisplay::drawCrosses(Mat image);
	void depthDisplay::calcHeights();
	float calcFacteted(float *candidates, float *errors);
	float calcCurved(float *candidates, float *errors);
	float calcDomed(float *candidates, float *errors);
	float calcRegression(float *errors);
	float depthDisplay::flatten(float *candidates, float *flat);
	void depthDisplay::rebase(float *candidates, float *calibrated);
public:
	depthDisplay(char *windowName, float sizeDivision);
	~depthDisplay();

	static void depthDisplay::imageIsolation(Mat srce, Mat *dest, int isolationLevel);
	static void depthDisplay::imageIsolationTalestNeighbour(Mat srce, int *statsTalestNeighbour, int *statsAmTalest, int isolationLevel);
	static void depthDisplay::image2ndTallest(Mat srce, int *stats2nTallest, int isolationLevel);
	static void depthDisplay::imageHistOfAve(Mat srce, int *stats, int isolationLevel);

	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	static void trackCallback(int val, void* userData);
	void showImg(Mat img);
	void setConfigFile(char *configFilename, char *calibrationConfigFilename, bool useCalibration);
	void resizeFrame(Mat* inputMat, Mat* outputMat);
	Point resizePoint(Point2f inputLoc);
	Point unResizePoint(Point2f inputLoc);
	void resizeFrame(Mat* inputMat, Mat* outputMat, float dispDivision, float zoom);
	Point resizePoint(Point2f inputLoc, float dispDivision, float zoom);
	Point unResizePoint(Point2f inputLoc, float dispDivision, float zoom);
	void saveConfig();
	void loadConfig();

	void adjustImage();
	void mouseCallback(int event, int x, int y, int flags);
	void trackCallback(int val);
	void setDisplayParams(float contrast, int baseLevel, int erodeQty, int dilateQty, int threshold);
	void getDisplayParams(float *pContrast, int *pBaseLevel, int *pErodeQty, int *pDilateQty, int *pThreshold);
};

