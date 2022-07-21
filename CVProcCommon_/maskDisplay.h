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
#define MENU_ADD 0
#define MENU_DELETE 1
#define MENU_CLEAR 2
#define MENU_SHOW 3
#define MENU_UPDATE 4

#define MENU_PANEL_WIDTH 120
#define MENU_PANEL_LINE_HEIGHT 20


class maskDisplay
{
private:
	char *mMenuTitle;
	char *mWindowName;
	char *mSliderName;
	Mat mOrgImage;
	Mat mAdjustedImage;
	Mat mDisplayImage;
	Mat mMask;
	Mat *mMaskOut;
	Mat mMaskIn;
	int mPropDepthVal;
	int mPropAngleVal;
	int mPropRadVal;
	int mPropUserType;
	int mRadVal;
	int mRadValSub;
	int mSelectedRectIdx;
	Rect2i *mSelectedRect;
	vector<Rect2i *> *mRects;
	clkStat mClkStat;
	int mRows;
	int mCols;
	int mTrackVal;
	char *mMenuStr[MENU_QTY];
	bool mMenuSelected[MENU_QTY];
	bool mShowFilled;
	int mMenuSelection;
	void clearAndSetMenu(int menuSelection);
	void clearMenuSelections();
	void flipMenu(int menuSelection);
	void adjustDisplay();
	void showMenu();
	void showMenuLine(int lineNo, char *txt);
	int menuSelected(int x, int y);
	int findRectIdx(int x, int y);
	bool rectIdxIsValid(int zIdx);
	void deleteRect(int zIdx);
	void createMask();
	Rect2i *maskDisplay::findRect(int x, int y);

public:
	maskDisplay(char *windowName);
	~maskDisplay();

	void mouseCallback(int event, int x, int y, int flags);
	void adjustImage();
	void addRect(int pxlX, int pxlY, int pxlBRX, int pxlBRY);
	void drawRect(Mat img, Rect2i *pZn, bool selected);
	static void mouseCallback(int event, int x, int y, int flags, void* userdata);
	void showImg(Mat img, std::vector<Rect2i *> *Rects, Mat *maskOut);
	void drawRectsOnAdjustedImage(Mat img);
	void drawRectsOnAdjustedImage(Mat img, Scalar clr, int width);
	void hide();
	vector<Rect2i *> *getRects() { return mRects; }
};

