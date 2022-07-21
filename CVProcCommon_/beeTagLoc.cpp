#include "csvFile.h"
#include "beeTagLoc.h"



beeTagLoc::beeTagLoc()
{
}

beeTagLoc::beeTagLoc(float ft, int id, vector2f ptCentre, float angle)
{
	mFTime = ft;
	mID = id;
	mPtCentre = ptCentre;
	mAngle = angle;
}

#define BEETAGLOC_REQUIRED_FIELD_QTY 6

string beeTagLoc::toStr() 
{
	stringstream ss;
	ss << mFTime << "," << mID << ","  << mPtCentre.toStr() << "," << mAngle;
	return ss.str();
}
bool beeTagLoc::parse(std::vector<std::string>   lineArray) {
	bool retv = false;
	int lineIdx = 0;
	if (lineArray.size() >= BEETAGLOC_REQUIRED_FIELD_QTY) {
		mFTime = csvFile::parseLine1fSafely(lineArray[lineIdx++], 0);
		mID = csvFile::parseLine1iSafely(lineArray[lineIdx++], -1);
		float X = csvFile::parseLine1fSafely(lineArray[lineIdx++], 0);
		float Y = csvFile::parseLine1fSafely(lineArray[lineIdx++], 0);
		mPtCentre.set(X, Y);
		mAngle = csvFile::parseLine1fSafely(lineArray[lineIdx++], 0);
	}
	return retv;
}

beeTagLoc::~beeTagLoc()
{
}

