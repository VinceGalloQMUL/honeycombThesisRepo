#pragma once
#include <string>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "CVcommon.h"

class beeTagLoc
{
public:
	float mFTime;
	int mID;
	vector2f mPtCentre;
	float mAngle;

	string toStr();
	bool parse(std::vector<std::string>   lineArray);
	beeTagLoc();
	beeTagLoc(float ft, int id,vector2f ptCentre, float angle);
	~beeTagLoc();
};

