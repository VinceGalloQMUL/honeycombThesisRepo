#include "zone3d.h"

#include "csvFile.h"

using namespace std;
using namespace System;

void zone3d::setGeometry(Point3f wallCentre, float rad, float limitZ) {
	setCentre(wallCentre);
	mRadius = rad;
	mRadiusSqr = rad * rad;
	mLimitZ = limitZ;
}
void zone3d::setCentre(Point3f wallCentre) {
	if (mParent != nullptr) {
		mCentreOffset = rotate(wallCentre - mParent->Centre(), -( mParent->mAngle));
	}
	else {
		mCentreOffset = wallCentre;
	}
}
void zone3d::setSize(float rad, float limitZ) {
	mRadius = rad;
	mRadiusSqr = rad * rad;
	mLimitZ = limitZ;
}

zone3d::zone3d() : mName(nullptr)
{
	mExclusion = false;

	mAngle = 0;
	mShape = zoneShape::circle;
	mUserDefinedType = 0;
	mType = subZone;
	mParent = nullptr;
	mID = 0;
}


zone3d::~zone3d()
{
	if (mName != nullptr) {
		delete mName;
	}

}

zone3d::zone3d(zone3d *parent, zoneShape shape, int userDefinedType, Point3f wallCentre, float rad, float limitZ, char *name, int id) : mName(nullptr)
{
	mExclusion = false;

	mAngle = 0;
	mShape = shape;
	mUserDefinedType = userDefinedType;
	mType = subZone;
	mParent = parent;
	setGeometry(wallCentre, rad, limitZ);
	setName(name);
	mID = id;
}

float zone3d::distanceSqr(Point3f toWhere, bool toFace) {
	Point3f tmpCentre = Centre();
	Point3f delta = toWhere - tmpCentre;
	if (!toFace) {
		delta = toWhere - Point3f(tmpCentre.x, tmpCentre.y, tmpCentre.z + mLimitZ / 2);
	}
	return delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
}
float zone3d::axialDeltaSq(Point3f toWhere) {
	Point3f delta = toWhere - Centre();
	return delta.x * delta.x + delta.y * delta.y;

}

float zone3d::axialDeltaSq(float x, float y, float z) {
	x -= Centre().x;
	y -= Centre().y;
	return x * x + y * y;

}

Point3f zone3d::Centre() {
	if (mParent != nullptr) {
		return rotate(mCentreOffset, mParent->mAngle) + mParent->Centre();
	}
	else {
		return mCentreOffset;
	}
}
float zone3d::getAngleDeg() {
	return getAngleRad() * 180 / Math::PI;
}
float zone3d::getAngleRad() {
	float parentAngle = 0;
	if (mParent != nullptr) {
		parentAngle = mParent->mAngle;
	}
	float retv = (mAngle + parentAngle);
	if (retv < -Math::PI) retv += 2 * Math::PI;
	if (retv > Math::PI) retv -= 2 * Math::PI;
	return retv;
}
void zone3d::setAngleDeg(float angleDeg) {
	float parentAngle = 0;
	if (mParent != nullptr) {
		parentAngle = mParent->getAngleRad();
	}
	mAngle = (angleDeg * Math::PI / 180) - parentAngle;
	if (mAngle < -Math::PI) mAngle += 2 * Math::PI;
	if (mAngle > Math::PI) mAngle -= 2 * Math::PI;
}

void zone3d::rotate(float relativeToCentreX, float relativeToCentreY, float angle, float *resX, float *resY) {
	float initAngle = angleRadFromCartesianXY(relativeToCentreX, relativeToCentreY);
	float hypotenuse = Math::Sqrt(relativeToCentreX * relativeToCentreX + relativeToCentreY * relativeToCentreY);
	initAngle += angle;
	*resX = hypotenuse * Math::Cos(initAngle);
	*resY = hypotenuse * Math::Sin(initAngle);

}
Point3f zone3d::rotate(Point3f relativeToCentre, float angle) {
	float initAngle = angleRadFromCartesianXY(relativeToCentre);
	float hypotenuse = Math::Sqrt(relativeToCentre.x * relativeToCentre.x + relativeToCentre.y * relativeToCentre.y);
	initAngle += angle;
	return Point3f(hypotenuse * Math::Cos(initAngle), hypotenuse * Math::Sin(initAngle), relativeToCentre.z);
}
float zone3d::angleFromCentreRadFromCartesianXY(float x, float y) {
	return angleRadFromCartesianXY(x - Centre().x, y - Centre().y);
}
float zone3d::angleFromCentreDegFromCartesianXY(float x, float y) {
	return angleFromCentreRadFromCartesianXY(x, y) * 180 / Math::PI;
}

Point3f zone3d::rotateAroundCentre(Point3f absLoc, float angle) {
	Point3f relativeToCentre = absLoc - Centre();
	return Centre() + rotate(relativeToCentre, angle);
}

float zone3d::angleDegFromCartesianXY(Point3f loc) {
	return angleRadFromCartesianXY(loc) * 180 / Math::PI;
}
float zone3d::angleDegFromCartesianXY(float x, float y) {
	return angleRadFromCartesianXY(x, y) * 180 / Math::PI;
}

float zone3d::angleRadFromCartesianXY(Point3f loc) {
	return angleRadFromCartesianXY(loc.x, loc.y);
}
float zone3d::angleRadFromCartesianXY(float x, float y) {
	float retv = 0;
	if (x == 0) {
		retv = (y > 0) ? Math::PI / 2 : 3 * Math::PI / 2;
	}
	else {
		retv = Math::Atan(Math::Abs(y) / Math::Abs(x));
		if (x > 0) { // TR
			if (y < 0) { // BR
				retv = 2 * Math::PI - retv;
			}
		}
		else {
			if (y < 0) { // BL
				retv = Math::PI + retv;
			}
			else { // TL
				retv = Math::PI - retv;
			}
		}
	}
	return retv;
}

float zone3d::angleDegMod360(float rawAngle) {
	float retv = rawAngle;
	if (rawAngle >= 0) {
		while (retv >= 360) {
			retv -= 360;
		}
	}
	else {
		while (retv < 0) {
			retv += 360;
		}
	}
	return retv;
}
bool zone3d::angleDegIsBetween(float testAngle0to360, float minAngle0to360, float deltaToMaxAngle0to360) {
	float aboveMin = testAngle0to360 - minAngle0to360;
	return angleDegMod360(aboveMin) <= deltaToMaxAngle0to360;
}

Point3f zone3d::getCorner(int cornerIdx, float tolerance) {
	float retX, retY;
	getCorner(cornerIdx, &retX, &retY, tolerance);
	return Point3f(retX, retY, 0);
}
void zone3d::getCorner(int cornerIdx, float *x, float *y, float tolerance) {
	float tolerantRad = mRadius * tolerance;
	if (mShape == zone3d::square) {
		switch (cornerIdx) {
		case 0:
			*x = -tolerantRad; *y = tolerantRad;
			break;
		case 1:
			*x = tolerantRad; *y = tolerantRad;
			break;
		case 2:
			*x = tolerantRad; *y = -tolerantRad;
			break;
		case 3:
			*x = -tolerantRad; *y = -tolerantRad;
			break;
		}
	}
	else if (mShape == zone3d::triangle){
		switch (cornerIdx) {
		case 0:
			*x = 0; *y = tolerantRad;
			break;
		case 1:
			*x = tolerantRad * Math::Sin(60 * Math::PI / 180); *y = -tolerantRad * Math::Cos(60 * Math::PI / 180);
			break;
		case 2:
			*x = -tolerantRad * Math::Sin(60 * Math::PI / 180); *y = -tolerantRad * Math::Cos(60 * Math::PI / 180);
			break;
		}

	}
	else if (mShape == zone3d::Dleft || mShape == zone3d::Dright) {
		switch (cornerIdx) {
		case 0:
			*x = 0; *y = tolerantRad;
			break;
		case 1:
			*x = 0; *y = -tolerantRad;
			break;
		}

	}
}

vector<Point2f> *zone3d::getCorners() {
	vector<Point2f> *retv = nullptr;
	float myNetAngle = getAngleRad();
	if (mShape == zone3d::square) {
		retv = new vector<Point2f>();
		Point3f tmpCentre = Centre();
		Point3f tmpLoc = Centre() + rotate(getCorner(0), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
		tmpLoc = Centre() + rotate(getCorner(1), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
		tmpLoc = Centre() + rotate(getCorner(2), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
		tmpLoc = Centre() + rotate(getCorner(3), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
	} else if (mShape == zone3d::triangle) {
		retv = new vector<Point2f>();
		Point3f tmpCentre = Centre();
		Point3f tmpLoc = Centre() + rotate(getCorner(0), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
		tmpLoc = Centre() + rotate(getCorner(1), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
		tmpLoc = Centre() + rotate(getCorner(2), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
	} else if (mShape == zone3d::Dleft || mShape == zone3d::Dright) {
		retv = new vector<Point2f>();
		Point3f tmpCentre = Centre();
		Point3f tmpLoc = Centre() + rotate(getCorner(0), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
		tmpLoc = Centre() + rotate(getCorner(1), myNetAngle);
		retv->push_back(Point2f(tmpLoc.x, tmpLoc.y));
	}

	return retv;
}

bool zone3d::isWithin(float x, float y, float z, float tolerance) {
	bool retv = false;
	if ((z >= 0) && z <= mLimitZ) {
		if (mShape == circle) {
			if ((axialDeltaSq(x, y, z) <= mRadiusSqr)) {
				retv = true;
			}
		}
		else if (mShape == triangle) {
			float unrotatedX, unrotatedY;
			float cornerX, cornerY;
			rotate(x - Centre().x, y - Centre().y, -getAngleRad(), &unrotatedX, &unrotatedY);
			getCorner(0, &cornerX, &cornerY, tolerance);
			float angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
			if (angleToCorner > 60 && angleToCorner < 120) {
				getCorner(1, &cornerX, &cornerY, tolerance);
				angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
				if (angleToCorner > 300 && angleToCorner < 360) {
					getCorner(2, &cornerX, &cornerY, tolerance);
					angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
					if (angleToCorner > 180 && angleToCorner < 240) {
						retv = true;
					}
				}
			}
		}
		else if (mShape == square) {
			float unrotatedX, unrotatedY;
			float cornerX, cornerY;
			rotate(x - Centre().x, y - Centre().y, -getAngleRad(), &unrotatedX, &unrotatedY);
			getCorner(0, &cornerX, &cornerY, tolerance);
			float angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
			if (angleToCorner > 90 && angleToCorner < 180) {
				getCorner(1, &cornerX, &cornerY, tolerance);
				angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
				if (angleToCorner > 0 && angleToCorner < 90) {
					getCorner(2, &cornerX, &cornerY, tolerance);
					angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
					if (angleToCorner > 270 && angleToCorner < 360) {
						getCorner(3, &cornerX, &cornerY, tolerance);
						angleToCorner = angleDegFromCartesianXY(cornerX - unrotatedX, cornerY - unrotatedY);
						if (angleToCorner > 180 && angleToCorner < 270) {
							retv = true;
						}
						retv = true;
					}
				}
			}
		}
		else if (mShape == Dleft) {
			if ((axialDeltaSq(x, y, z) <= mRadiusSqr)) {
				float unrotatedX, unrotatedY;
				float cornerX, cornerY;
				rotate(x - Centre().x, y - Centre().y, -getAngleRad(), &unrotatedX, &unrotatedY);
				if (unrotatedX < 0) {
					retv = true;
				}
			}

		}
		else if (mShape == Dright) {
			if ((axialDeltaSq(x, y, z) <= mRadiusSqr)) {
				float unrotatedX, unrotatedY;
				float cornerX, cornerY;
				rotate(x - Centre().x, y - Centre().y, -getAngleRad(), &unrotatedX, &unrotatedY);
				if (unrotatedX > 0) {
					retv = true;
				}
			}

		}
	}
	return retv;
}

bool zone3d::isWithin(Point2f testLoc, float tolerance) {
	return isWithin(testLoc.x, testLoc.y, 0, tolerance);
}
void zone3d::setName(const char *name) { 
	if (mName != nullptr) {
		delete mName;
	}
	int len = strlen(name);
	mName = new char[len + 1];
	strcpy(mName, name);
	mName[len] = '\0';
}


zone3d *zone3d::duplicate(zone3d *parent, const char *name, int id) {
	zone3d * retv = new zone3d(parent, mShape, mUserDefinedType, Centre(), mRadius, mLimitZ, (char *)name, id);
	retv->mAngle = mAngle;

	return retv;
}


string *zone3d::serialise() {
	stringstream ss;

	int parentID = (mParent != nullptr) ? mParent->mID : -1;
	ss << mType << "," << mID << "," << parentID << "," << mUserDefinedType << "," << mRadius << "," << mRadiusSqr << "," << mLimitZ << "," << mAngle << "," << mShape << "," << mCentreOffset.x << "," << mCentreOffset.y << "," << mCentreOffset.z << "," << mName << "," << (mExclusion ? "1" : "0");
	string *retv = new string(ss.str());
	return retv;
}
bool zone3d::deSerialise(vector<string> args, zone3d *parent, int startOffset) {
	bool retv = false;
	if (args.size() >= startOffset + 13) {
		int idx = startOffset;
		mType = (zone3d::zone3dType)csvFile::parseLine1iSafely(args[idx++], 0);
		mID = csvFile::parseLine1iSafely(args[idx++], 0);
		mParent = parent; idx++; // csvFile::parseLine1iSafely(args[idx++], 0);
		mUserDefinedType = csvFile::parseLine1iSafely(args[idx++], 0);
		mRadius = csvFile::parseLine1fSafely(args[idx++], 0);
		mRadiusSqr = csvFile::parseLine1fSafely(args[idx++], mRadius * mRadius);
		mLimitZ = csvFile::parseLine1fSafely(args[idx++], 0);
		mAngle = csvFile::parseLine1fSafely(args[idx++], 0);
		mShape = (zone3d::zoneShape)csvFile::parseLine1iSafely(args[idx++], 0);
		float tmpX = csvFile::parseLine1fSafely(args[idx++], 0);
		float tmpY = csvFile::parseLine1fSafely(args[idx++], 0);
		float tmpZ = csvFile::parseLine1fSafely(args[idx++], 0);
		mCentreOffset = Point3f(tmpX, tmpY, tmpZ);
		setName(args[idx++].c_str());
		if (args.size() >= startOffset + 14) {
			mExclusion = csvFile::parseLine1fSafely(args[idx++], 0) > 0;
		}
		retv = true;
	}
	return retv;
}

bool zone3dCompound::isWithin(float x, float y, float z, float tolerance) {
	bool retv = zone3d::isWithin(x, y, z, tolerance);
	if (retv && mType == compoundZone) {
		if (mSubZones.size() > 0) {
			for each (zone3d *sz in mSubZones) {
				if (sz->mExclusion && sz->isWithin(x, y, z, tolerance)) { // test of ifWithin ignores z, otherwise the test here should be
													// sz->mExclusion && (sz->isWithin(x, y, 0, tolerance) || bsz->eyondZrange())
													// soo easier to just blitz the z component
					retv = false;
					break;
				}
			}
		}
	}
	return retv;
}

bool zone3dCompound::isWithinIgnoreExcludes(float x, float y, float z, float tolerance) {
	bool retv = zone3d::isWithin(x, y, z, tolerance);
	return retv;
}

bool zone3dCompound::isWithinSubZones(float x, float y, float z, float tolerance) {
	bool retv = false;
	if (mSubZones.size() > 0) {
		for each (zone3d *sz in mSubZones) {
			if (!sz->mExclusion && sz->isWithin(x, y, z, tolerance)) {
				retv = true;
				break;
			}
		}
	}
	return retv;
}

void zone3dCompound::addZone(zone3d *additionalZone) {
	mSubZones.push_back(additionalZone);
}
bool zone3dCompound::contains(char *zoneName) {
	bool retv = false;
	int idx = findSubZoneIdx(zoneName);
	if (idx >= 0 && idx < mSubZones.size()) {
		retv = true;
	}
	return retv;
}
bool zone3dCompound::removeZone(char *zoneName) {
	return removeZone(findSubZoneIdx(zoneName));
}
bool zone3dCompound::removeZone(int szIdx) {
	bool retv = false;
	if (szIdx >= 0 && szIdx < mSubZones.size()) {
		mSubZones.erase(mSubZones.begin() + szIdx);
		retv = true;
	}
	return retv;
}
vector<char *> *zone3dCompound::subZoneNames() {
	vector<char *> *retv = new vector<char *>();
	for (int idx = 0; idx < mSubZones.size(); idx++) {
		retv->push_back(mSubZones[idx]->getName());
	}
	return retv;
}
bool zone3dCompound::hasSubZones() {
	bool retv = false;
	if (mSubZones.size() > 0) {
		retv = true;
	}
	return retv;
}

vector<zone3d *> *zone3dCompound::subZones() {
	return &mSubZones;
}
zone3dCompound::zone3dCompound(zoneShape shape, int userDefinedType, Point3f wallCentre, float rad, float limitZ, char *name, int id) : zone3d(nullptr, shape, userDefinedType, wallCentre,  rad,  limitZ,  name, id) {
	mType = compoundZone;
	mExclusion = false;

}
zone3dCompound::zone3dCompound() {
	mType = compoundZone;
	mExclusion = false;
}
zone3dCompound::~zone3dCompound() {

}

zone3dCompound *zone3dCompound::duplicate(const char *name, int *pId) {
	int workingId = *pId;
	zone3dCompound * retv = new zone3dCompound(mShape, mUserDefinedType, Centre(), mRadius, mLimitZ, (char *) name, workingId++);
	retv->mAngle = mAngle;
	for (int idx = 0; idx < mSubZones.size(); idx++) {
		const char *sName = mSubZones[idx]->getName();
		char *newName = generateDuplicateName(sName);
		zone3d *tmpZone = mSubZones[idx]->duplicate(retv, newName, workingId++);
		delete newName;
		retv->addZone(tmpZone);
	}
	return retv;
}
char *zone3dCompound::generateDuplicateName(const char *oldName) {
	int newNameLen = strlen(oldName) + 64;
	char *newName = new char[newNameLen];
	int uniqueIdx = 0;
	bool nameIsFound = true;
	do {
		if (uniqueIdx < 1) {
			sprintf(newName, "%sDup", oldName);
		}
		else {
			sprintf(newName, "%sDup%d", oldName, uniqueIdx);
		}
		if (findSubZoneIdx(newName) >= 0) {
			uniqueIdx++;
		}
		else {
			nameIsFound = false;
		}
	} while (nameIsFound);
	return newName;
}

zone3d *zone3dCompound::findSubZone(char *zoneName) {
	zone3d *retv = nullptr;
	int idx = findSubZoneIdx(zoneName);
	if (idx >= 0 && idx < mSubZones.size()) {
		retv = mSubZones[idx];
	}
	return retv;
}
int zone3dCompound::findSubZoneIdx(char *zoneName) {
	int retv = -1;
	for (int idx = 0; idx < mSubZones.size(); idx++) {
		if (strcmp(mSubZones[idx]->getName(), zoneName) == 0) {
			retv = idx;
			break;
		}
	}
	return retv;
}
void zone3dCompound::clearAndDelete() {
	for (int idx = 0; idx < mSubZones.size(); idx++) {
		delete mSubZones[idx];
	}
	mSubZones.clear();
}
int zone3dCompound::highestID() {
	int retv = 0;
	if (mID > retv) retv = mID;
	for (int idx = 0; idx < mSubZones.size(); idx++) {
		if (mSubZones[idx]->mID > retv) retv = mSubZones[idx]->mID;
	}
	return retv;
}

string *zone3dCompound::serialise() {
	stringstream ss;
	ss << mType << "," << mSubZones.size() << ",";
	string *retv = new string(ss.str());
	string *baseVals = zone3d::serialise();
	retv->append(*baseVals);
	retv->append("\n");
	delete baseVals;
	for (int idx = 0; idx < mSubZones.size(); idx++) {
		baseVals = mSubZones[idx]->serialise();
		retv->append(*baseVals);
		retv->append("\n");
		delete baseVals;
	}
	return retv;
}
int zone3dCompound::deSerialise(int subZonesNeeded, vector<string> args) { // returns -1 if fails, otherwise the number of subzones that are to follow
	int retv = -1;
	if (args.size() >= 2) {
		int tmp = stoi(args[0]); // type
		if (tmp == (int)zone3d::subZone) {
			zone3d *tmpSubZone = new zone3d();
			if (tmpSubZone->deSerialise(args, this, 0)) {
				addZone(tmpSubZone);
				retv = subZonesNeeded - 1;
			}
			else {
				delete tmpSubZone;
			}
		}
		else if (tmp == (int)zone3d::compoundZone) {
			int reqSubZones = stoi(args[1]);
			if (zone3d::deSerialise(args, nullptr, 2)) { // the rest of the line are my own base class values 
				retv = reqSubZones;
			}
		}
	}
	return retv;
}
