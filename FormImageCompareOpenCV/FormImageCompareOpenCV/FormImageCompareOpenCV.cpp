//	src = imread("C:/projects/OpenCV/opencv-3.3/opencv/sources/samples/data/lena.jpg", IMREAD_COLOR);
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "superimposeDisplay.h"
#include "display4Locations.h"
#include "adjustableDisplay.h"

using namespace cv;
using namespace std;
using namespace System;

namespace FormImageCompareOpenCV {
	// 2%
	
	int marginFraction = 50;
	int pixelsW;
	int pixelsH;
	int mFrameNum;
	float mFramesPerSec = 25;
	bool isOpen = false;
	bool mForceFullSize = false;
	superimposeDisplay *pSUperDisplay = nullptr;
	display4Locations *pRefLocs = nullptr;
	display4Locations *pStartLocs = nullptr;
	display4Locations *pEndLocs = nullptr;
#define FP_OPENERR 1
#define FP_READERR 2
	perspectiveImageReference mReferenceGeomtry;
	perspectiveImageReference mStartGeomtry;
	perspectiveImageReference mEndGeomtry;
	perspective mPersp;

	adjustableDisplay *mAdjustableDisplay = nullptr;
	void setGeoTypesShown(bool *setValsNative, bool *setValsRandom, int arrySize) {
		pSUperDisplay->setGeoTypesShown(setValsNative, setValsRandom, arrySize);
	}

	int getWidth() {
		return pixelsW;
	}
	int getHeight() {
		return pixelsH;
	}
	int getFrameNum() {
		return mFrameNum;
	}
	float getFrameRate() {
		return mFramesPerSec;
	}

	bool pause = true;

	void pausePlay() {
		pause = !pause;
	}
	void addToAverage(Mat averageM, Mat inputM2, int *averageCount) {
		int channels = averageM.channels();
		int channels2 = inputM2.channels();
		(*averageCount)++;
		for (int y = 0; y < averageM.rows; y++) {
			int32_t* pd0 = averageM.ptr<int32_t>(y);
			uchar* pd1 = inputM2.ptr<uchar>(y);

			for (int x = 0; x < averageM.cols; x++) {
				for (int c = 0; c < channels; c++) {
					int idx = x * channels + c;
					int newVal = pd0[idx];
					newVal += pd1[idx];
					pd0[idx] = newVal;
				}
			}
		}
	}
	void downScaleAverage(Mat inputM, Mat output, int averageCount) {
		int channels = inputM.channels();
		int channelsd = output.channels();
		for (int y = 0; y < inputM.rows; y++) {
			int32_t* pd0 = inputM.ptr<int32_t>(y);
			uchar* pdd = output.ptr<uchar>(y);

			for (int x = 0; x < inputM.cols; x++) {
				for (int c = 0; c < channels; c++) {
					int idx = x * channels + c;
					int newVal = pd0[idx];
					newVal /= averageCount;
					pdd[idx] = newVal;
				}
			}
		}
	}
	bool mDisplayHalfsize = false;

	void halfSizeFrame(Mat *inputMat, Mat *outputMat) {
		//		imshow("d1", *inputClr);
		Mat tmp = inputMat->clone();
		//		imshow("d2", tmp);
		//blur(*inputClr, tmp, Size(5, 5), Point(-1, -1));
		resize(tmp, tmp, cv::Size(tmp.cols * 0.5, tmp.rows * 0.5), 0, 0, CV_INTER_LINEAR);
		*outputMat = tmp;
	}
	Mat imageRightSize( Mat frameMat) {
		Mat retv = frameMat;
		if (mDisplayHalfsize && !mForceFullSize) {
			Mat halfFrame;
			halfSizeFrame(&frameMat, &halfFrame);
			retv = halfFrame;
		}
		return retv;
	}
	void showImageRightSize(const cv::String &caption, Mat frameMat) {
		imshow(caption, imageRightSize( frameMat));
	}

	//Mat mat_l(img_height,img_width,CV_16UC3);
	Mat averageFrame;
	int averageFrameQty;
	Mat showAverage;
	Mat dispframe;
	string partFname;
	VideoCapture capture;
	Mat mImageComparison;
	bool showEdgeNotMask = false;
	vector<Rect2i *> mMaskRectangles;
	int tl, tr, bl, br, lt, lb, rt, rb;
	int tlB, ltB, brB, rbB;
	void blackOrWhiteOut(Mat inputM, int tl, int tr, int bl, int br, int lb, int lt, int rb, int rt, int fillValue) {
		int channels = inputM.channels();
		for (int y = 0; y < inputM.rows; y++) {
			uchar* pd0 = inputM.ptr<uchar>(y);
			int ll = (y < lt) ? tl : tl + ((y - lt) * (bl - tl)) / (lb - lt);
			int lr = (y < rt) ? tr : tr + ((y - rt) * (br - tr)) / (rb - rt);
			for (int x = 0; x < inputM.cols; x++) {
				int ltop = (x < tl) ? lt : lt + ((x - tl) * (rt - lt)) / (tr - tl);
				int lbot = (x < bl) ? lb : lb + ((x - bl) * (rb - lb)) / (br - bl);
				if (x < ll || x > lr || y < ltop || y > lbot) {
					int idx = x * channels;
					for (int c = 0; c < channels; c++) {
						pd0[idx + c] = fillValue;
					}
				}
			}
		}

	}
	void whiteout(Mat inputM, int tl, int tr, int bl, int br, int lb, int lt, int rb, int rt) {
		int channels = inputM.channels();
		for (int y = 0; y < inputM.rows; y++) {
			uchar* pd0 = inputM.ptr<uchar>(y);
			int ll = tl + (y < lt)? 0 : ((y - lt) * (bl - tl)) / (lb - lt);
			int lr = tr + (y > rt)? 0 : ((y - rt) * (br - tr)) / (rb - rt);
			for (int x = 0; x < inputM.cols; x++) {
				int ltop = lt + (x * (rt - lt)) / inputM.cols;
				int lbot = lb + (x * (rb - lb)) / inputM.cols;
				if (x > ll && x < lr && y > ltop && y < lbot) {
					int idx = x * channels;
					for (int c = 0; c < channels; c++) {
						pd0[idx + c] = 0;
					}
				}
			}
		}

	}
	void forceFullSize(bool doFullsize) {
		mForceFullSize = doFullsize;
	}
	void clearAverage() {
		averageFrame = Mat::zeros(pixelsH, pixelsW, CV_32SC3);
		averageFrameQty = 0;
	}
	void openComparison(const char* filepathname)
	{
		mImageComparison = imread(filepathname);
		if (pSUperDisplay != nullptr) delete pSUperDisplay;
		pSUperDisplay = new superimposeDisplay("SUperDisplay", imageRightSize(showAverage), imageRightSize(mImageComparison));
		pSUperDisplay->showImg();
	}
	void setScaleDistanceMM(float dist) {
		pSUperDisplay->setScaleDistanceMM(dist);
	}
	float getScaleDistanceMM() {
		return pSUperDisplay->getScaleDistanceMM();
	}
	std::string getHive() {
		return pSUperDisplay->getHive();
	}
	void setHive(std::string hive) {
		pSUperDisplay->setHive(hive);
	}
	void saveConfigFile() {
		pSUperDisplay->saveConfigFile();
	}
	std::string getCellData(std::string boundaryType) {
		return pSUperDisplay->getCellData(boundaryType);
	}

	void openComparison(const char* filepathname, const char* filepathname2, const char *filename1, const char * filename2, float sizeDivision)
	{
		std::string experimentID;
		std::string productName = string(filepathname2);
		std::string configName1 = string(filepathname);
		configName1 = configName1 + ".cfg.txt";
		std::string configName2 = string(filepathname2);
		configName2 = configName2 + ".cfg.txt";
		int pathoffset = productName.find_last_of('\\');
		std::string filenameStr2 = productName.substr(pathoffset + 1);
		productName = productName.substr(0, pathoffset);
		filenameStr2 = filenameStr2.substr(0, filenameStr2.find_last_of('.'));
		std::string pathname = string(filepathname);
		pathoffset = pathname.find_last_of('\\');
		std::string filenameStr = pathname.substr(pathoffset + 1);
		filenameStr = filenameStr.substr(0, filenameStr.find_last_of('.'));
		productName = productName + "\\" + filenameStr2 + "_" + filenameStr + ".cfg";
		experimentID = filenameStr.substr(0, 5);
		std::string windowCaption = "SuperDisplay ";
		windowCaption += filename1;
		windowCaption += " - ";
		windowCaption += filename2;
		mImageComparison = imread(filepathname);
		Mat imageCOmparison2 = imread(filepathname2);
		if (pSUperDisplay != nullptr) delete pSUperDisplay;
		pSUperDisplay = new superimposeDisplay((char *)(windowCaption.c_str()), imageRightSize(imageCOmparison2), imageRightSize(mImageComparison), 0, 0, sizeDivision);
		pSUperDisplay->setConfigFile(configName1.c_str(), configName2.c_str(), productName.c_str(), experimentID.c_str());
		pSUperDisplay->showImg();
	}

	void openTest(const char* filepathname, const char *filename, float sizeDivision)
	{

		std::string experimentID;
		std::string productName = string(filepathname);
		std::string configName1 = string(filepathname);
		configName1 = configName1 + ".cfg.txt";
		std::string configName2 = string(filepathname);
		configName2 = configName2 + ".cfg.txt";
		int pathoffset = productName.find_last_of('\\');
		std::string filenameStr2 = productName.substr(pathoffset + 1);
		productName = productName.substr(0, pathoffset);
		filenameStr2 = filenameStr2.substr(0, filenameStr2.find_last_of('.'));
		std::string pathname = string(filepathname);
		pathoffset = pathname.find_last_of('\\');
		std::string filenameStr = pathname.substr(pathoffset + 1);
		filenameStr = filenameStr.substr(0, filenameStr.find_last_of('.'));
		productName = productName + "\\" + filenameStr2 + "_" + filenameStr + ".cfg";
		experimentID = filenameStr.substr(0, 5);
		std::string windowCaption = "SuperDisplay ";
		windowCaption += filename;

		
		mImageComparison =  imread(filepathname);
		if (pSUperDisplay != nullptr) delete pSUperDisplay;
		pSUperDisplay = new superimposeDisplay((char *)(windowCaption.c_str()), imageRightSize(mImageComparison), imageRightSize(mImageComparison), 0, 0, sizeDivision);
		pSUperDisplay->setConfigFile(configName1.c_str(), configName2.c_str(), productName.c_str(), experimentID.c_str());
		pSUperDisplay->showImg();


		pSUperDisplay->resizeFrame(&mImageComparison, &mImageComparison, 1 , 1 , Point(500, 500));
		mAdjustableDisplay = new adjustableDisplay((char *)filepathname);
		pSUperDisplay->setAdjustableDisplay(mAdjustableDisplay);
		mAdjustableDisplay->showImg(mImageComparison, sizeDivision);
	}


	int openVideo(char* filepathname)
	{
		capture.open(filepathname);
		if (!capture.isOpened()) {
			//error in opening the video input
			cerr << "Unable to open video file: " << filepathname << endl;
			return FP_OPENERR;
		}
		if (!capture.read(dispframe)) {
			return FP_READERR;
		}
		mFrameNum = 1;
		isOpen = true;
		dispframe.copyTo(showAverage);
		pixelsW = dispframe.cols;
		pixelsH = dispframe.rows;
		clearAverage();
		addToAverage(averageFrame, dispframe, &averageFrameQty);
		downScaleAverage(averageFrame, showAverage, averageFrameQty);
		showImageRightSize("AverageFrame", showAverage);
		if (pixelsW > 1400) {
			mDisplayHalfsize = true;
		}
		partFname = filepathname;
		partFname += ".Metrics.txt";
		mFramesPerSec = capture.get(CV_CAP_PROP_FPS);

		namedWindow("Frame");
		showImageRightSize("Frame", dispframe);
		//namedWindow("Frame");
		//partFname = filepathname;
		//partFname += ".Background.png";
		//background = imread(partFname, IMREAD_COLOR);
		//imshow("Frame", background);
		mReferenceGeomtry.orgTL = mStartGeomtry.orgTL = mEndGeomtry.orgTL = Point2f(100, 150);
		mReferenceGeomtry.orgTR = mStartGeomtry.orgTR = mEndGeomtry.orgTR = Point2f(300, 150);
		mReferenceGeomtry.orgBL = mStartGeomtry.orgBL = mEndGeomtry.orgBL = Point2f(100, 350);
		mReferenceGeomtry.orgBR = mStartGeomtry.orgBR = mEndGeomtry.orgBR = Point2f(300, 350);
		mReferenceGeomtry.referenceLocation = mStartGeomtry.referenceLocation = mEndGeomtry.referenceLocation = 0;


//		bitwise_and(frame, frameMask, frame);

		return EXIT_SUCCESS;
	}
	float interpolatedPoint(float pointStart, float pointEnd, float ratio) {
		return pointStart + ratio * (pointEnd - pointStart);
	}
	Point2f interpolatedPoint(Point2f pointStart, Point2f pointEnd, float ratio) {
		return Point2f(interpolatedPoint(pointStart.x, pointEnd.x, ratio), interpolatedPoint(pointStart.y, pointEnd.y, ratio));
	}
	Mat transformOneFrame(Mat rawFrame, int frameNum) {
		float frameStretchRatio;
		Mat retv;
		rawFrame.copyTo(retv);
		perspectiveImageReference transformPerspective;
		frameStretchRatio = frameNum - mStartGeomtry.referenceLocation;

		frameStretchRatio /= mEndGeomtry.referenceLocation - mStartGeomtry.referenceLocation;
		transformPerspective.orgTL = interpolatedPoint(mStartGeomtry.orgTL, mEndGeomtry.orgTL, frameStretchRatio);
		transformPerspective.orgTR = interpolatedPoint(mStartGeomtry.orgTR, mEndGeomtry.orgTR, frameStretchRatio);
		transformPerspective.orgBR = interpolatedPoint(mStartGeomtry.orgBR, mEndGeomtry.orgBR, frameStretchRatio);
		transformPerspective.orgBL = interpolatedPoint(mStartGeomtry.orgBL, mEndGeomtry.orgBL, frameStretchRatio);
		mPersp.calcPerspectiveLambda(transformPerspective, mReferenceGeomtry);
		mPersp.warp(dispframe, retv, true);
		return retv;
	}
	void showOneVideoFrame(bool applyTransform) {
		if (isOpen && capture.read(dispframe)) {
			mFrameNum++;
			if (applyTransform)
				dispframe = transformOneFrame(dispframe, mFrameNum);
			showImageRightSize("Frame", dispframe);
			addToAverage(averageFrame, dispframe, &averageFrameQty);
			downScaleAverage(averageFrame, showAverage, averageFrameQty);
			showImageRightSize("AverageFrame", showAverage);
		}
	}
	void justShowOneVideoFrame() {
		if (isOpen && capture.read(dispframe)) {
			mFrameNum++;
			showImageRightSize("Frame", dispframe);
		}
	}
	void showNVideoFrame(int num, bool applyTransform) {
		for (int idx = 0; idx < num; idx++) {
			showOneVideoFrame(applyTransform);
		}
	}
	void setVideoFrame(int relativeFrame) {
		if (isOpen) {
			mFrameNum = capture.get(CAP_PROP_POS_FRAMES);
			mFrameNum += relativeFrame;
			capture.set(CAP_PROP_POS_FRAMES, mFrameNum);
			mFrameNum = capture.get(CAP_PROP_POS_FRAMES);
		}

	}
	void setVideoFrameAbs(int absFrame) {
		if (isOpen) {
			mFrameNum = absFrame;
			capture.set(CAP_PROP_POS_FRAMES, mFrameNum);
			mFrameNum = capture.get(CAP_PROP_POS_FRAMES);
		}

	}

	string beeDisplaySave(char* filepathname)
	{
		char outfname[4096];
		std::stringstream retvstr;
		if (isOpen) {
			snprintf(outfname, 4096, "%s.sparse.png", filepathname);
			imwrite(outfname, showAverage);
			retvstr << " , " << outfname << " saved";
		}
		return retvstr.str();
	}
	/*
		display4Locations *pRefLocs = nullptr;
	display4Locations *pStartLocs = nullptr;
	display4Locations *pEndLocs = nullptr;
#define FP_OPENERR 1
#define FP_READERR 2
	perspectiveImageReference mReferenceGeomtry;
	perspectiveImageReference mStartGeomtry;
	perspectiveImageReference mEndGeomtry;
*/
	void display4LocationsCallback(int callerSuppliedData) {
		switch (callerSuppliedData) {
		case -1:
			pRefLocs->getCopyOfPerspectiveData(&mReferenceGeomtry);
			break;
		case 0:
			pStartLocs->getCopyOfPerspectiveData(&mStartGeomtry);
			break;
		case 1:
			pEndLocs->getCopyOfPerspectiveData(&mEndGeomtry);
			break;
		}
	}
	void setGeometryUsingFrame(int frameNo, int StartOrEnd) {
		setVideoFrameAbs(frameNo - 1);
		justShowOneVideoFrame();
		switch (StartOrEnd) {
		case -1:
			break;
		case 0:
			mStartGeomtry.referenceLocation = frameNo;
			if (pStartLocs != nullptr) delete pStartLocs;
			pStartLocs = new display4Locations("Start Locs", dispframe, mStartGeomtry, display4LocationsCallback, 0);
			break;
		case 1:
			mEndGeomtry.referenceLocation = frameNo;
			if (pEndLocs != nullptr) delete pEndLocs;
			pEndLocs = new display4Locations("End Locs", dispframe, mEndGeomtry, display4LocationsCallback, 1);
			break;

		}
	}
	void openReference(const char* filepathname, int frameNUmber) {
		mImageComparison = imread(filepathname);
		if (pRefLocs != nullptr) delete pRefLocs;
		pRefLocs = new display4Locations("Ref Locs", mImageComparison, mReferenceGeomtry, display4LocationsCallback, -1);

	}

}