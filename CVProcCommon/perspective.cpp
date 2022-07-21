#include "perspective.h"
#include "areaDesc.h"

void perspectiveImageReference::clone(perspectiveImageReference& clonee) {
	orgTL = clonee.orgTL;
	orgTR = clonee.orgTR;
	orgBR = clonee.orgBR;
	orgBL = clonee.orgBL;
	referenceLocation = clonee.referenceLocation;

}

std::string perspectiveImageReference::serialise() {
	std::stringstream ss;
	ss << orgTL.x << "," << orgTL.y << ",";
	ss << orgTR.x << "," << orgTR.y << ",";
	ss << orgBL.x << "," << orgBL.y << ",";
	ss << orgBR.x << "," << orgBR.y;
	return ss.str();
}

void perspectiveImageReference::deserialise(std::vector<std::string> &valStrArray) {
	if (valStrArray.size() == 8) {
		int idx = 0;
		orgTL.x = stof(valStrArray[idx++]);
		orgTL.y = stof(valStrArray[idx++]);
		orgTR.x = stof(valStrArray[idx++]);
		orgTR.y = stof(valStrArray[idx++]);
		orgBL.x = stof(valStrArray[idx++]);
		orgBL.y = stof(valStrArray[idx++]);
		orgBR.x = stof(valStrArray[idx++]);
		orgBR.y = stof(valStrArray[idx++]);
	}
}

perspective::perspective()
{
	mLambda =  Mat(2, 4, CV_32FC1);
	mLambdaInv =  Mat(2, 4, CV_32FC1);
	mCanAdjustPerspective = false;
	mPerspectiveAdjusted = false;
	mHaveRW[0] = mHaveRW[1] = mHaveRW[2] = mHaveRW[3] = false;
	fullImageCols = NUMCELLSWIDTH_ACTUAL;
	fullImageRows = NUMCELLSHEIGHTBROOD_ACTUAL;
}

perspective::~perspective()
{
}
void perspective::setRotated(int wth, int ht) {
	imgHt = ht;
	imgWth = wth;
	rotate = true;
}

Point2f perspective::doRotation(Point2f inPt) {
	if (rotate) {
		return Point2f(imgWth - inPt.x, imgHt - inPt.y);
	}
	else return inPt;
}
void perspective::setPerspectiveLocation(int idx, Point2f orgLoc, Point2f destLoc, Point2f rw) {
	if (idx >= 0 && idx < 4) {
		mOrgPictureLoc[idx] = doRotation( orgLoc);
		mAdjustedPictureLoc[idx] = destLoc;
		setPerspectiveRW(idx, rw);
	}
}
void perspective::setPerspectiveRW(int idx, Point2f rw) {
	if (idx >= 0 && idx < 4) {
		mRWloc[idx] = rw;

		mHaveRW[idx] = true;
		if (mHaveRW[0] && mHaveRW[1] && mHaveRW[2] && mHaveRW[3]) {
			calcRWPlane();
		}
	}
}

void perspective::setToDoAdjustment(bool toDoIt) {
	mPerspectiveAdjusted = toDoIt && mCanAdjustPerspective;
}

void perspective::calcPerspectiveLambda() {
	mLambda = getPerspectiveTransform(mOrgPictureLoc, mAdjustedPictureLoc);
	mLambdaInv = getPerspectiveTransform(mAdjustedPictureLoc, mOrgPictureLoc);
}
void perspective::calcPerspectiveLambda(perspectiveImageReference layoutFrom, perspectiveImageReference layoutTo) {
	calcPerspectiveLambda_(layoutFrom.orgTL, layoutFrom.orgTR, layoutFrom.orgBR, layoutFrom.orgBL, layoutTo.orgTL, layoutTo.orgTR, layoutTo.orgBR, layoutTo.orgBL);
}

void perspective::calcPerspectiveLambda_(Point2f orgTL, Point2f orgTR, Point2f orgBR, Point2f orgBL, Point2f destTL, Point2f destTR, Point2f destBR, Point2f destBL) {

	mOrgPictureLoc[0] = orgTL;
	mOrgPictureLoc[1] = orgTR;
	mOrgPictureLoc[2] = orgBR;
	mOrgPictureLoc[3] = orgBL;
	mAdjustedPictureLoc[0] = destTL;
	mAdjustedPictureLoc[1] = destTR;
	mAdjustedPictureLoc[2] = destBR;
	mAdjustedPictureLoc[3] = destBL;
	mLambda = getPerspectiveTransform(mOrgPictureLoc, mAdjustedPictureLoc);
	mLambdaInv = getPerspectiveTransform(mAdjustedPictureLoc, mOrgPictureLoc);
	mCanAdjustPerspective = true;
	mTopY = mAdjustedPictureLoc[0].y;
	mOrgPictureLoc[0] = Point2f(orgTL.x,mAdjHeadY);
	mOrgPictureLoc[1] = Point2f(orgTR.x,mAdjHeadY);
	mOrgPictureLoc[2] = orgTR;
	mOrgPictureLoc[3] = orgTL;
	mAdjustedPictureLoc[0] = Point2f(destTL.x,mOrgHeadY);
	mAdjustedPictureLoc[1] = Point2f(destTR.x,mOrgHeadY);
	mAdjustedPictureLoc[2] = destTR;
	mAdjustedPictureLoc[3] = destTL;
	mTopLambda = getPerspectiveTransform(mOrgPictureLoc, mAdjustedPictureLoc);
}

void perspective::calcPerspectiveLambda(Point2f orgTL, Point2f orgTR, Point2f orgBR, Point2f orgBL, Point2f rwTL, Point2f rwTR, Point2f rwBR, Point2f rwBL) {
	calcPerspectiveLambda(0, 0, orgTL, orgTR, orgBR, orgBL, rwTL, rwTR, rwBR, rwBL);
}
void perspective::calcPerspectiveLambda(int imgW, int imgH, Point2f orgTL, Point2f orgTR, Point2f orgBR, Point2f orgBL, Point2f rwTL, Point2f rwTR, Point2f rwBR, Point2f rwBL) {
	orgTL = doRotation(orgTL);
	orgBL = doRotation(orgBL);
	orgTR = doRotation(orgTR);
	orgBR = doRotation(orgBR);
	
	int rectL = (orgTL.x < orgBL.x) ? orgTL.x : orgBL.x;
	int rectR = (orgTR.x > orgBR.x) ? orgTR.x : orgBR.x;
	int rectT = (orgTL.y < orgTR.y) ? orgTL.y : orgTR.y;
	int rectB = (orgBL.y > orgBR.y) ? orgBL.y : orgBR.y;
	float deltaXmm = ((rwTR.x - rwTL.x) + (rwBR.x - rwBL.x)) / 2;
	float deltaYmm = ((rwTR.y - rwBR.y) + (rwTL.y - rwBL.y)) / 2;
	float deltaXpxl = rectR - rectL;
	float deltaYpxl = rectB - rectT;
	// deltaXpxl / deltaXmm =  deltaYpxl / delatYmm
	float deltaDeltaYpxl = ((deltaYmm * deltaXpxl) / deltaXmm) - deltaYpxl;
	rectB += deltaDeltaYpxl / 2;
	rectT -= deltaDeltaYpxl / 2;
	if (imgW != 0 && imgH != 0) {
		// constrained
		if ((rectB - rectT) > imgH) {
			// too big to fit
			rectT = 0;
			rectB = imgH;
		}
		else {
			if (rectT < 0) {
				rectB += rectT;
				rectT = 0;
			}
		}
	}
	calcPerspectiveLambda_(orgTL, orgTR, orgBR, orgBL, Point2f(rectL, rectT), Point2f(rectR, rectT), Point2f(rectR, rectB), Point2f(rectL, rectB));
}

void perspective::warp(Mat srce, Mat dest, bool warpAnyway) {
	if (mCanAdjustPerspective && (mPerspectiveAdjusted || warpAnyway)) {
		if (rotate) {
			Point2f centre(srce.cols / 2, srce.rows / 2);
			double angle180 = 180;
			double scale = 1.0;
			Mat rotMat = getRotationMatrix2D(centre, angle180, scale);
			Mat rotated;
			srce.copyTo(rotated);
			warpAffine(srce, rotated, rotMat, srce.size());
			//rotated.copyTo(dest);
			warpPerspective(rotated, dest, mLambda, srce.size());
		}
		else {
			if (mPartitionedWarp) {
				Mat tmp;
				Mat tmp2;
				int bottomY = srce.rows - 1;
//				warpPerspective(srce.rowRange(topY, bottomY), dest.rowRange(topY, bottomY), mLambda, srce.rowRange(topY, bottomY).size());
				warpPerspective(srce, tmp, mLambda, srce.size());
				srce.copyTo(dest);
				warpPerspective(srce, tmp2, mTopLambda, srce.size());
				tmp2.rowRange(0, mTopY).copyTo(tmp.rowRange(0, mTopY));
				tmp.copyTo(dest);
				//warpPerspective(srce, dest, mTopLambda, srce.size());
			}
			else {
				warpPerspective(srce, dest, mLambda, srce.size());
			}
		}
	}
	else {
		srce.copyTo(dest);
	}
}


// transfom between adjusted and un-adjusted pixel locations (inverse == true or false)
Point2f perspective::adjustedPerspective(Point2f inputPt, bool inverse, bool doItAnyway) {
	Point2f adjustedLoc;
	if (mCanAdjustPerspective && (mPerspectiveAdjusted || doItAnyway)) {
		vector<Point2f> vTempIn;
		vector<Point2f> vTempOut;
		vTempIn.push_back(doRotation( inputPt));
		vTempIn.push_back(Point2f(inputPt.x,0));
		vTempIn.push_back(Point2f(0,inputPt.y));
		vTempIn.push_back(Point2f(0,0));
		if (inverse) {
			perspectiveTransform(vTempIn, vTempOut, mLambdaInv);
		}
		else {
			perspectiveTransform(vTempIn, vTempOut, mLambda);
		}
		//adjustedLoc = Point2f(vTempOut[0].x + vTempOut[2].x, vTempOut[0].y + vTempOut[1].y);
		adjustedLoc = vTempOut[0];
	}
	else {
		return inputPt;
	}
	return adjustedLoc;
}

void perspective::adjustedPerspective(vector<Point2f> &vTempIn, vector<Point2f> &vTempOut, bool inverse, bool doItAnyway) {
	if (mCanAdjustPerspective && (mPerspectiveAdjusted || doItAnyway)) {
		if (inverse) {
			perspectiveTransform(vTempIn, vTempOut, mLambdaInv);
		}
		else {
			perspectiveTransform(vTempIn, vTempOut, mLambda);
		}
	}
	else {
		vTempOut.clear();
		for each(Point2f pt in vTempIn) {
			vTempOut.push_back(pt);
		}
	}
}
void perspective::calcRWPlane() {
	// assume plane is X-Y , so Z = 0
	// also assumes that the corected area is a rectangle
	// points numbered from top left, clockwise
	int i = 0;
	int j = 1;
	float dydX01 = (mRWloc[1].y - mRWloc[0].y) / (mAdjustedPictureLoc[1].x - mAdjustedPictureLoc[0].x);
	float dydX32 = (mRWloc[2].y - mRWloc[3].y) / (mAdjustedPictureLoc[2].x - mAdjustedPictureLoc[3].x);
	mdydX = (dydX01 + dydX32) / 2;
	float dxdY03 = (mRWloc[3].x - mRWloc[0].x) / (mAdjustedPictureLoc[3].y - mAdjustedPictureLoc[0].y);
	float dxdY12 = (mRWloc[2].x - mRWloc[1].x) / (mAdjustedPictureLoc[2].y - mAdjustedPictureLoc[1].y);
	mdxdY = (dxdY03 + dxdY12) / 2;
	float dxdX01 = (mRWloc[1].x - mRWloc[0].x) / (mAdjustedPictureLoc[1].x - mAdjustedPictureLoc[0].x);
	float dxdX32 = (mRWloc[2].x - mRWloc[3].x) / (mAdjustedPictureLoc[2].x - mAdjustedPictureLoc[3].x);
	mdxdX = (dxdX01 + dxdX32) / 2;
	float dydY03 = (mRWloc[3].y - mRWloc[0].y) / (mAdjustedPictureLoc[3].y - mAdjustedPictureLoc[0].y);
	float dydY12 = (mRWloc[2].y - mRWloc[1].y) / (mAdjustedPictureLoc[2].y - mAdjustedPictureLoc[1].y);
	mdydY = (dydY03 + dydY12) / 2;
	mOrgx = 0;
	mOrgy = 0;
	for (int idx = 0; idx < 4; idx++) {
		mOrgx += (mRWloc[idx].x - mdxdX * mAdjustedPictureLoc[idx].x - mdxdY * mAdjustedPictureLoc[idx].y) / 4;
		mOrgy += (mRWloc[idx].y - mdydY * mAdjustedPictureLoc[idx].y - mdydX * mAdjustedPictureLoc[idx].x) / 4;
	}
}

// take a pixel location within the adjusted image and return the realworld point
Point2f perspective::getRWloc(Point2f adjustedPictureLoc) {
	float x = mdxdX * adjustedPictureLoc.x + mdxdY * adjustedPictureLoc.y;
	float y = mdydY * adjustedPictureLoc.y + mdydX * adjustedPictureLoc.x;
	return Point2f(mOrgx + x, mOrgy + y);
}
// take a pixel location within the UN-adjusted image, adjust it, then and return the realworld point
Point2f perspective::getAdjustedRWloc(Point2f pictureLoc) {
	return getRWloc(adjustedPerspective(pictureLoc));
}

// take a RealWorld location and return the pixel location on the ADJUSTED image
Point2f perspective::getAdjustedPictureloc(Point2f rwLoc) {
	float X = (rwLoc.x - mOrgx) / mdxdX;
	float Y = (rwLoc.y - mOrgy) / mdydY;
	X -= Y * mdxdY;
	Y -= X * mdydX;
	return Point2f(X, Y);
}


