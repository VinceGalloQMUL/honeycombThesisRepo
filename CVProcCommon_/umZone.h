#pragma once
#include <string>
#include "CVcommon.h"

class umZone
{
public:
	int mCentreX;
	int mCentreY;
	int mRadiusOuter;
	int mRadiusInner;
	int mRadOuterSqr;
	int mRadInnerSqr;
	bool mIsCircle;
	char *mName;
	void setGeometry(int midX, int midY, int radInc, int radExc);
	void setRadius(int radInc, int radExc);
	umZone();
	umZone(int midX, int midY, int radInc, int radExc, char *name);
	int umZone::distanceSqr(int locX, int locY);
	bool isWithin(int locX, int locY, bool useInner);
	bool isTowards(int locX, int locY, float direction);
	char *getName();
	void setName(char *name);
	~umZone();
};

