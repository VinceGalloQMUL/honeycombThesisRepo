#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


using namespace cv;
using namespace std;

public class perspectiveImageReference {
public:
	Point2f orgTL;
	Point2f orgTR;
	Point2f orgBR;
	Point2f orgBL;
	int referenceLocation;
	perspectiveImageReference() {
		orgTL.x = orgTL.y = 0;
		orgTR.x = orgTR.y = 0;
		orgBL.x = orgBL.y = 0;
		orgBR.x = orgBR.y = 0;
	}
	void clone(perspectiveImageReference& clonee);
	std::string serialise();
	void deserialise(std::vector<std::string> &valStrArray);
};

class perspective
{
	Mat mTopLambda;
	int mTopY;
	int mOrgHeadY = 250;
	int mAdjHeadY = 160;
	Mat mLambda;
	Mat mLambdaInv;
	Point2f mOrgPictureLoc[4];
	Point2f mAdjustedPictureLoc[4];
	Point2f mRWloc[4];
	bool mHaveRW[4];
	float mdydX;
	float mdxdY;
	float mdxdX;
	float mdydY;
	float mOrgx;
	float mOrgy;
	bool rotate = false;
	int imgHt;
	int imgWth;
	bool mPartitionedWarp = false;
	Point2f perspective::doRotation(Point2f inPt);
	void perspective::adjustedPerspective(vector<Point2f> &vTempIn, vector<Point2f> &vTempOut, bool inverse = false, bool doItAnyway = false);
public:
	bool mCanAdjustPerspective;
	bool mPerspectiveAdjusted;
	float fullImageRows;
	float fullImageCols;
	perspective();
	~perspective();
	void setToDoAdjustment(bool toDoIt);
	void setPartitionedWarp(bool doit) { mPartitionedWarp = doit; }
	void setRotated(int wth, int ht);
	void perspective::setPerspectiveLocation(int idx, Point2f orgLoc, Point2f destLoc, Point2f rw);
	void perspective::setPerspectiveRW(int idx, Point2f rw);
	void perspective::calcPerspectiveLambda();
	void perspective::calcPerspectiveLambda(perspectiveImageReference layoutFrom, perspectiveImageReference layoutTo);
	void perspective::calcPerspectiveLambda_(Point2f orgTL, Point2f orgTR, Point2f orgBR, Point2f orgBL, Point2f destTL, Point2f destTR, Point2f destBR, Point2f destBL);
	void perspective::calcPerspectiveLambda(Point2f orgTL, Point2f orgTR, Point2f orgBR, Point2f orgBL, Point2f rwTL, Point2f rwTR, Point2f rwBR, Point2f rwBL);
	void perspective::calcPerspectiveLambda(int imgW, int imgH, Point2f orgTL, Point2f orgTR, Point2f orgBR, Point2f orgBL, Point2f rwTL, Point2f rwTR, Point2f rwBR, Point2f rwBL);
	// transfom between adjusted and un-adjusted pixel locations (inverse == true or false)
	Point2f adjustedPerspective(Point2f inputPt, bool inverse = false, bool doItAnyway = false);
	// vTempIn and vTempOut need to be different vectors
	void warp(Mat srce, Mat dest, bool warpAnyway = false);
	void calcRWPlane();
	// take a pixel location within the adjusted image and return the realworld point
	Point2f getRWloc(Point2f adjustedPictureLoc);
	// take a pixel location within the UN-adjusted image, adjust it, then and return the realworld point
	Point2f getAdjustedRWloc(Point2f pictureLoc);
	// take a RealWorld location and return the pixel location on the ADJUSTED image
	Point2f perspective::getAdjustedPictureloc(Point2f rwLoc);
};

