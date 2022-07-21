#include "areaDesc.h"

areaDesc::areaDesc() {
    emptyEntry();
}
void areaDesc::clone(areaDesc& peer) {
	mType = peer.mType;
	mArea = peer.mArea;
    mShape = peer.mShape;
	mDrone = peer.mDrone;
	mDelta = peer.mDelta;
    pt0 = peer.pt0;
    pt1 = peer.pt1;
    pt2 = peer.pt2;
    pt3 = peer.pt3;
	linearCellGapMultiplier = peer.linearCellGapMultiplier;
}
std::string areaDesc::serialise() {
    std::stringstream ss;
    ss << mShape << "," << ((mDelta) ? ((mDrone) ? (mType + BROODIDX_OFFSET_DRONE + BROODIDX_OFFSET_DELTA) : (mType + BROODIDX_OFFSET_DELTA)) : ((mDrone) ? (mType + BROODIDX_OFFSET_DRONE) : mType ));
    ss << "," << pt0.x << "," << pt0.y;
    ss << "," << pt1.x << "," << pt1.y;
	if (mShape == shape::line) {
		ss << "," << linearCellGapMultiplier << ",0";
	}
	else {
		ss << "," << pt2.x << "," << pt2.y;
	}
	ss << "," << pt3.x << "," << pt3.y;
    return ss.str();
}
Point resizePointNZ(Point2f inputLoc, perspective persp, float dispDivision) {
	Point2f retv = Point2f(((inputLoc.x)) / dispDivision, ((inputLoc.y)) / dispDivision);
	if (inputLoc.x > 0 && inputLoc.y > 0) {
		retv = persp.adjustedPerspective(retv, false, true);

	}
	return retv;
}

bool areaDesc::deserialise(std::vector<std::string>& valStrArray) {
	bool retv = false;
	if (valStrArray.size() == 10) {
		int idx = 0;
		mShape = (shape)stoi(valStrArray[idx++]);
		mType = (shape)stoi(valStrArray[idx++]);
		if (mType >= BROODIDX_OFFSET_DELTA) {
			mDelta = true;
			mType -= BROODIDX_OFFSET_DELTA;
		}
		if (mType >= BROODIDX_OFFSET_DRONE) {
			mDrone = true;
			mType -= BROODIDX_OFFSET_DRONE;
		}
		pt0 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
		idx += 2;
		pt1 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
		idx += 2;
		if (mShape == shape::line) {
			pt2 = Point2f(0,0);
			linearCellGapMultiplier = stof(valStrArray[idx]);
			if (linearCellGapMultiplier < 0.5f)
				linearCellGapMultiplier = 1;
		}
		else {
			pt2 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
		}
		idx += 2;
		pt3 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
		idx += 2;
		retv = true;
	}
	return retv;
}

std::string areaDesc::serialiseAdjusted(perspective persp, float dispDivision, int cellQty, Point ref0, Point refH, Point refD, int pxlsH, int pxlsV) {
    std::stringstream ss;
	mArea = calcArea(persp, dispDivision, pxlsH, pxlsV);
    ss << mShape << "," << (mDelta ? mType + 40 : mType);
    ss << "," << Zpt0.x << "," << Zpt0.y;
	ss << "," << Zpt1.x << "," << Zpt1.y;
	ss << "," << Zpt2.x << "," << Zpt2.y;
	ss << "," << Zpt3.x << "," << Zpt3.y;
	//Point Zref0 = resizePointNZ(ref0, persp, dispDivision);
	//Point ZrefH = resizePointNZ(refH, persp, dispDivision);
	//Point ZrefD = resizePointNZ(refD, persp, dispDivision);
	ss << "," << mArea;
	return ss.str();

}


int areaDesc::calcArea(perspective persp, float dispDivision, int pxlsH, int pxlsV) {
	Zpt0 = resizePointNZ(pt0, persp, dispDivision);
	Zpt1 = resizePointNZ(pt1, persp, dispDivision);
	Zpt2 = resizePointNZ(pt2, persp, dispDivision);
	Zpt3 = resizePointNZ(pt3, persp, dispDivision);
	float pixelsPerCellHor = pxlsH / persp.fullImageCols; // NUMCELLSWIDTH;
	float pixelsPerCellHeight = pxlsV / persp.fullImageRows; // NUMCELLSHEIGHTBROOD;
	int retv = 0;
	switch (mShape) {
	case areaDesc::shape::point:
			retv = 1;
			break;
	case areaDesc::shape::line:
			retv = calcAreaLine(Zpt0, Zpt1, pixelsPerCellHor, pixelsPerCellHeight);
			break;
		case areaDesc::shape::tri:
			retv = calcAreaTri(Zpt0, Zpt1, Zpt2, pixelsPerCellHor, pixelsPerCellHeight);
			break;
		case areaDesc::shape::quad:
			retv = calcAreaQuad(Zpt0, Zpt1, Zpt2, Zpt3, pixelsPerCellHor, pixelsPerCellHeight);
			break;
	}
	return retv;
}

int areaDesc::calcArea(perspective persp, float dispDivision, int pxlsH, int pxlsV, std::vector<Point2f>& outPtArray) {
	Zpt0 = resizePointNZ(pt0, persp, dispDivision);
	Zpt1 = resizePointNZ(pt1, persp, dispDivision);
	Zpt2 = resizePointNZ(pt2, persp, dispDivision);
	Zpt3 = resizePointNZ(pt3, persp, dispDivision);
	float pixelsPerCellHor = pxlsH / persp.fullImageCols; // NUMCELLSWIDTH;
	float pixelsPerCellHeight = pxlsV / persp.fullImageRows; // NUMCELLSHEIGHTBROOD;
	int retv = 0;
	switch (mShape) {
	case areaDesc::shape::point:
		retv = 1;
		break;
	case areaDesc::shape::line:
		retv = calcAreaLine(Zpt0, Zpt1, pixelsPerCellHor, pixelsPerCellHeight, outPtArray);
		break;
	case areaDesc::shape::tri:
		retv = calcAreaTri(Zpt0, Zpt1, Zpt2, pixelsPerCellHor, pixelsPerCellHeight);
		break;
	case areaDesc::shape::quad:
		retv = calcAreaQuad(Zpt0, Zpt1, Zpt2, Zpt3, pixelsPerCellHor, pixelsPerCellHeight);
		break;
	}
	return retv;
}

float areaDesc::calcLengthCells(Point pt0, Point pt1, bool isDrone, float pixelsPerCellHor, float pixelsPerCellHeight, float linearMultiplier) {
	float retv = 0;
	float celsW = calcLengthWidth(pt0, pt1) / pixelsPerCellHor;
	if (isDrone) celsW = celsW / 1.3f;
	float celsH = calcLengthHeight(pt0, pt1) / pixelsPerCellHeight;
	if (isDrone) celsH = celsH / 1.3f;
	retv = Math::Sqrt(celsH * celsH + celsW * celsW);
	retv *= linearMultiplier;
	return retv;
}
int areaDesc::calcAreaLine(Point pt0, Point pt1, float pixelsPerCellHor, float pixelsPerCellHeight, std::vector<Point2f>& outPtArray) {
	int retv = 0;
	float lineLen = calcLengthCells(pt0, pt1, mDrone, pixelsPerCellHor, pixelsPerCellHeight, linearCellGapMultiplier);
	retv = (int)(FENCEPOSTROUNDING + lineLen);
	int cells = ((int)(FENCEPOSTROUNDING + lineLen) - 1);
	if (cells > 0) {
		Point2f cellDelta = (pt1 - pt0);
		cellDelta /= cells;
		for (int idx = 0; idx < retv; idx++) {
			Point2f timesDelta = idx * cellDelta;
			timesDelta = timesDelta + (Point2f)pt0;
			outPtArray.push_back(timesDelta);
		}

	}
	return retv;
}

int areaDesc::calcAreaLine(Point pt0, Point pt1, float pixelsPerCellHor, float pixelsPerCellHeight) {
	int retv = 0;
	float lineLen = calcLengthCells(pt0, pt1, mDrone, pixelsPerCellHor, pixelsPerCellHeight, linearCellGapMultiplier);
	retv = (int)(FENCEPOSTROUNDING + lineLen);
	//if (celsW > 0) {
	//	float ratio = celsH / celsW;
	//	if (ratio > 1.4f) {
	//		retv = (int)(1.5f + (celsH));

	//	}
	//	else if (ratio > 0.667f) {
	//		//celsW -= celsH / 2;
	//		retv = (int)(1.5f + (Math::Sqrt((celsH * celsH) + (celsW * celsW))));

	//	}
	//	else {
	//		retv = (int)(1.5f + (Math::Sqrt((celsH * celsH * 2) + celsW * celsW)));

	//	}
	//}
	return retv;
}
int areaDesc::calcAreaTri(Point pt0, Point pt1, Point pt2, float pixelsPerCellHor, float pixelsPerCellHeight, std::vector<Point2f>& outPtArray) {
	return calcAreaTri(pt0, pt1, pt2, pixelsPerCellHor, pixelsPerCellHeight);
}

int areaDesc::calcAreaTri(Point pt0, Point pt1, Point pt2, float pixelsPerCellHor, float pixelsPerCellHeight) {
	int retv = 0;
	float lineLen0 = calcLengthCells(pt0, pt1, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	float lineLen1 = calcLengthCells(pt0, pt2, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	float lineLen2 = calcLengthCells(pt1, pt2, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	// regular layout, so should all be the same...so take average
	lineLen0 = (int)(FENCEPOSTROUNDING + (lineLen0 + lineLen1 + lineLen2) / 3);
	retv = lineLen0 * (lineLen0 + 1) / 2;
	return retv;
}
int areaDesc::calcAreaQuad(Point pt0, Point pt1, Point pt2, Point pt3, float pixelsPerCellHor, float pixelsPerCellHeight, std::vector<Point2f>& outPtArray) {
	return calcAreaQuad( pt0,  pt1,  pt2,  pt3,  pixelsPerCellHor,  pixelsPerCellHeight);
}
int areaDesc::calcAreaQuad(Point pt0, Point pt1, Point pt2, Point pt3, float pixelsPerCellHor, float pixelsPerCellHeight) {
	float lineLen0 = calcLengthCells(pt0, pt1, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	float lineLen1 = calcLengthCells(pt1, pt2, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	float lineLen2 = calcLengthCells(pt2, pt3, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	float lineLen3 = calcLengthCells(pt3, pt0, mDrone, pixelsPerCellHor, pixelsPerCellHeight, 1);
	// regular layout, so should oposites be the same...so take average
	lineLen0 = (lineLen0 + lineLen2) / 2;
	lineLen1 = (lineLen1 + lineLen3) / 2;
	int w = (int)(FENCEPOSTROUNDING +lineLen0 );
	int h = (int)(FENCEPOSTROUNDING +lineLen1);
	int retv = w * h;
	return retv;
}

float areaDesc::calcGradient(Point pt0, Point pt1) {
	float deltaX = pt1.x - pt0.x;
	float deltaY = pt1.y - pt0.y;

	return Math::Abs(deltaY / deltaX);
}

float areaDesc::calcLength(Point pt0, Point pt1) {
	float deltaX = pt1.x - pt0.x;
	float deltaY = pt1.y - pt0.y;

	return Math::Sqrt(deltaY * deltaY + deltaX * deltaX);
}
float areaDesc::calcLengthWidth(Point pt0, Point pt1) {
	return Math::Abs(pt1.x - pt0.x);
}
float areaDesc::calcLengthHeight(Point pt0, Point pt1) {
	return Math::Abs(pt1.y - pt0.y);
}

bool areaDesc::isHorizontal(Point pt0, Point pt1, float gradH, float gradD) {
	float tmpGrad = calcGradient(pt0, pt1);

	float deltaH = Math::Abs(tmpGrad - gradH);
	float deltaD = Math::Abs(tmpGrad - gradD);
	return deltaD > deltaH;
}

bool areaDesc::isEmpty(Point pt) {
    return pt.x == 0 && pt.y == 0;
}
bool areaDesc::isEmpty() {
    return isEmpty(pt0);
}
void areaDesc::emptyEntry() {
    mShape = shape::point;
	mType = 0;
	mDrone = false;
	mDelta = false;
    pt0 = Point(0, 0);
    pt1 = Point(0, 0);
	pt2 = Point(0, 0);
	pt3 = Point(0, 0);
	linearCellGapMultiplier = 1;
}
