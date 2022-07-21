#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "CVcommon.h"
#include "locationsFile.h"
#include "umZone.h"
#include "zone3d.h"
#include "perspective.h"

using namespace cv;
using namespace std;
using namespace System;

struct clsfctnRecord;
class fpClassifier; // forward declaration, circular dependency and one or the other had to be first
class ruleValidity;

class movementRule {
public:
	enum ruleType { ruleNone, ruleCompound, ruleSpeed, ruleDirection, ruleDirectionR, ruleDirectionZ,
		ruleDuration, ruleOutside, ruleInside, ruleDirectionP, ruleSpeedR, ruleSpeedT, ruleDistanceZ,
		ruleHalfTop, ruleHalfBottom, ruleHalfLeft, ruleHalfRight
	};
	enum ruleConjunction { conjunctionNone, conjunctionAND, conjunctionOR, conjunctionTHEN, conjunctionNOT	};
	enum ruleScope { zone, subzone, zoneX, subzoneX };
	enum ruleOverlap {overlapNone, overlapMinCommon, overlapMaxEither, thenPre, thenPost, thenTotal };
	static const char *ruleNames[];
	static const char *ruleConjunctionNames[];
	static const char *overlapNames[];
	static const char *ruleScopeNames[];
	string mName;
	ruleType mType;
	int mUserZoneType;
	ruleConjunction mConjunction;
	ruleOverlap mOverlap;
	float mMinimum;
	float mMaximum;
	float mRequiredPortion;
	fpClassifier *pParent;
	float mThenMaxOverlap;
	float mThenMaxGap;
	bool mIsFragmentable;
	movementRule(fpClassifier *parent);
	~movementRule();

	string *serialise();
	bool deSerialise(vector<string> args, string *resultComment, int startingOffset = 0);
	void addRule(movementRule *toBeAdded);
	bool movementRule::overlapTiming(std::vector<point3dMm> *pLocations, int *st, int *et, ruleValidity *rv);
	bool testCompoundRuleValidity(std::vector<point3dMm> *pLocations, vector<ruleValidity *> *pSimpleRuleValidities);
	ruleValidity *findNamedRuleValidity(string ruleName, int earliestIdx, vector<ruleValidity *> *pSimpleRuleValidities);
	ruleValidity *movementRule::findNamedZoneRuleValidity(string ruleName, vector<string *> *previousRules, bool findTheEarliest, bool earlierOtherBlock, int earliestIdx, zone3d *reqZone, vector<ruleValidity *> *pSimpleRuleValidities);
	int usesNamedRule(string soughtName);
	bool scopeIsZone();
	bool scopeIsSubZone();
	bool scopeIsX();
private:
	ruleScope mScope;
	vector<movementRule *> subRules;
	movementRule::ruleType parseType(string arg);
	movementRule::ruleConjunction parseConjunction(string arg);
	movementRule::ruleOverlap parseOverlap(string arg);
	movementRule::ruleScope movementRule::parseScope(string arg);
};

class movementClassification {
public:
	string mName;
	movementRule *pRule;
	movementClassification(fpClassifier *parent);
	string *serialise();
	bool deSerialise(vector<string> args, string *resultComment, int startingOffset = 0);
	int mColourR;
	int mColourG;
	int mColourB;
private:
	fpClassifier *pParent;
};

class ruleValidity {
public:
	int mStartIdx;
	int mEndIdx;
	zone3d *mZoneWhereValid;
	movementRule *mValidRule;
	ruleValidity(int sIdx, int eIdx, zone3d *z, movementRule *v) { mStartIdx = sIdx; mEndIdx = eIdx; mZoneWhereValid = z; mValidRule = v; }
};

class classificationValidty {
public:
	int mStartIdx;
	int mEndIdx;
	string mValidRule;
	string mValidClassification;
	classificationValidty(int sIdx, int eIdx, string ruleName, string classificationName) { mStartIdx = sIdx; mEndIdx = eIdx; mValidRule = ruleName; mValidClassification = classificationName; }
	string *serialise();
	bool deSerialise(vector<string> args, string *resultComment, int startingOffset = 0);
};

class zoneRules {
public:
	zone3d *mZone;
	vector<ruleValidity *> mRules;
};

class zoneClassifications {
public:
	zone3d *mZone;
	vector<classificationValidty *> mClassifications;
};

struct zoneReleventLocations {
	int locIdxStart;
	int locIdxEnd;
	zoneReleventLocations(float s, float e) { locIdxStart = s; locIdxEnd = e; }
};

class clsfctnTotaliser {
	int mLastUpdateIdx;
	bool mCurrentState;
	bool mEventJustEnded;
	float mEventTime;
	float mEventSpeed;
	float mStartTime;
	float mEndTime;
	vector<float> mEventTimes;
	vector<float> mEventSpeeds;
public:
	clsfctnTotaliser();
	void addEvent(int idx, float evtSpeed, float evtTime, float prevTime, bool active);
	int getCount();
	float getCummulativeTime();
	float getEventTime();
	float getEventSpeed();
	float getTotalTime();
	float getAverageTime();
	float getVarriance();
	float getTotalSpeed();
	float getAverageSpeed();
	float getSpeedVarriance();
};

class clsfctnEntryOrExit {
public:
	bool mIsEntry;
	float mAngle;
	float mEventTime;
	int mZoneID;
	int mZoneUserType;
	string mRuleName;
	clsfctnEntryOrExit(bool isEntry, float angle, float eventTime, int zoneID, int zoneUserType, string ruleName);
};

class fpClassifier : csvFile
{
	string mRuleReadResult;
	// global store of all movementRules
	vector<movementRule *> allMovementRules;
	vector<movementRule *> allCompoundMovementRules;
	vector<movementClassification *> allMovementClassifications;
	vector<zoneRules> allZoneRules;
	vector<zoneClassifications> allZoneClassifications;
	vector<classificationValidty> exportableClassifications;
	vector<ruleValidity *> *pSimpleRuleResults;
	vector<vector<clsfctnEntryOrExit *> *> *pRuleEntryExits;

//	vector<umZone *> *mZones;
	vector<zone3dCompound *> *mZone3dCompounds;
	float mHoverSpeed;
	float mWhizzSpeed;
	float mFramesPerSec;
	float mSizeLimit;
	float mFeedPeriod;
	float zoneHysteresisSec = 0.1;
	vector<clsfctnRecord> recordList;
	perspective *mPersp;
#define classifierRange 7
	int mColourCodes[classifierRange][3];
	void clearAllVectors();
	vector<zoneReleventLocations> *oneZoneSimpleRuleMultiple(std::vector<point3dMm> *pLocations, vector<zoneReleventLocations> *pLocsWithinZone, movementRule *ruleToTest, zone3d *zoneToParse);
	float fpClassifier::pointToPointSpeedSq(std::vector<point3dMm> *pLocations, int idx);
	float fpClassifier::rawPointToPointSpeedSq(std::vector<point3dMm> *pLocations, int idx);
	float fpClassifier::pointToPointDirection(std::vector<point3dMm> *pLocations, int idx);
	float fpClassifier::pointToPointDirectionZ(std::vector<point3dMm> *pLocations, int idx);
	vector<zoneReleventLocations> *fpClassifier::amalgamate(std::vector<point3dMm> *pLocations, vector<zoneReleventLocations> *pLocsWithinRule, movementRule *ruleToTest);
	float fpClassifier::pointToPointDirectionPolar(std::vector<point3dMm> *pLocations, int idx, float orgX, float orgY);
	float fpClassifier::pointToPointSpeedRadial(std::vector<point3dMm> *pLocations, int idx, float orgX, float orgY);
	float fpClassifier::pointToPointSpeedTangential(std::vector<point3dMm> *pLocations, int idx, float orgX, float orgY);

public:
	static const char *classnames[];
	static const float defaultHoverLimit;
	enum readResult {classOK, nofile, formatErr};
	enum classification {unclassified, distant, flight, reject, scanBefore, feed, scanAfter };
	enum clsfctnEvent { none, goaway, comeclose, enterZone, leaveZone, enterInner, leaveInner, startHover, endHover , startWhizz, endWhizz};
	enum locationState {idleLocation, withinZone, withinInner, away};
	enum motionState {idleMotion, normalMovement, hovering, whizzing};
	fpClassifier();
	~fpClassifier();
	readResult read(const char *filepathname);
	bool saveRulesAndClassifications(std::vector<point3dMm> *pLocations, float infillFrameRate, const char *basefilepathname, bool appendToSummaryFile);
	void infillRulesAndClassifications(ofstream *locFile, float previousFrameTime, float nextLocTime, float timeIncrement, vector<clsfctnTotaliser *> *pRuleTotalisers);
	bool saveExportableClassifications(std::vector<point3dMm> *pLocations, const char *basefilepathname);
	bool saveExportableClassificationSet(std::vector<point3dMm> *pLocations, const char *className, const char *basefilepathname);
	readResult readClassifications(const char *filepathname);
	int classificationQty() { return exportableClassifications.size(); }
	string serialise();
	string getRuleReadResult();
	bool parseLine(std::vector<std::string>   lineArray, int *red, int *green, int *blue);
	void setSpeeds(float framesPerSec, float hover, float whizz, float sizeLimit, float feedPeriod);
//	void setZones(vector<umZone *> *pZones);
	void setPerspective(perspective *persp);
	void parse(std::vector<point3dMm> *pLocations);
	clsfctnEvent fpClassifier::enterOrLeaveZone(locationState currentLocState, point3dMm *pLocr, bool innerDistinct, umZone **zoneEntered);
	fpClassifier::clsfctnEvent checkSpeedChange(motionState currentMoveState, point3dMm *pLoc);
	bool speedLookAheadNotRevert(std::vector<point3dMm> *pLocations, int locIdx, motionState currentMoveState);
	bool locationLookAheadNotRevert(std::vector<point3dMm> *pLocations, int locIdx, locationState currentLocState, umZone *zoneEntered);
//	umZone *fpClassifier::getZoneAt(int idx);
	static const char *toString(classification cls);
	void colour(classification cls, int *red, int * green, int *blue);
	float timePeriod(std::vector<point3dMm> *pLocations, int startIdx, int endIdx);

	bool fpClassifier::parse(std::vector<point3dMm> *pLocations, vector<zone3dCompound *> *pZonesToParse);
	bool parseOneZoneForSimpleRules(std::vector<point3dMm> *pLocations, vector<ruleValidity* > *pSimpleRuleValidities, vector<clsfctnEntryOrExit *> *pOneZoneEntryExits, zone3d *zoneToParse);
	bool compoundFromSimpleRules(std::vector<point3dMm> *pLocations, vector<ruleValidity* > *pSimpleRuleValidities);
	void setZones(vector<zone3dCompound *> *pZones);
	zone3dCompound *fpClassifier::getZone3dAt(int idx);
	movementRule *findNamedRule(string ruleName);
	int fpClassifier::findRuleClassificationIdx(movementRule *srch);
	movementClassification *findNamedClassification(string ruleName);
	int lookBackOrForward(std::vector<point3dMm> *pLocations, int startidx, zone3d *zoneToParse, float timeDelta);
	bool findAllPointsForOneRuleInOneZone(std::vector<point3dMm> *pLocations, zone3d *zoneToParse, movementRule *mvr, vector<zoneReleventLocations> *pLocsWithinZone, vector<zoneReleventLocations> *pLocsBefore, vector<zoneReleventLocations> *pLocsAfter, float sizeTolerance, float preTimeTolerance, float postTimeTolerance);
	bool findAllPointsForOneRuleInOneZoneOrSubzone(std::vector<point3dMm> *pLocations, zone3d *zoneToParse, movementRule *mvr, vector<zoneReleventLocations> *pLocsBefore, vector<zoneReleventLocations> *pLocsAfter,
		vector<ruleValidity* > *pSimpleRuleValidities, vector<ruleValidity* > *pAdditionalSimpleRuleValidities, float sizeTolerance, float preTimeTolerance, float postTimeTolerance);
	zoneReleventLocations findNextPoints(std::vector<point3dMm> *pLocations, int idx, zone3d *zoneToParse, bool zoneNotSubzone, bool doExclusions, float sizeTollerance);

	bool locBandValid(zoneReleventLocations *bandToTest) { return ((bandToTest->locIdxStart >= 0) && (bandToTest->locIdxEnd >= 0) && (bandToTest->locIdxStart < bandToTest->locIdxEnd));  }
};
struct clsfctnRecord {
	int locIdxStart;
	fpClassifier::locationState recordedLocState;
	fpClassifier::motionState recordedMoveState;
};
