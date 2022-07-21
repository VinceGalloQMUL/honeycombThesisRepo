#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;
using namespace System;

class zone3d
{
public:
	enum zone3dType {subZone, compoundZone};
	enum zoneShape { circle, square, triangle, Dleft, Dright };
	int mUserDefinedType;
	int mID;
	zone3d *mParent;
	float mRadius;
	float mRadiusSqr;
	float mLimitZ;
	bool mExclusion;
	zoneShape mShape;
	char *mName;
	Point3f Centre();
	void setGeometry(Point3f wallCentre, float rad, float limitZ);
	void setCentre(Point3f wallCentre);
	void setSize(float rad, float limitZ);
	zone3d(zone3d *parent, zoneShape shape, int userDefinedType, Point3f wallCentre, float rad, float limitZ, char *name, int id);
	bool isSubZone() { return mType == subZone;  }
	float distanceSqr(Point3f toWhere, bool toFace);
	bool isWithin(float x, float y, float z, float tolerance = 1.0);
//	bool isWithin(Point3f testLoc, float tolerance = 1.0);
	bool isWithin(Point2f testLoc, float tolerance = 1.0);
	char *getName() { return mName; }
	void setName(const char *name);
	string *serialise();
	bool deSerialise(vector<string> args, zone3d *parent, int startingOffset);
	vector<Point2f> *getCorners();
	float getAngleDeg();
	float getAngleRad();
	void setAngleDeg(float angleDeg);
	float zone3d::angleFromCentreRadFromCartesianXY(float x, float y);
	float zone3d::angleFromCentreDegFromCartesianXY(float x, float y);
	static float zone3d::angleRadFromCartesianXY(float x, float y);
	static float zone3d::angleRadFromCartesianXY(Point3f loc);
	static float zone3d::angleDegFromCartesianXY(float x, float y);
	static float zone3d::angleDegFromCartesianXY(Point3f loc);
	static float zone3d::angleDegMod360(float rawAngle);
	static bool zone3d::angleDegIsBetween(float testAngle, float minAngle, float deltaToMaxAngle0to360);
	~zone3d();
	zone3d();
	zone3d *duplicate(zone3d *parent, const char *name, int id);
protected:
	float mAngle;
	Point3f mCentreOffset;
	zone3dType mType;

private:
	float axialDeltaSq(Point3f toWhere);
	float zone3d::axialDeltaSq(float x, float y, float z);
	void zone3d::getCorner(int cornerIdx, float *x, float *y, float tolerance = 1.0);
	Point3f getCorner(int cornerIdx, float tolerance = 1.0);
	void rotate(float relativeToCentreX, float relativeToCentreY, float angle, float *resX, float *resY);
	Point3f rotate(Point3f relativeToCentre, float angle);
	Point3f rotateAroundCentre(Point3f absLoc, float angle);

};

class zone3dCompound : public zone3d {
	vector<zone3d *> mSubZones;
public:
	bool hasSubZones();
	bool isWithinSubZones(float x, float y, float z, float tolerance = 1.0);
	bool isWithin(float x, float y, float z, float tolerance = 1.0);
	bool isWithinIgnoreExcludes(float x, float y, float z, float tolerance = 1.0);
	void addZone(zone3d *additionalZone);
	bool contains(char *zoneName);
	char *zone3dCompound::generateDuplicateName(const char *oldName);
	zone3d *findSubZone(char *zoneName);
	bool removeZone(char *zoneName);
	bool removeZone(int szIdx);
	vector<zone3d *> *subZones();
	vector<char *> *subZoneNames();
	int highestID();
	void zone3dCompound::clearAndDelete();
	zone3dCompound(zoneShape shape, int userDefinedType, Point3f wallCentre, float rad, float limitZ, char *name, int id);
	zone3dCompound();
	~zone3dCompound();
	zone3dCompound *duplicate(const char *name, int *id);
	string *serialise();
	int deSerialise(int subZonesNeeded, vector<string> args); // returns -1 if fails, otherwise the number of subzones that are to follow
private:
	int findSubZoneIdx(char *zoneName);

};
