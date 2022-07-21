#include "umZone.h"



umZone::umZone()
{
	mIsCircle = true;
	mName = NULL;
}

umZone::umZone(int midX, int midY, int radInc, int radExc, char *name)
{
	mCentreX = midX;
	mCentreY = midY;
	mRadiusOuter = radInc;
	mRadiusInner = radExc;
	mRadInnerSqr = radExc * radExc;
	mRadOuterSqr = radInc * radInc;
	mIsCircle = true;
	int tmpLen = strlen(name) + 1;
	mName = new char[tmpLen];
	strncpy(mName, name, tmpLen);
}

umZone::~umZone()
{
	if (mName != NULL) delete mName;
}
void umZone::setGeometry(int midX, int midY, int radInc, int radExc) {
	mCentreX = midX;
	mCentreY = midY;
	mRadiusOuter = radInc;
	mRadiusInner = radExc;
	mRadInnerSqr = radExc * radExc;
	mRadOuterSqr = radInc * radInc;
}
void umZone::setRadius(int radInc, int radExc) {
	mRadiusOuter = radInc;
	mRadiusInner = radExc;
	mRadInnerSqr = radExc * radExc;
	mRadOuterSqr = radInc * radInc;
}
char *umZone::getName() {
	return mName;
}
void umZone::setName(char *name) {
	mName = name;
}
int umZone::distanceSqr(int locX, int locY) {
	int deltaX = locX - mCentreX;
	int deltaY = locY - mCentreY;
	return deltaX * deltaX + deltaY * deltaY;
}
bool umZone::isWithin(int locX, int locY, bool useInner) {
	int distSqr = distanceSqr(locX, locY);
	return (useInner) ? (mRadInnerSqr >= distSqr) : (mRadOuterSqr >= distSqr);
}
bool umZone::isTowards(int locX, int locY, float direction) {
	bool retv = false;
	vector2f from(locX, locY);
	vector2f toCentre(locX, locY, mCentreX, mCentreY);
	float angleToCentre = toCentre.directionDegXY();
	float distanceToCentre = toCentre.magnitude();
	float angleToTangent = (angleToCentre + 90) * Math::PI / 180.0;
	float cos = Math::Cos(angleToTangent);
	float sin = Math::Sin(angleToTangent);
	vector2f deltaEdge(Math::Cos(angleToTangent) * mRadiusOuter, - Math::Sin(angleToTangent) * mRadiusOuter);
	vector2f toTopEdge(mCentreX, mCentreY);
	vector2f toBottomEdge(mCentreX, mCentreY);
	toTopEdge.add(deltaEdge);
	toBottomEdge.minus(deltaEdge);
	toTopEdge.minus(from);
	toBottomEdge.minus(from);
	float angleToTopEdge = toTopEdge.directionDegXY();
	float angleToBottomEdge = toBottomEdge.directionDegXY();
	float angleBottomToTopEdge = angleToTopEdge - angleToBottomEdge;
	float angleBottomToDirection = direction - angleToBottomEdge;
	angleBottomToTopEdge = (angleBottomToTopEdge < 0)? 360 + angleBottomToTopEdge : angleBottomToTopEdge;
	angleBottomToDirection = (angleBottomToDirection < 0)? 360 + angleBottomToDirection : angleBottomToDirection;
	if (angleBottomToTopEdge < 180) {
		retv = angleBottomToDirection < angleBottomToTopEdge;

	}
	else {
		retv = angleBottomToDirection > angleBottomToTopEdge;

	}
	return retv;
}

