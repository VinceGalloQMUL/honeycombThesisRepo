#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <list>

using namespace cv;
using namespace std;
using namespace System;
#include "CVcommon.h"
#include "csvFile.h"

#include "zonesFile.h"

#include "fpClassifier.h"

#define ZONE_AREA_TOLERANCE 1.00
// was 1.05
#define USER_ZONE_ANY -1

const char *movementRule::ruleNames[] = { "NONE", "COMPOUND", "SPEED", "DIRECTION", "DIRECTIONR", "DIRECTIONZ",
											"DURATION", "OUTSIDE", "INSIDE", "DIRECTIONP", "SPEEDR", "SPEEDT", "DISTANCEZ",
											"HALFTOP", "HALFBOTTOM", "HALFLEFT", "HALFRIGHT"};
const char *movementRule::ruleConjunctionNames[] = { "---", "AND", "OR", "THEN", "NOT"};
const char *movementRule::overlapNames[] = { "NONE", "MINCOMMON", "MAXEITHER", "PRE", "POST", "TOTAL" };
const char *movementRule::ruleScopeNames[] = { "ZONE", "SUBZONE", "ZONEX", "SUBZONEX" };

const char *fpClassifier::classnames[] = { "unclassified", "distant", "flight", "reject", "scanBefore", "feed", "scanAfter" };

const float fpClassifier::defaultHoverLimit = 50.0;


string *movementRule::serialise() {
	stringstream ss;
	ss << mName << "," << ruleNames[(int)mType] << ",";
	switch (mType) {
	case ruleNone:
		ss << "ruleNone, 0, 0, 0";
		break;
	case ruleSpeed:
	case ruleSpeedR:
	case ruleSpeedT:
	case ruleDirection:
	case ruleDirectionR:
	case ruleDirectionP:
	case ruleDirectionZ:
	case ruleDuration:
	case ruleOutside:
	case ruleInside:
	case ruleDistanceZ:
	case ruleHalfTop:
	case ruleHalfBottom:
	case ruleHalfLeft:
	case ruleHalfRight:
		ss << ruleScopeNames[(int)mScope] << ", " << mUserZoneType << ", " << mMinimum << ", " << mMaximum << ", " << mRequiredPortion;
		break;
	case ruleCompound:
		if (mConjunction == conjunctionTHEN) {
			ss << mUserZoneType << "," << ruleConjunctionNames[(int)mConjunction] << ","  << mThenMaxOverlap << "," << mThenMaxGap << "," << overlapNames[(int)mOverlap];
		}
		else {
			ss << mUserZoneType << "," << ruleConjunctionNames[(int)mConjunction] << "," << overlapNames[(int)mOverlap];
		}
		for (int idx = 0; idx < subRules.size(); idx++) {
			ss << "," << subRules[idx]->mName;
		}
		break;
	}
	string *retv = new string(ss.str());
	return retv;
}
movementRule::ruleType movementRule::parseType(string arg) {
	movementRule::ruleType retv = ruleNone;
	std::transform(arg.begin(), arg.end(), arg.begin(), ::toupper);
	for (int idx = 0; idx < sizeof(ruleNames) / sizeof(ruleNames[0]); idx++) {
		if (arg == ruleNames[idx]) {
			retv = (movementRule::ruleType)idx;
			break;
		}
	}
	return retv;
}
movementRule::ruleConjunction movementRule::parseConjunction(string arg) {
	movementRule::ruleConjunction retv = conjunctionNone;
	std::transform(arg.begin(), arg.end(), arg.begin(), ::toupper);
	for (int idx = 0; idx < sizeof(ruleConjunctionNames) / sizeof(ruleConjunctionNames[0]); idx++) {
		if (arg == ruleConjunctionNames[idx]) {
			retv = (movementRule::ruleConjunction)idx;
		}
	}
	return retv;
}
movementRule::ruleOverlap movementRule::parseOverlap(string arg) {
	movementRule::ruleOverlap retv = overlapNone;
	std::transform(arg.begin(), arg.end(), arg.begin(), ::toupper);
	for (int idx = 0; idx < (sizeof(overlapNames) / sizeof(overlapNames[0])); idx++) {
		if (arg == overlapNames[idx]) {
			retv = (movementRule::ruleOverlap)idx;
			break;
		}
	}
	return retv;
}
bool  movementRule::scopeIsZone() {
	return (mScope == zone || mScope == zoneX);
}
bool  movementRule::scopeIsSubZone() {
	return (mScope == subzone || mScope == subzoneX);
}
bool  movementRule::scopeIsX() {
	return (mScope == zoneX || mScope == subzoneX);
}

bool movementRule::deSerialise(vector<string> args, string *resultComment, int startOffset) {
	bool retv = false;
	if (args.size() >= startOffset + 6) {
		int idx = startOffset;
		mName = args[idx++];
		mType = parseType(args[idx++]);
		switch (mType) {
		case ruleSpeed:
		case ruleDirection:
		case ruleDirectionR:
		case ruleDirectionZ:
		case ruleDuration:
		case ruleOutside:
		case ruleInside:
		case ruleSpeedR:
		case ruleSpeedT:
		case ruleDistanceZ:
		case ruleDirectionP:
		case ruleHalfTop:
		case ruleHalfBottom:
		case ruleHalfLeft:
		case ruleHalfRight:
		{
				mScope = parseScope(args[idx++]);
				mUserZoneType = csvFile::parseLine1iSafely(args[idx++], USER_ZONE_ANY);
				mMinimum = csvFile::parseLine1fSafely(args[idx++], 0);
				mMaximum = csvFile::parseLine1fSafely(args[idx++], 0);
				mRequiredPortion = csvFile::parseLine1fSafely(args[idx++], 1.0);
			}
			retv = true;
			break;
		case ruleCompound:
			mUserZoneType = csvFile::parseLine1iSafely(args[idx++], USER_ZONE_ANY);
			mConjunction = parseConjunction(args[idx++]);
			if (mConjunction == conjunctionTHEN) {
				mThenMaxOverlap = csvFile::parseLine1fSafely(args[idx++], 0);
				mThenMaxGap = csvFile::parseLine1fSafely(args[idx++], 0);
				if (args.size() != startOffset + 9) {
					*resultComment = "Expected exactly 2 sub-rules for rule " + args[0];
					break;
				}
			}
			if (mConjunction == conjunctionNOT) {
				mIsFragmentable = true;
			}
			mOverlap = parseOverlap(args[idx++]);
			retv = true;
			for (; idx < args.size(); idx++) {
				movementRule *tmpRule = pParent->findNamedRule(args[idx]);
				if (tmpRule != nullptr) {
					subRules.push_back(tmpRule);
				}
				else {
					*resultComment = "Rule " + args[idx] + " not found";
					retv = false;
					break;
				}
			}
			break;
		default:
					*resultComment = "Unknown rule type " + args[1] + " for rule " + args[0];
			break;
		}
	}
	else {
		*resultComment = "Too few arguments";
	}
	return retv;
}

void movementRule::addRule(movementRule *toBeAdded) {
	subRules.push_back(toBeAdded);
}

ruleValidity *movementRule::findNamedRuleValidity(string ruleName, int earliestIdx, vector<ruleValidity *> *pSimpleRuleValidities) {
	ruleValidity *retv = nullptr;
	for each (ruleValidity *rv in *pSimpleRuleValidities) {
		if (earliestIdx >= rv->mStartIdx && earliestIdx <= rv->mEndIdx && rv->mValidRule->mName == ruleName) {
			retv = rv;
		}
	}
	return retv;
}
ruleValidity *movementRule::findNamedZoneRuleValidity(string ruleName, vector<string *> *previousRules, bool findTheEarliest, bool earlierOtherBlock, int earliestIdx, zone3d *reqZone, vector<ruleValidity *> *pSimpleRuleValidities) {
	ruleValidity *retv = nullptr;
	int idxCnt = 0;
	ruleValidity *earliestNotReq = nullptr;

	for each (ruleValidity *rv in *pSimpleRuleValidities) {
		if (earliestIdx < rv->mEndIdx) { // ruleValidity extends till after the poit we are testing...so worth a look
			if (rv->mValidRule->mName == ruleName) { // and the name matches
				if ((mUserZoneType == USER_ZONE_ANY) || reqZone == nullptr || reqZone == rv->mZoneWhereValid || 
						(rv->mZoneWhereValid->mParent != nullptr && mUserZoneType == rv->mZoneWhereValid->mParent->mUserDefinedType) ||
						(reqZone->mParent != nullptr && mUserZoneType == reqZone->mParent->mUserDefinedType)) {
					// found candidate
					if (!findTheEarliest) {
						retv = rv;
						break;
					}
					if (retv == nullptr || retv->mStartIdx > rv->mStartIdx) {
						retv = rv;
					}
				}
			}
			else {
				// pattern for another rule that the one we seek..so find earliest such to provide a limit on the viable search
				if (rv->mStartIdx > earliestIdx && (earliestNotReq == nullptr || earliestNotReq->mStartIdx > rv->mStartIdx)) {
					bool rememberAsEarliest = true;
					if (previousRules != nullptr) {
						for each (string *ignoreStr in *previousRules)
						{
							if (*ignoreStr == rv->mValidRule->mName) {
								rememberAsEarliest = false;
								break;
							}
						}
					}
					if (rememberAsEarliest) earliestNotReq = rv;
				}

			}
		}
		idxCnt++;
	}
	if (earlierOtherBlock && earliestNotReq != nullptr && retv != nullptr && earliestNotReq->mStartIdx < retv->mStartIdx) {
		return nullptr;
	}
	else {
		return retv;
	}
}
int movementRule::usesNamedRule(string soughtName) {
	int retv = -1;
	for (int idx = 0; idx < subRules.size(); idx++) {
		if (subRules[idx]->mName == soughtName) {
			retv = idx;
			break;
		}
	}
	return retv;
}
bool movementRule::overlapTiming(std::vector<point3dMm> *pLocations, int *st, int *et, ruleValidity *rv) {
	bool retv = false;
	switch(mOverlap) {
	case overlapNone:
		break;
	case thenPre:
	case thenPost:
	case thenTotal:
		{
			bool thenGap = true;
			bool thenEncompass = false;
			if (*et < rv->mStartIdx) { // existing times followed by a gap before the new event 
				if ((pLocations->at(rv->mStartIdx).fTime - pLocations->at(*et).fTime) < mThenMaxGap) {
					retv = true;
				}
			}
			else { // tail of previous is after the start of the new
				thenGap = false;
				if ((pLocations->at(*et).fTime - pLocations->at(rv->mStartIdx).fTime) < mThenMaxOverlap) {
					retv = true;
					if ((pLocations->at(*et).fTime > pLocations->at(rv->mEndIdx).fTime)) {
						//current endtime later than discovered end time, so wholly overlapping
						thenEncompass = true;
						retv = false; //revised my thinkng, I'm now going for the thought that encompassed zones shoudl be disregarded
										// it doesnt feel like a 'THEN'
					}
				}
				else {
					if (rv->mValidRule->mIsFragmentable) {
						retv = true;
					}
				}
			}
			if (retv) { // so far, we have a successful THEN conjunction, now sort out the resulting period
				if (mOverlap == thenPre) { // start stays as it was
					if (thenGap) *et = rv->mStartIdx; //end moves to the later of old end or following start
				} else if (mOverlap == thenPost) {
					*st = *et;
					if (!thenGap) *et = rv->mStartIdx; //start moves to the earlier of old end or following start
					*et = rv->mEndIdx; // end moves to the end of the follower
				} else { // total, start remains where it was, 
					if (!thenEncompass) *et = rv->mEndIdx; // end moves to the end of the follower, unless this is within the corrent, whereupon the end stays the same
				}

			}
		}
		break;
	case overlapMinCommon:
		if (*st <= rv->mStartIdx && *et >= rv->mStartIdx) {
			*st = rv->mStartIdx;
			if (*et > rv->mEndIdx) {
				*et = rv->mEndIdx;
			}
			retv = true;
		}
		else if (*st <= rv->mEndIdx && *et >= rv->mEndIdx){
			*et = rv->mEndIdx;
			if (*st < rv->mStartIdx) {
				*st = rv->mStartIdx;
				retv = true;
			}
			retv = true;
		}
		else {
			retv = false; // no overlap, so the combination doesnt work .. probably would have failed on the AND conjunction clause anyway
		}
		break;
	case overlapMaxEither:
		if (*st > rv->mStartIdx) {
			*st = rv->mStartIdx;
		}
		if (*et < rv->mEndIdx) {
			*et = rv->mEndIdx;
		}
		retv = true;
		break;

	}
	return retv;

}
bool movementRule::testCompoundRuleValidity(std::vector<point3dMm> *pLocations, vector<ruleValidity *> *pSimpleRuleValidities) {
	int st = 0;
	int et = 0;
	int ruleEarliest = -1;
	bool foundAtLeastOne = false;
	bool firstRule = true;
	ruleValidity *tmpValidity = nullptr;
	zone3d *tmpZone = nullptr;
	bool isValid = false;
	if (mType == ruleCompound) { // shouldnt have even been invoked if its not
		switch (mConjunction) {
		case conjunctionAND:
		case conjunctionOR:
		case conjunctionTHEN:
			for (;;) {
				firstRule = true;
				tmpZone = nullptr;
				isValid = false;
				vector<string *> previousRules;
				for each (movementRule *mr in subRules) {
					ruleValidity *oneRule;
					if ((oneRule = findNamedZoneRuleValidity(mr->mName, &previousRules, true, false /*!firstRule*/, ruleEarliest, tmpZone, pSimpleRuleValidities)) != nullptr) {
						if (firstRule) {
							tmpZone = oneRule->mZoneWhereValid;
							if (ruleEarliest < 0)
								ruleEarliest = oneRule->mStartIdx;
							st = oneRule->mStartIdx;
							et = oneRule->mEndIdx;
							firstRule = false;
							isValid = false;
							previousRules.push_back(&(oneRule->mValidRule->mName));
							if (mConjunction == conjunctionOR) {
								isValid = true;
							} // an OR conjunction will return with valid if at least one was found
						}
						else {
							isValid = overlapTiming(pLocations, &st, &et, oneRule); // we will break out if we need to contradicts this assumption
						}
					}
					else {
						if (mConjunction == conjunctionAND || mConjunction == conjunctionTHEN) {
							isValid = false; // AND and THEN need both(all) rules to be valid
						} // an OR conjunction will return with valid if at least one was found
						break;
					}
				}
				if (isValid) {
					foundAtLeastOne = true;
					pSimpleRuleValidities->push_back(new ruleValidity(st, et, tmpZone, this));
				}
				ruleEarliest = et + 1;
				if (firstRule) break; // still looking for a suitable first rule, so exit for(;;)
			}
			break;
		case conjunctionNOT:
			if (subRules.size() > 0) {
				int idxStart = 0;
				int idxTmpEnd = 0;
				int idxEnd = pLocations->size() - 1;
				movementRule *mr = subRules[0];
				std::vector<ruleValidity *> notArray;
				bool inGap = true;
				for(int locIdx = 0; locIdx < idxEnd; locIdx++)  {
					bool foundOneSection = false;
					for each (ruleValidity *rv in *pSimpleRuleValidities) {
						if (rv->mValidRule->mName == mr->mName) {
							tmpZone = rv->mZoneWhereValid;
							if (locIdx >= rv->mStartIdx && locIdx <= rv->mEndIdx) {
								foundOneSection = true;
								// found a rule that blocks out this point
								if (inGap) {
									// were in a gap, so quit search
									idxTmpEnd = locIdx - 1;
								}
								else {
									// continue seeking another gap
									idxStart = locIdx + 1; // next gap must be at least one further on
								}
								break; // break out of rule test loop, continue locations loop
							}
						}
					}
					if (foundOneSection) {
						if (inGap) {
							// were in a gap, so now not, save the block we have accumulated thus far
							if (idxTmpEnd > idxStart) {
								notArray.push_back(new ruleValidity(idxStart, idxTmpEnd, tmpZone, this));
							}
							idxStart = idxTmpEnd + 1;
							inGap = false;
						}
						else {
							// continue seeking another gap
						}
					}
					else {
						if (inGap) {
							// were in a gap, so continue as such
						}
						else {
							// were not in a gap, but now found one
							idxStart = locIdx;
							inGap = true;
						}
					}
				}
				if (idxStart < idxEnd) {
					notArray.push_back(new ruleValidity(idxStart, idxEnd, tmpZone, this));
				}
				if (notArray.size() > 0) {
					for each (ruleValidity *rv in notArray) {
						pSimpleRuleValidities->push_back(rv);
					}

				}
			}
			break;
		};
	}
	return foundAtLeastOne;
}

movementRule::ruleScope movementRule::parseScope(string arg) {
	movementRule::ruleScope retv = zone;
	std::transform(arg.begin(), arg.end(), arg.begin(), ::toupper);
	for (int idx = 0; idx < sizeof(ruleScopeNames) / sizeof(ruleScopeNames[0]); idx++) {
		if (arg == ruleScopeNames[idx]) {
			retv = (movementRule::ruleScope)idx;
			break;
		}
	}
	return retv;
}



movementRule::movementRule(fpClassifier * parent)
{
	mType = ruleNone;
	mConjunction = ruleConjunction::conjunctionNone;
	mOverlap = ruleOverlap::overlapNone;
	mMinimum = 0;
	mMaximum = 0;;
	mRequiredPortion = 0;
	mThenMaxGap = 0;
	mThenMaxOverlap = 0;
	pParent = parent;
	mIsFragmentable = false;
}


movementRule::~movementRule()
{
}

bool movementClassification::deSerialise(vector<string> args, string *resultComment, int startOffset) {
	bool retv = false;
	if (args.size() == startOffset + 5) {
		int idx = startOffset;
		mName = args[idx++];
		if (args[idx] == "NONE") {
			pRule = nullptr;
			idx++;
		}
		else {
			pRule = pParent->findNamedRule(args[idx++]);
			if (pRule == nullptr) {
				*resultComment = "Rule " + args[idx - 1] + " not found in Classification " + mName;
				return false;
			}
		}
		mColourR = csvFile::parseLine1iSafely(args[idx++], 128);
		mColourG = csvFile::parseLine1iSafely(args[idx++], 128);
		mColourB = csvFile::parseLine1iSafely(args[idx++], 128);
		retv = true;
	}
	return retv;
}

string *movementClassification::serialise() {
	stringstream ss;
	if (pRule == nullptr) {
		ss << mName << ",NONE";
	}
	else {
		ss << mName << "," << pRule->mName;
	}
	ss << "," << mColourR << "," << mColourG << "," << mColourB;
	string *retv = new string(ss.str());
	return retv;
}
movementClassification::movementClassification(fpClassifier * parent)
{
	pParent = parent;
}

string *classificationValidty::serialise() {
	stringstream ss;
	ss << mStartIdx << "," << mEndIdx << "," << mValidRule << "," << mValidClassification;
	string *retv = new string(ss.str());
	return retv;
}
bool classificationValidty::deSerialise(vector<string> args, string *resultComment, int startOffset) {
	// 3,32, Zone0,compoundRule1,class1

	bool retv = false;
	if (args.size() == startOffset + 4) {
		int idx = startOffset;
		mStartIdx = csvFile::parseLine1iSafely(args[idx++], -1);
		mEndIdx = csvFile::parseLine1iSafely(args[idx++], -1);
		mValidRule = args[idx++];
		mValidClassification = args[idx++];
		retv = true;
	}
	else {
		*resultComment = "Wrong number of arguments, expected 4";

	}
	return retv;
}


fpClassifier::fpClassifier()
{
	//	mZones = new vector<umZone *>();
	mHoverSpeed = 0;
	mWhizzSpeed = 1000;
	mFramesPerSec = 40;
	mFeedPeriod = 1.5;
	mPersp = nullptr;
	pSimpleRuleResults = nullptr;
	pRuleEntryExits = nullptr;
	mColourCodes[0][0] = 128; //unclassified *red = 128; *green = 128; *blue = 128;
	mColourCodes[0][1] =  128;
	mColourCodes[0][2] =  128;
	mColourCodes[1][0] = 255; //distant *red = 255; *green = 255; *blue = 255;
	mColourCodes[1][1] = 255;
	mColourCodes[1][2] = 255;
	mColourCodes[2][0] = 40; //flight *red = 40; *green = 40; *blue = 40;
	mColourCodes[2][1] = 40;
	mColourCodes[2][2] = 40;
	mColourCodes[3][0] = 237; //reject *red = 237; *green = 28; *blue = 36;
	mColourCodes[3][1] = 28;
	mColourCodes[3][2] = 36;
	mColourCodes[4][0] = 255; //scanBefore *red = 255; *green = 242; *blue = 0;
	mColourCodes[4][1] = 242;
	mColourCodes[4][2] = 0;
	mColourCodes[5][0] = 34; //feed *red = 34; *green = 177; *blue = 76;
	mColourCodes[5][1] = 177;
	mColourCodes[5][2] = 76;
	mColourCodes[6][0] = 0; //scanAfter *red = 0; *green = 162; *blue = 232;
	mColourCodes[6][1] = 162;
	mColourCodes[6][2] = 232;

}
void fpClassifier::colour(classification cls, int *red, int * green, int *blue) {
	int clsInt = (int)cls;
	if (clsInt >= 0 && clsInt < allMovementClassifications.size()) {
		*red = allMovementClassifications[clsInt]->mColourR;
		*green = allMovementClassifications[clsInt]->mColourG;
		*blue = allMovementClassifications[clsInt]->mColourB;
	}
	else {
		*red = *green = *blue = 128;
	}
	//switch (cls) {
	//default:
	//case classification::unclassified:
	//	*red = mColourCodes[0][0];
	//	*green = mColourCodes[0][1];
	//	*blue = mColourCodes[0][2];
	//	break;
	//case classification::distant:
	//	*red = mColourCodes[1][0];
	//	*green = mColourCodes[1][1];
	//	*blue = mColourCodes[1][2];
	//	break;
	//case classification::flight:
	//	*red = mColourCodes[2][0];
	//	*green = mColourCodes[2][1];
	//	*blue = mColourCodes[2][2];
	//	break;
	//case classification::reject:
	//	*red = mColourCodes[3][0];
	//	*green = mColourCodes[3][1];
	//	*blue = mColourCodes[3][2];
	//	break;
	//case classification::scanBefore:
	//	*red = mColourCodes[4][0];
	//	*green = mColourCodes[4][1];
	//	*blue = mColourCodes[4][2];
	//	break;
	//case classification::feed:
	//	*red = mColourCodes[5][0];
	//	*green = mColourCodes[5][1];
	//	*blue = mColourCodes[5][2];
	//	break;
	//case classification::scanAfter:
	//	*red = mColourCodes[6][0];
	//	*green = mColourCodes[6][1];
	//	*blue = mColourCodes[6][2];
	//	break;
	//}
}

fpClassifier::~fpClassifier()
{
}
const char *fpClassifier::toString(classification cls) {
	return classnames[(int)cls];
}

bool fpClassifier::parseLine(std::vector<std::string>   lineArray, int *red, int *green, int *blue) {
	bool retv = false;
	if (lineArray.size() == 3) {
		*red = std::stoi(lineArray[0]);
		*green = std::stoi(lineArray[1]);
		*blue = std::stoi(lineArray[2]);
		retv = true;
	}
	return retv;
}

void fpClassifier::clearAllVectors() {
	for each (movementRule *mr in allMovementRules) delete mr;
	// contents will already have been deleted by the line above .. for each (movementRule *mr in allCompoundMovementRules) delete mr;
	for each (movementClassification *mc in allMovementClassifications) delete mc;
	allMovementRules.clear();
	allCompoundMovementRules.clear();
	allMovementClassifications.clear();
	allZoneRules.clear();
	allZoneClassifications.clear();
	exportableClassifications.clear();
}

string fpClassifier::getRuleReadResult() {
	return mRuleReadResult;
}

fpClassifier::readResult fpClassifier::read(const char *filepathname) {
	readResult retv = nofile;

	int lineNo = 0;
////	throw(664);
	stringstream ss;
	ifstream str;
	clearAllVectors();
	str.open(filepathname);
	string oneRuleReadResult;
	if (str.is_open()) {
		retv = formatErr;
		lineNo++;
		std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		if (lineArray.size() != 1) {
			// original format, N lines of 3 colours
			int red, blue, green;
			for (int idx = 1; idx < classifierRange; idx++) {
				lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
				if (parseLine(lineArray, &red, &green, &blue)) {
					mColourCodes[idx][0] = red;
					mColourCodes[idx][1] = green;
					mColourCodes[idx][2] = blue;
				}
			}
		}
		else {
			mRuleReadResult = "";
			lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
			lineNo++;
			if (lineArray.size() == 1 && lineArray[0] == "Rules") {
				for (;;) {
					lineNo++;
					lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
					movementRule *tmprule = new movementRule(this);
					if (tmprule->deSerialise(lineArray, &oneRuleReadResult, 0)) {
						allMovementRules.push_back(tmprule);
						if (tmprule->mType == movementRule::ruleCompound) {
							allCompoundMovementRules.push_back(tmprule);
						}
					}
					else {
						delete tmprule;
						break;
					}
				}
 			}
			if (lineArray.size() == 1 && lineArray[0] == "Classifications") {
				for (;;) {
					lineNo++;
					lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
					if (lineArray.size() < 5) {
						retv = classOK;
						break;
					}
					movementClassification *tmpClass = new movementClassification(this);
					if (tmpClass->deSerialise(lineArray, &oneRuleReadResult, 0)) {
						allMovementClassifications.push_back(tmpClass);
					}
					else {
						delete tmpClass;
						ss <<  "Line " << lineNo << ", " << oneRuleReadResult << "\n";
						break;
					}
				}
			} else 
			{
				ss <<  "Line " << lineNo << ", " << oneRuleReadResult << "\n";
			}
		}
		str.close();
		bool atLeastOneRuleUnused = false;
		for (int mrIdx = 0; mrIdx < allMovementRules.size(); mrIdx++) {
			bool ruleIsUsed = false;
			for (int cmrIdx = 0; cmrIdx < allCompoundMovementRules.size(); cmrIdx++) {
				if (allCompoundMovementRules[cmrIdx]->usesNamedRule(allMovementRules[mrIdx]->mName) != -1) {
					ruleIsUsed = true;
					break;
				}
			}
			for (int clsIdx = 0; clsIdx < allMovementClassifications.size(); clsIdx++) {
				if (allMovementClassifications[clsIdx]->pRule != nullptr && allMovementClassifications[clsIdx]->pRule->mName == allMovementRules[mrIdx]->mName) {
					ruleIsUsed = true;
					break;
				}
			}
			if (!ruleIsUsed) {
				ss << "Rule " << allMovementRules[mrIdx]->mName << " is not referenced\n";
				retv = readResult::formatErr;
			}
		}
	}
	if (retv == readResult::formatErr) {
		mRuleReadResult = ss.str();
	}
	return retv;
}

string fpClassifier::serialise() {
	string retv;
	string *tmpStr;
	retv = "Rules\n";
	for (int idx = 0; idx < allMovementRules.size(); idx++) {
		tmpStr = allMovementRules[idx]->serialise();
		retv += *tmpStr;
		delete tmpStr;
		retv += "\n";
	}
	retv += "Classifications\n";
	for (int idx = 0; idx < allMovementClassifications.size(); idx++) {
		tmpStr = allMovementClassifications[idx]->serialise();
		retv += *tmpStr;
		delete tmpStr;
		retv += "\n";
	}
	return retv;
}

bool fpClassifier::saveExportableClassifications(std::vector<point3dMm> *pLocations, const char *basefilepathname) {
	bool retv = true;
	for (int idx = 0; idx < allMovementClassifications.size(); idx++) {
		saveExportableClassificationSet(pLocations, allMovementClassifications[idx]->mName.c_str(), basefilepathname);
	}
	return retv;
}

bool fpClassifier::saveRulesAndClassifications(std::vector<point3dMm> *pLocations, float infillFrameRate, const char *basefilepathname, bool appendToSummaryFile ) {
	bool retv = false;
	vector<clsfctnTotaliser *> ruleTotalisers;
	bool doInfill = false;
	float infillFrameGaps = 0;
	float infillFrameGapLimit = 0;
	if (infillFrameRate > 1.0f) {
		infillFrameGaps = 1.0f / infillFrameRate;
		infillFrameGapLimit = 1.1f / infillFrameRate; // allow a bit of scope for computational jutter
	}
	int filenameLen = strlen(basefilepathname);
	filenameLen += 16; // ".Classified.csv" + '\0';
	char *tmpFilenameBuff = new char[filenameLen];
	snprintf(tmpFilenameBuff, filenameLen, "%s.Classified.csv", basefilepathname);
	ofstream locFile(tmpFilenameBuff, ios::out);
	int znIdx = 0;
	float previousFrameTime = pLocations->at(0).fTime;
	float previousFrameSpeed = pLocations->at(0).speed;
	for (int idx = 0; idx < allMovementRules.size(); idx++) {
		ruleTotalisers.push_back(new clsfctnTotaliser());
	}
	locFile << locationsFile::writeCaption();
	for (int idx = 0; idx < allMovementRules.size(); idx++) {
		locFile << ", " << allMovementRules[idx]->mName << ", " << allMovementRules[idx]->mName << ", " << allMovementRules[idx]->mName << "_speed";
	}
	locFile << "\n";

	for (int locIdx = 0; locIdx < pLocations->size(); locIdx++) {
		if (doInfill && (pLocations->at(locIdx).fTime > infillFrameGapLimit + previousFrameTime)) {
			for (int idx = 0; idx < allMovementRules.size(); idx++) {
				// now output the totaliser data
				ruleTotalisers[idx]->addEvent(locIdx, previousFrameSpeed, infillFrameGapLimit + previousFrameTime, previousFrameTime, false); // this will close out any totaliser evets that were running but have now stopped
			}
			infillRulesAndClassifications(&locFile, previousFrameTime, pLocations->at(locIdx).fTime, infillFrameGaps, &ruleTotalisers);
		}
		else {
			locFile << locationsFile::write(&(pLocations->at(locIdx))); // output the loc data
			for each (ruleValidity *rv in *pSimpleRuleResults)
			{
				if (locIdx >= rv->mStartIdx && locIdx <= rv->mEndIdx) {
					for (int idx = 0; idx < allMovementRules.size(); idx++) {
						if (rv->mValidRule->mName == allMovementRules[idx]->mName) {
							float tmpSPd = rawPointToPointSpeedSq(pLocations, locIdx); // pLocations->at(locIdx).speed
							tmpSPd = Math::Sqrt(tmpSPd);
							ruleTotalisers[idx]->addEvent(locIdx, tmpSPd, pLocations->at(locIdx).fTime, previousFrameTime, true);
						}
					}
				}
			}
			for (int idx = 0; idx < allMovementRules.size(); idx++) {
				// now output the totaliser data
				ruleTotalisers[idx]->addEvent(locIdx, pLocations->at(locIdx).speed, pLocations->at(locIdx).fTime, previousFrameTime, false); // this will close out any totaliser evets that were running but have now stopped
				locFile << ", " << ruleTotalisers[idx]->getCummulativeTime() << ", " << ruleTotalisers[idx]->getEventTime() << ", " << ruleTotalisers[idx]->getEventSpeed();
			}
			locFile << "\n";

		}
		previousFrameTime = pLocations->at(locIdx).fTime;
		previousFrameSpeed = pLocations->at(locIdx).speed;
	}
	int endIdx = pLocations->size() - 1;
	for (int completeIdx = 0; completeIdx < 4; completeIdx++) {
		locFile << locationsFile::write(&(pLocations->at(endIdx))); // output the loc data
		for (int idx = 0; idx < allMovementRules.size(); idx++) {
			// now output the totaliser data
			switch (completeIdx) {
			case 0:
				locFile << ",, " << ruleTotalisers[idx]->getCount() << ", ";
				break;
			case 1:
				locFile << ",, " << ruleTotalisers[idx]->getTotalTime() << ", " << ruleTotalisers[idx]->getTotalSpeed();
				break;
			case 2:
				locFile << ",, " << ruleTotalisers[idx]->getAverageTime() << ", " << ruleTotalisers[idx]->getAverageSpeed();
				break;
			case 3:
				locFile << ",, " << ruleTotalisers[idx]->getVarriance() << ", " << ruleTotalisers[idx]->getSpeedVarriance();
				break;
			}
		}
		locFile << "\n";

	}

	retv = true;
	locFile.close();

	filenameLen = strlen(basefilepathname);
	filenameLen += 16; // ".EntryExits.csv" + '\0';
	char *entryExitFilenameBuff = new char[filenameLen];
	snprintf(entryExitFilenameBuff, filenameLen, "%s.EntryExits.csv", basefilepathname);
	ofstream entryExitFile(entryExitFilenameBuff, ios::out);
	int ruleIdx = 0;
	entryExitFile << "EntryNotExit, RuleName, ZoneType, ZoneID, EventTime, AngleToZOneCentre\n";
	for each(vector<clsfctnEntryOrExit *> *pVec in  *pRuleEntryExits) {
		for each(clsfctnEntryOrExit *pClsfcnt in *pVec) {
			entryExitFile << pClsfcnt->mIsEntry << "," << pClsfcnt->mRuleName << "," << pClsfcnt->mZoneUserType << "," << pClsfcnt->mZoneID << "," << pClsfcnt->mEventTime << "," << pClsfcnt->mAngle << "\n";
		}
		ruleIdx++;
	}

	filenameLen = strlen(basefilepathname);
	filenameLen += 24; // ".Classified.summary.csv" + '\0';
	tmpFilenameBuff = new char[filenameLen];
	snprintf(tmpFilenameBuff, filenameLen, "%s.Classified.summary.csv", basefilepathname);
	string pathfilename(basefilepathname);
	string justfilename = pathfilename.substr(pathfilename.find_last_of("\\") + 1);
	justfilename = justfilename.substr(0, justfilename.find("."));
	ofstream locFile2(tmpFilenameBuff, (appendToSummaryFile)? ios::app : ios::out);
	if (!appendToSummaryFile) {
		for (int idx = 0; idx < allMovementRules.size(); idx++) {
			if (idx == 0) {
				locFile2 << "Video";
			}
			locFile2 << "," << allMovementRules[idx]->mName << ",,,," << allMovementRules[idx]->mName << "_speed,,";
		}
		locFile2 << "\n";
		for (int idx = 0; idx < allMovementRules.size(); idx++) {
			if (idx == 0) {
				locFile2 << "Video";
			}
			locFile2 << ",Nb events,Total,Average,Variance(S.E),Total,Average,Variance(S.E)";
		}
		locFile2 << "\n";
	}
	for (int idx = 0; idx < allMovementRules.size(); idx++) {
		if (idx == 0) {
			locFile2 << justfilename;
		}
		locFile2 << "," << ruleTotalisers[idx]->getCount() << ", " << ruleTotalisers[idx]->getTotalTime() << ", " << ruleTotalisers[idx]->getAverageTime() << ", " << ruleTotalisers[idx]->getVarriance();
		locFile2  << ","<< ruleTotalisers[idx]->getTotalSpeed() << ", " << ruleTotalisers[idx]->getAverageSpeed() << ", " << ruleTotalisers[idx]->getSpeedVarriance();
	}
	locFile2 << "\n";
	locFile2.close();
	return retv;

}
void fpClassifier::infillRulesAndClassifications(ofstream *pLocFile, float previousFrameTime, float nextLocTime, float timeIncrement, vector<clsfctnTotaliser *> *pRuleTotalisers) {
	point3dMm mtPt;
	mtPt.size = mtPt.aspectRatio = mtPt.RWx = mtPt.RWy = mtPt.RWz = 0;
	mtPt.speed = mtPt.RWxSmoothed = mtPt.RWySmoothed = mtPt.RWzSmoothed = 0;
	mtPt.directionX = mtPt.directionY = mtPt.directionZ = 0;
	mtPt.classification = -1;
	float runningTime = previousFrameTime;
	do {
		runningTime += timeIncrement;
		mtPt.fTime = runningTime;
		*pLocFile << locationsFile::write(&mtPt);
		for (int idx = 0; idx < allMovementRules.size(); idx++) {
			// now output the totaliser data
			*pLocFile << ", " << pRuleTotalisers->at(idx)->getCummulativeTime() << ", " << pRuleTotalisers->at(idx)->getEventTime();
		}
		*pLocFile << "\n";
		//for (int idx = 0; idx < allMovementRules.size(); idx++) {
		//	*pLocFile << ", 0, 0";
		//}
	} while (runningTime < nextLocTime);

}
void writeRulesAndClassifications(ofstream *locFile, std::vector<point3dMm> *pLocations, int locIdx) {

}

bool fpClassifier::saveExportableClassificationSet(std::vector<point3dMm> *pLocations, const char *className, const char *filepathname) {
	bool retv = false;
	int filenameLen = strlen(filepathname);
	filenameLen += strlen(className);
	filenameLen += 8; // '.' + "C." + '.' + txt + '\0';
	char *tmpFilenameBuff = new char[filenameLen];
	snprintf(tmpFilenameBuff, filenameLen, "%s.C.%s.txt", filepathname, className);
	ofstream locFile(tmpFilenameBuff, ios::out);
	int znIdx = 0;
	locFile << "Time,matchingRules,...\n";
	for (int locIdx = 0; locIdx < pLocations->size(); locIdx++) {
		bool locUsed = false;
		for each (classificationValidty cv in exportableClassifications)
		{
			if (cv.mValidClassification == className) {
				if (locIdx >= cv.mStartIdx && locIdx <= cv.mEndIdx) {
					for each (ruleValidity *rv in *pSimpleRuleResults)
					{
						if (locIdx >= rv->mStartIdx && locIdx <= rv->mEndIdx) {
							if (!locUsed)
								locFile << pLocations->at(locIdx).fTime; //locationsFile::write(&(pLocations->at(idx)));
							locUsed = true;
							locFile << "," << rv->mValidRule->mName;
						}
					}
				}
			}
		}
		if (locUsed)
			locFile << "\n";
	}
	retv = true;
	locFile.close();
	return retv;

}

fpClassifier::readResult fpClassifier::readClassifications(const char *filepathname) {
	readResult retv = nofile;
	//stringstream ss;
	//int lineNo = 0;
	//std::ifstream str;
	//clearAllVectors();
	//str.open(filepathname);
	//string oneRuleReadResult;
	//if (str.is_open()) {
	//	retv = formatErr;
	//	lineNo++;
	//	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	//	if (lineArray.size() != 1) {
	//		ss << "First line is not a version number\n";
	//		retv = readResult::formatErr;
	//	}
	//	else {
	//		int verNo = csvFile::parseLine1iSafely(lineArray[0], 1);
	//		switch (verNo) {
	//		case 1:
	//			for (;;) {
	//				lineArray = getNextLineAndSplitIntoTokens(str);
	//				if (lineArray.size() != 5) {
	//					classificationValidty *tmpcv = new classificationValidty();
	//					if (tmpcv->serialise(lineArray)) {
	//						vector<movementRule *> allMovementRules;
	//						vector<movementRule *> allCompoundMovementRules;
	//						vector<movementClassification *> allMovementClassifications;

	//					}
	//					else {

	//					}
	//				}
	//			}
	//			break;
	//		default:
	//			retv = readResult::formatErr;
	//			ss << "Line " << lineNo << ", " << lineArray[0] << " is not a recognised version number\n";
	//			break;
	//		}
	//	}
	//}
	//if (retv == readResult::formatErr) {
	//	mRuleReadResult = ss.str();
	//}

	return retv;
}


void fpClassifier::setZones(vector<zone3dCompound *> *pZones) {
	mZone3dCompounds = pZones;
}
zone3dCompound *fpClassifier::getZone3dAt(int idx) {
	int znIdx = 0;
	for each (zone3dCompound *zn in *mZone3dCompounds)
	{
		if (znIdx++ == idx) return zn;
	}
	return NULL;
}


//umZone *fpClassifier::getZoneAt(int idx) {
//	int znIdx = 0;
//	for each (umZone *zn in *mZones)
//	{
//		if (znIdx++ == idx) return zn;
//	}
//	return NULL;
//}

void fpClassifier::setSpeeds(float framesPerSec, float hover, float whizz, float sizeLimit, float feedPeriod) {
	mFramesPerSec = framesPerSec;
	mHoverSpeed = hover;
	mWhizzSpeed = whizz;
	mSizeLimit = sizeLimit;
	mFeedPeriod = feedPeriod;
}
//void fpClassifier::setZones(vector<umZone *> *pZones) {
//	mZones = pZones;
//}
void fpClassifier::setPerspective(perspective *persp) {
	mPersp = persp;
}

float fpClassifier::timePeriod(std::vector<point3dMm> *pLocs, int startIdx, int endIdx) {
	float retv = pLocs->at(endIdx).fTime - pLocs->at(startIdx).fTime;
	retv /= mFramesPerSec;
	return retv;
}

fpClassifier::clsfctnEvent fpClassifier::enterOrLeaveZone(locationState currentLocState, point3dMm *pLoc, bool innerDistinct, umZone **zoneEntered) {
	//int headX, headY;
	//float bodyToHeadDistance = Math::Sqrt(pLoc->size / Math::PI);// *0.75 * tmpBeeLoc.loc.aspectRatio;

	//headX = pLoc->RWx + bodyToHeadDistance * pLoc->directionX;
	//headY = pLoc->RWy - bodyToHeadDistance * pLoc->directionY;

	//if (currentLocState != locationState::away) {
	//	if (pLoc->size > mSizeLimit) {
	//		return clsfctnEvent::goaway;
	//	}
	//}
	//if ((currentLocState == locationState::idleLocation)) {
	//	int znIdx = 0;
	//	for each (umZone *zn in *mZones)
	//	{
	//		if (zn->isWithin(headX, headY, false)) {
	//			*zoneEntered = zn;
	//			return clsfctnEvent::enterZone;
	//		}
	//	}
	//} else if ((currentLocState == locationState::withinZone)) {
	//	if (zoneEntered != NULL && *zoneEntered != NULL) {
	//		if (innerDistinct && (*zoneEntered)->isWithin(headX, headY, true)) {
	//			return clsfctnEvent::enterInner;
	//		}
	//		else if ((*zoneEntered)->isWithin(headX, headY, false)) {
	//			return clsfctnEvent::none;
	//		}
	//		else {
	//			return clsfctnEvent::leaveZone;
	//		}
	//	}
	//} else if ((currentLocState == locationState::withinInner)) {
	//	if (zoneEntered != NULL && *zoneEntered != NULL) {
	//		if ((*zoneEntered)->isWithin(headX, headY, true)) {
	//			return clsfctnEvent::none;
	//		}
	//		else if ((*zoneEntered)->isWithin(headX, headY, false)) {
	//			return (innerDistinct) ? clsfctnEvent::leaveInner : clsfctnEvent::none;
	//		}
	//		else {
	//			return clsfctnEvent::leaveZone;
	//		}
	//	}
	//}
	//else if (currentLocState == locationState::away) {
	//	if (pLoc->size < mSizeLimit) {
	//		return clsfctnEvent::comeclose;
	//	}

	//}
	return clsfctnEvent::none;
}
bool fpClassifier::locationLookAheadNotRevert(std::vector<point3dMm> *pLocations, int locIdx, locationState currentLocState, umZone *zoneEntered) {
	bool retv = true;
	float startTime = pLocations->at(locIdx).fTime;
	while (++locIdx < pLocations->size()) {
		clsfctnEvent locEvent;
		float tmpTime = pLocations->at(locIdx).fTime;
		if (tmpTime - startTime > zoneHysteresisSec) break;
		locEvent = enterOrLeaveZone(currentLocState, &(pLocations->at(locIdx)), true, &zoneEntered);
		if (locEvent == clsfctnEvent::none) {
			return false;
		}
	}
	return retv;
}
bool fpClassifier::speedLookAheadNotRevert(std::vector<point3dMm> *pLocations, int locIdx, motionState currentMoveState) {
	bool retv = true;
	float startTime = pLocations->at(locIdx).fTime;
	while (++locIdx < pLocations->size()) {
		clsfctnEvent locEvent;
		float tmpTime = pLocations->at(locIdx).fTime;
		if (tmpTime - startTime > zoneHysteresisSec) break;
		locEvent = checkSpeedChange(currentMoveState, &(pLocations->at(locIdx)));
		if (locEvent == clsfctnEvent::none) {
			return false;
		}
	}
	return retv;
}
fpClassifier::clsfctnEvent fpClassifier::checkSpeedChange(motionState currentMoveState, point3dMm *pLoc) {
	clsfctnEvent retv = clsfctnEvent::none;
	if (currentMoveState == motionState::idleMotion) {
		if (pLoc->speed < mHoverSpeed) {
			retv = clsfctnEvent::startHover;
		}
		else if ((pLoc->speed >= mHoverSpeed) && (pLoc->speed <= mWhizzSpeed)) {
			retv = clsfctnEvent::endHover ;
		}
		else {
			retv = clsfctnEvent::startWhizz;
		}
	} else if (currentMoveState == motionState::hovering) {
		if (pLoc->speed < mHoverSpeed) {
			retv = clsfctnEvent::none;
		}
		else  {
			retv = clsfctnEvent::endHover ;
		}
	} else if (currentMoveState == motionState::normalMovement) {
		if (pLoc->speed < mHoverSpeed) {
			retv = clsfctnEvent::startHover;
		}
		else if ((pLoc->speed >= mHoverSpeed) && (pLoc->speed <= mWhizzSpeed)) {
			retv = clsfctnEvent::none ;
		}
		else {
			retv = clsfctnEvent::startWhizz;
		}
	} else if (currentMoveState == motionState::whizzing) {
		if ((pLoc->speed <= mWhizzSpeed)) {
			retv = clsfctnEvent::endWhizz ;
		}
		else {
			retv = clsfctnEvent::none;
		}
	}
	return retv;
}
movementRule *fpClassifier::findNamedRule(string ruleName) {
	movementRule *retv = nullptr;
	std::transform(ruleName.begin(), ruleName.end(), ruleName.begin(), ::toupper);

	for (int idx = 0; idx < allMovementRules.size(); idx++) {
		string tstName = allMovementRules[idx]->mName;
		std::transform(tstName.begin(), tstName.end(), tstName.begin(), ::toupper);

		if (ruleName == tstName) {
			retv = allMovementRules[idx];
			break;
		}
	}
	return retv;
}

movementClassification *fpClassifier::findNamedClassification(string ruleName) {
	movementClassification *retv = nullptr;
	std::transform(ruleName.begin(), ruleName.end(), ruleName.begin(), ::toupper);

	for (int idx = 0; idx < allMovementClassifications.size(); idx++) {
		string tstName = allMovementClassifications[idx]->mName;
		std::transform(tstName.begin(), tstName.end(), tstName.begin(), ::toupper);
		if (ruleName == tstName) {
			retv = allMovementClassifications[idx];
			break;
		}
	}
	return retv;
}
int fpClassifier::findRuleClassificationIdx(movementRule *srch) {
	int retv = -1;

	for (int idx = 0; idx < allMovementClassifications.size(); idx++) {
		if (srch == allMovementClassifications[idx]->pRule) {
			retv = idx;
			break;
		}
	}
	return retv;
}

bool fpClassifier::parse(std::vector<point3dMm> *pLocations, vector<zone3dCompound *> *pZonesToParse) {
	bool retv = false;
	vector<zoneClassifications *> *pClassifications = new vector<zoneClassifications *>();
	// clear all classifications,
	int unclassifiedIdx = findRuleClassificationIdx(nullptr);
	for (int idx = 0; idx < pLocations->size(); idx++) {
		pLocations->at(idx).classification = unclassifiedIdx;
	}
	if (pRuleEntryExits != nullptr)
		delete pRuleEntryExits;
	pRuleEntryExits = new vector<vector<clsfctnEntryOrExit *> *>();
	if (pSimpleRuleResults != nullptr)
		delete pSimpleRuleResults;
	pSimpleRuleResults = new vector<ruleValidity *>();
	for each (zone3dCompound *pZn in *pZonesToParse) {
		vector<clsfctnEntryOrExit *> *pOneZoneEntryExits = new vector<clsfctnEntryOrExit *>();
		parseOneZoneForSimpleRules(pLocations, pSimpleRuleResults, pOneZoneEntryExits, pZn);
		pRuleEntryExits->push_back(pOneZoneEntryExits);
	}
	// now have all simple rules, each processed within a zone
	if (pSimpleRuleResults != nullptr && pSimpleRuleResults->size() > 0) {
		retv = compoundFromSimpleRules(pLocations, pSimpleRuleResults);
	}
	if (!retv) {
		pClassifications->clear();
	}
	// now create classifications from the rule hierrchy
	if (pSimpleRuleResults != nullptr && pSimpleRuleResults->size() > 0) {
		for each (ruleValidity *rv in *pSimpleRuleResults) {
			int classificationIdx;
			if ((classificationIdx = findRuleClassificationIdx(rv->mValidRule)) != -1) {
				classificationValidty tmpClassificationValid(rv->mStartIdx, rv->mEndIdx, rv->mValidRule->mName, allMovementClassifications[classificationIdx]->mName);
				exportableClassifications.push_back(tmpClassificationValid);
				if (!(allMovementClassifications[classificationIdx]->mColourB == 0 && 
					allMovementClassifications[classificationIdx]->mColourG == 0 && 
					allMovementClassifications[classificationIdx]->mColourR == 0)) {
					for (int idx = rv->mStartIdx; idx <= rv->mEndIdx; idx++) {
						pLocations->at(idx).classification = classificationIdx;
					}
				}
			}
		}
	}
	return retv;
}

bool fpClassifier::findAllPointsForOneRuleInOneZone(std::vector<point3dMm> *pLocations, zone3d *zoneToParse, movementRule *mvr, vector<zoneReleventLocations> *pLocsWithinZone, vector<zoneReleventLocations> *pLocsBefore, vector<zoneReleventLocations> *pLocsAfter, float sizeTolerance, float preTimeTolerance, float postTimeTolerance) {
	int previousEndIdx = -1;
	bool foundBand = true;
	bool includeExclusions = ! mvr->scopeIsX();
	bool doZoneNotSUbzone = mvr->scopeIsZone() ;
	do {
		zoneReleventLocations nextBand = findNextPoints(pLocations, previousEndIdx + 1, zoneToParse, doZoneNotSUbzone, includeExclusions, sizeTolerance);
		if (locBandValid(&nextBand)) {
			previousEndIdx = nextBand.locIdxEnd;
			pLocsWithinZone->push_back(nextBand);
			zoneReleventLocations preBand(-1, nextBand.locIdxStart);
			if (preTimeTolerance < 0) {
				preBand.locIdxStart = lookBackOrForward(pLocations, nextBand.locIdxStart, zoneToParse, preTimeTolerance);
				if (locBandValid(&preBand)) {
					pLocsBefore->push_back(preBand);
				}
			}
			if (postTimeTolerance > 0) {
				zoneReleventLocations postBand(nextBand.locIdxEnd, -1);
				postBand.locIdxEnd = lookBackOrForward(pLocations, nextBand.locIdxEnd, zoneToParse, postTimeTolerance);
				if (locBandValid(&postBand)) {
					pLocsAfter->push_back(postBand);
				}
			}
		}
		else {
			foundBand = false;
		}
	} while (foundBand);
	return foundBand;
}
int fpClassifier::lookBackOrForward(std::vector<point3dMm> *pLocations, int startidx, zone3d *zoneToParse, float timeDelta) {
	int retv = startidx;
	float timeLimit;
	int stepIdx = 1;
	timeLimit = pLocations->at(startidx).fTime + timeDelta;
	if (timeDelta < 0) {
		stepIdx = -1;
	}
	else {
		stepIdx = 1;
	}
	for (int idx = startidx; idx >= 0 && idx < pLocations->size(); idx += stepIdx) {
		point3dMm *tmpLoc = &(pLocations->at(idx));
		if ((timeDelta > 0)? (tmpLoc->fTime >= timeLimit) : (tmpLoc->fTime <= timeLimit)) {
			break;
		}
		if (zoneToParse->isWithin(tmpLoc->RWxSmoothed, tmpLoc->RWySmoothed, tmpLoc->RWzSmoothed, 1.0)) {
			// track has returned to the zone?
			break;
		}
		retv = idx;
	}
	return retv;
}

zoneReleventLocations fpClassifier::findNextPoints(std::vector<point3dMm> *pLocations, int startidx, zone3d *zoneToParse, bool zoneNotSubzone, bool includeExclusions, float sizeTolerance) {
	bool inSignificantBand = false;
	zoneReleventLocations retv(-1, -1);
	if ((zoneNotSubzone && !zoneToParse->isSubZone()) || (!zoneNotSubzone && zoneToParse->isSubZone())) {
		for (int idx = startidx; idx < pLocations->size(); idx++) {
			point3dMm *tmpLoc = &(pLocations->at(idx));
			bool ptIsWithin = false;
			if (zoneToParse->isSubZone()) {
				if (includeExclusions || !zoneToParse->mExclusion) {
					ptIsWithin = zoneToParse->isWithin(tmpLoc->RWxSmoothed, tmpLoc->RWySmoothed, tmpLoc->RWzSmoothed, sizeTolerance);
					if (ptIsWithin && !includeExclusions) { // now chech this point is not excluded by an exclusion zone in our parent
						ptIsWithin = ((zone3dCompound *)zoneToParse->mParent)->isWithin(tmpLoc->RWxSmoothed, tmpLoc->RWySmoothed, tmpLoc->RWzSmoothed, sizeTolerance);
					}
				}
			}
			else {
				if (includeExclusions) {
					ptIsWithin = ((zone3dCompound *)zoneToParse)->isWithinIgnoreExcludes(tmpLoc->RWxSmoothed, tmpLoc->RWySmoothed, tmpLoc->RWzSmoothed, sizeTolerance);
				} else {
					ptIsWithin = ((zone3dCompound *)zoneToParse)->isWithin(tmpLoc->RWxSmoothed, tmpLoc->RWySmoothed, tmpLoc->RWzSmoothed, sizeTolerance);
				}
			}
			if (ptIsWithin) {
				if (inSignificantBand) {
					retv.locIdxEnd = idx;
				}
				else {
					inSignificantBand = true;
					retv.locIdxStart = retv.locIdxEnd = idx;
				}
			}
			else {
				if (inSignificantBand) {
					inSignificantBand = false;
					retv.locIdxEnd = idx;
					break; // was in the zone, and now left it..so search is done
				}
			}
		}
	}
	return retv;
}
float fpClassifier::pointToPointSpeedSq(std::vector<point3dMm> *pLocations, int idx) {
	float retv = -1;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float deltaTime = pt2->fTime - pt1->fTime;
		float distanceX = pt2->RWxSmoothed - pt1->RWxSmoothed;
		float distanceY = pt2->RWySmoothed - pt1->RWySmoothed;
		float distanceZ = pt2->RWzSmoothed - pt1->RWzSmoothed;
		retv = (distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ) / (deltaTime * deltaTime);
	}
	return retv;
}
float fpClassifier::rawPointToPointSpeedSq(std::vector<point3dMm> *pLocations, int idx) {
	float retv = -1;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float deltaTime = pt2->fTime - pt1->fTime;
		float distanceX = pt2->RWx - pt1->RWx;
		float distanceY = pt2->RWy - pt1->RWy;
		float distanceZ = pt2->RWz - pt1->RWz;
		retv = (distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ) / (deltaTime * deltaTime);
	}
	return retv;
}
float fpClassifier::pointToPointDirection(std::vector<point3dMm> *pLocations, int idx) {
	float retv = 0;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float distanceX = pt2->RWxSmoothed - pt1->RWxSmoothed;
		float distanceY = pt2->RWySmoothed - pt1->RWySmoothed;
//		float distanceZ = pt2->RWzSmoothed - pt1->RWzSmoothed;
		retv = zone3d::angleDegMod360(zone3d::angleDegFromCartesianXY(distanceX, distanceY));
	}
	return retv;
}
float fpClassifier::pointToPointDirectionZ(std::vector<point3dMm> *pLocations, int idx) {
	float retv = 0;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float distanceX = pt2->RWxSmoothed - pt1->RWxSmoothed;
		float distanceY = pt2->RWySmoothed - pt1->RWySmoothed;
		float distanceZ = Math::Abs(pt2->RWzSmoothed - pt1->RWzSmoothed);
		float distanceXandY = distanceX * distanceX + distanceY * distanceY;
		distanceXandY = Math::Sqrt(distanceXandY);
		retv = zone3d::angleDegMod360(zone3d::angleDegFromCartesianXY( distanceZ, distanceXandY));
	}
	return retv;
}
float fpClassifier::pointToPointDirectionPolar(std::vector<point3dMm> *pLocations, int idx, float orgX, float orgY) {
	float retv = 0;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float polarDegToPt1 = zone3d::angleDegFromCartesianXY(pt1->RWxSmoothed - orgX, pt1->RWySmoothed - orgY);
		float polarDegPt1ToPt2 = zone3d::angleDegFromCartesianXY(pt2->RWxSmoothed - pt1->RWxSmoothed, pt2->RWySmoothed - pt1->RWySmoothed);
		retv = zone3d::angleDegMod360(polarDegPt1ToPt2 - polarDegToPt1);
	}
	return retv;
}
float fpClassifier::pointToPointSpeedRadial(std::vector<point3dMm> *pLocations, int idx, float orgX, float orgY) {
	float retv = 0;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float deltaTime = pt2->fTime - pt1->fTime;
		float distanceX = pt1->RWxSmoothed - orgX;
		float distanceY = pt1->RWySmoothed - orgY;
		float radius1 = Math::Sqrt(distanceX * distanceX + distanceY * distanceY);
		distanceX = pt2->RWxSmoothed - orgX;
		distanceY = pt2->RWySmoothed - orgY;
		float radius2 = Math::Sqrt(distanceX * distanceX + distanceY * distanceY);
		retv = (radius2 - radius1) / deltaTime;
	}
	return retv;
}
float fpClassifier::pointToPointSpeedTangential(std::vector<point3dMm> *pLocations, int idx, float orgX, float orgY) {
	float retv = 0;
	if (idx >= 0 && idx < (pLocations->size() - 1)) {
		point3dMm *pt1 = &(pLocations->at(idx));
		point3dMm *pt2 = &(pLocations->at(idx + 1));
		float deltaTime = pt2->fTime - pt1->fTime;
		float polarDegToPt1 = zone3d::angleDegFromCartesianXY(pt1->RWxSmoothed - orgX, pt1->RWySmoothed - orgY);
		float polarDegToPt2 = zone3d::angleDegFromCartesianXY(pt1->RWxSmoothed - orgX, pt1->RWySmoothed - orgY);
		retv = zone3d::angleDegMod360(polarDegToPt2 - polarDegToPt1) / deltaTime;
	}
	return retv;
}

vector<zoneReleventLocations> *fpClassifier::oneZoneSimpleRuleMultiple(std::vector<point3dMm> *pLocations, vector<zoneReleventLocations> *pLocsWithinZone, movementRule *ruleToTest, zone3d *zoneToParse) {
	// arguments are the whole list of points, and a list of those sections that are within the zone
	// it may be that there are gaps in the sections which we need to stitch over depending on the degree of compliance mandated in the rule
	// first stage is to build a new collection of sections which strictly satisfy the rule
	vector<zoneReleventLocations> *pLocsWithinRule = new vector<zoneReleventLocations>();
	if (ruleToTest->mUserZoneType == USER_ZONE_ANY || ruleToTest->mUserZoneType == zoneToParse->mUserDefinedType) {
		float minSq = ruleToTest->mMinimum * ruleToTest->mMinimum;
		float maxSq = ruleToTest->mMaximum * ruleToTest->mMaximum;
		zoneReleventLocations tmpValidSpeed(-1, -1);
		float orgX = zoneToParse->Centre().x;
		float orgY = zoneToParse->Centre().y;
		int locationsLimit = pLocations->size() - 1;
		float deltaAngleMinToMax = 0;
		switch (ruleToTest->mType) {
		case movementRule::ruleDirectionR:
		case movementRule::ruleDirectionZ:
		case movementRule::ruleDirection:
		case movementRule::ruleDirectionP:
			deltaAngleMinToMax = zone3d::angleDegMod360(ruleToTest->mMaximum - ruleToTest->mMinimum);
			break;
		}

		for each (zoneReleventLocations pt in *pLocsWithinZone) {
			float timeStart = -1;
			for (int idx = pt.locIdxStart; idx < pt.locIdxEnd && idx < locationsLimit; idx++) {
				bool lclTestResult = false;
				float instantTestResult = 0;
				bool breakOut = false;
				switch (ruleToTest->mType) {
				case movementRule::ruleInside:
					if (timeStart < 0) {
						timeStart = pLocations->at(idx).fTime;
					}
					lclTestResult = (pLocations->at(idx).fTime >= ruleToTest->mMinimum && pLocations->at(idx).fTime <= ruleToTest->mMaximum);
					break;
				case movementRule::ruleSpeed:
					instantTestResult = pointToPointSpeedSq(pLocations, idx);
					lclTestResult = (instantTestResult >= minSq && instantTestResult <= maxSq);
					break;
				case movementRule::ruleDirectionR:
					instantTestResult = -zoneToParse->getAngleDeg();
					// ******** Fall Through ************
				case movementRule::ruleDirection:
					// incremental calculation starting from either 0 or - host angle
					instantTestResult += pointToPointDirection(pLocations, idx);
					lclTestResult = zone3d::angleDegIsBetween(instantTestResult, ruleToTest->mMinimum, deltaAngleMinToMax);
					break;
				case movementRule::ruleDirectionZ:
					// incremental calculation starting from  0
					instantTestResult += pointToPointDirectionZ(pLocations, idx);
					lclTestResult = zone3d::angleDegIsBetween(instantTestResult, ruleToTest->mMinimum, deltaAngleMinToMax);
					break;
				case movementRule::ruleDuration:
					instantTestResult = pLocations->at(pt.locIdxEnd).fTime - pLocations->at(pt.locIdxStart).fTime;
					if (instantTestResult >= ruleToTest->mMinimum && instantTestResult <= ruleToTest->mMaximum) {
						tmpValidSpeed.locIdxStart = pt.locIdxStart;
						tmpValidSpeed.locIdxEnd = pt.locIdxEnd;
						pLocsWithinRule->push_back(tmpValidSpeed);
					}
					breakOut = true; // no need to iterate throguh the points
					break;
				case movementRule::ruleDirectionP:
					instantTestResult = pointToPointDirectionPolar(pLocations, idx, orgX, orgY);
					lclTestResult = zone3d::angleDegIsBetween(instantTestResult, ruleToTest->mMinimum, deltaAngleMinToMax);
					break;
				case movementRule::ruleSpeedR:
					instantTestResult = pointToPointSpeedRadial(pLocations, idx, orgX, orgY);
					lclTestResult = (instantTestResult >= ruleToTest->mMinimum && instantTestResult <= ruleToTest->mMaximum);
					break;
				case movementRule::ruleSpeedT:
					instantTestResult = pointToPointSpeedTangential(pLocations, idx, orgX, orgY);
					lclTestResult = (instantTestResult >= ruleToTest->mMinimum && instantTestResult <= ruleToTest->mMaximum);
					break;
				case movementRule::ruleDistanceZ:
					instantTestResult = pLocations->at(idx).RWzSmoothed ;
					lclTestResult = (instantTestResult >= ruleToTest->mMinimum && instantTestResult <= ruleToTest->mMaximum);
					break;
				case movementRule::ruleHalfTop:
					// for all of these we know the point is within the zone, now just need to tesst where relative to the centre
					lclTestResult = (pLocations->at(idx).RWy >= orgY);
					if (lclTestResult) { // now do the speed part of the scan rule
						instantTestResult = pointToPointSpeedSq(pLocations, idx);
						lclTestResult = (instantTestResult >= minSq && instantTestResult <= maxSq);
					}
					break;
				case movementRule::ruleHalfBottom:
					lclTestResult = (pLocations->at(idx).RWy < orgY);
					if (lclTestResult) { // now do the speed part of the scan rule
						instantTestResult = pointToPointSpeedSq(pLocations, idx);
						lclTestResult = (instantTestResult >= minSq && instantTestResult <= maxSq);
					}
					break;
				case movementRule::ruleHalfLeft:
					lclTestResult = (pLocations->at(idx).RWx < orgX);
					if (lclTestResult) { // now do the speed part of the scan rule
						instantTestResult = pointToPointSpeedSq(pLocations, idx);
						lclTestResult = (instantTestResult >= minSq && instantTestResult <= maxSq);
					}
					break;
				case movementRule::ruleHalfRight:
					lclTestResult = (pLocations->at(idx).RWx >= orgX);
					if (lclTestResult) { // now do the speed part of the scan rule
						instantTestResult = pointToPointSpeedSq(pLocations, idx);
						lclTestResult = (instantTestResult >= minSq && instantTestResult <= maxSq);
					}
					break;
				}
				if (lclTestResult) {
					// test is within band...latch the start if not yet begun
					if (tmpValidSpeed.locIdxStart == -1) {
						tmpValidSpeed.locIdxStart = idx;
					}
					tmpValidSpeed.locIdxEnd = idx; // +1;
				}
				else {
					// speed is beyond bounds, so stop band if we started it
					if (tmpValidSpeed.locIdxStart != -1) {
						pLocsWithinRule->push_back(tmpValidSpeed);
					}
					tmpValidSpeed.locIdxStart = -1; // reset ready for another band detection
				}
			}
			if (tmpValidSpeed.locIdxStart != -1) {
				pLocsWithinRule->push_back(tmpValidSpeed);
			}
			tmpValidSpeed.locIdxStart = -1; // reset ready for another band detection

		}
	}
	return pLocsWithinRule;
}
//vector<zoneReleventLocations> *fpClassifier::simpleRuleDirection(std::vector<point3dMm> *pLocations, vector<zoneReleventLocations> *pLocsWithinZone, movementRule *ruleToTest, zone3d *zoneToParse) {
//	// arguments are the whole list of points, and a list of those sections that are within the zone
//	// it may be that there are gaps in the sections which we need to stitch over depending on the degree of compliance mandated in the rule
//	// first stage is to build a new collection of sections which strictly satisfy the rule
//	bool adjustForZone = false;
//	if (ruleToTest->mType == movementRule::ruleType::ruleDirectionR) adjustForZone = true;
//	vector<zoneReleventLocations> *pLocsWithinRule = new vector<zoneReleventLocations>();
//	zoneReleventLocations tmpValidDir(-1, -1);
//	int locationsLimit = pLocations->size() - 1;
//	for each (zoneReleventLocations pt in *pLocsWithinZone) {
//		for (int idx = pt.locIdxStart; idx <= pt.locIdxEnd && idx < locationsLimit; idx++) {
//			float instantDir = pointToPointDirection(pLocations, idx);
//			if (adjustForZone) instantDir -= zoneToParse->getAngleDeg();
//			if (instantDir >= ruleToTest->mMinimum && instantDir <= ruleToTest->mMaximum) {
//				// speed is within band...latch the start if not yet begun
//				if (tmpValidDir.locIdxStart == -1) {
//					tmpValidDir.locIdxStart = idx;
//				}
//				tmpValidDir.locIdxEnd = idx + 1;
//			}
//			else {
//				// speed is beyond bounds, so stop band if we started it
//				if (tmpValidDir.locIdxStart != -1) {
//					pLocsWithinRule->push_back(tmpValidDir);
//				}
//				tmpValidDir.locIdxStart = -1; // reset ready for another band detection
//			}
//		}
//		if (tmpValidDir.locIdxStart != -1) {
//			pLocsWithinRule->push_back(tmpValidDir);
//		}
//		tmpValidDir.locIdxStart = -1; // reset ready for another band detection
//	}
//	return pLocsWithinRule;
//}

vector<zoneReleventLocations> *fpClassifier::amalgamate(std::vector<point3dMm> *pLocations, vector<zoneReleventLocations> *pLocsWithinRule, movementRule *ruleToTest) {
	bool amalgamationToBeDone = false;
	vector<zoneReleventLocations> *pLocsAmalgamated = new vector<zoneReleventLocations>();
	int ruleIdxLimit = (pLocsWithinRule->size() - 1);
	for (int idx = 0; idx < ruleIdxLimit; idx++) {
		int IdxStart = pLocsWithinRule->at(idx).locIdxStart;
		int IdxPlus1Start = pLocsWithinRule->at(idx + 1).locIdxStart;
		int IdxEnd = pLocsWithinRule->at(idx).locIdxEnd;
		int IdxPlus1End = pLocsWithinRule->at(idx + 1).locIdxEnd;
		zoneReleventLocations locIdx = pLocsWithinRule->at(idx);
		zoneReleventLocations locIdxPlus1 = pLocsWithinRule->at(idx + 1);
		float fTimeIdxStart = pLocations->at(IdxStart).fTime;
		float fTimeIdxPlus1Start = pLocations->at(IdxPlus1Start).fTime;
		float fTimeIdxEnd = pLocations->at(IdxEnd).fTime;
		float fTimeIdxPlus1End = pLocations->at(IdxPlus1End).fTime;
		float potentialMergedTime = fTimeIdxPlus1End - fTimeIdxStart;
		float potentialMissingTime = fTimeIdxPlus1Start - fTimeIdxEnd;
		potentialMergedTime = pLocations->at(pLocsWithinRule->at(idx + 1).locIdxEnd).fTime - pLocations->at(pLocsWithinRule->at(idx).locIdxStart).fTime;
		potentialMissingTime = pLocations->at(pLocsWithinRule->at(idx + 1).locIdxStart).fTime - pLocations->at(pLocsWithinRule->at(idx).locIdxEnd).fTime;
//		if (potentialMissingTime  <= ruleToTest->mRequiredPortion) {
		if (potentialMergedTime > 0 && ((potentialMissingTime / potentialMergedTime) <= (ruleToTest->mRequiredPortion))) {
			// less of a gap than the rule limit, so amalgamate
			pLocsWithinRule->at(idx).locIdxEnd = -1; // flag this to be amalgamated
			amalgamationToBeDone = true;
		}
	}
	//for (int idx = ruleIdxLimit; idx > 0; idx--) {
	//	int IdxStart = pLocsWithinRule->at(idx).locIdxStart;
	//	int IdxMinus1Start = pLocsWithinRule->at(idx - 1).locIdxStart;
	//	int IdxEnd = pLocsWithinRule->at(idx).locIdxEnd;
	//	int IdxMinus1End = pLocsWithinRule->at(idx - 1).locIdxEnd;
	//	if (IdxMinus1End == -1 || IdxEnd == -1) 
	//		continue;
	//	zoneReleventLocations locIdx = pLocsWithinRule->at(idx);
	//	zoneReleventLocations locIdxMinus1 = pLocsWithinRule->at(idx - 1);
	//	float fTimeIdxStart = pLocations->at(IdxStart).fTime;
	//	float fTimeIdxMinus1Start = pLocations->at(IdxMinus1Start).fTime;
	//	float fTimeIdxEnd = pLocations->at(IdxEnd).fTime;
	//	float fTimeIdxMinus1End = pLocations->at(IdxMinus1End).fTime;
	//	float potentialMergedTime = fTimeIdxMinus1Start - fTimeIdxEnd;
	//	float potentialMissingTime = fTimeIdxMinus1End - fTimeIdxStart;
	//	potentialMergedTime = pLocations->at(pLocsWithinRule->at(idx - 1).locIdxStart).fTime - pLocations->at(pLocsWithinRule->at(idx).locIdxEnd).fTime;
	//	potentialMissingTime = pLocations->at(pLocsWithinRule->at(idx - 1).locIdxEnd).fTime - pLocations->at(pLocsWithinRule->at(idx).locIdxStart).fTime;
	//	if (potentialMissingTime / potentialMergedTime <= (1 - ruleToTest->mRequiredPortion)) {
	//		// less of a gap than the rule limit, so amalgamate
	//		pLocsWithinRule->at(idx-1).locIdxEnd = -1; // flag this to be amalgamated
	//		amalgamationToBeDone = true;
	//	}
	//}
	if (amalgamationToBeDone) {
		int sectionStart;
		int sectionEnd = -1;
		sectionStart = -1;
		for (int idx = 0; idx < pLocsWithinRule->size(); idx++) {
			if (pLocsWithinRule->at(idx).locIdxEnd == -1 && idx < ruleIdxLimit) {
				sectionEnd = pLocsWithinRule->at(idx + 1).locIdxEnd;
				if (sectionStart == -1) {
					sectionStart = pLocsWithinRule->at(idx).locIdxStart;
				}
			}
			else {
				if (sectionEnd == -1) {
					pLocsAmalgamated->push_back(pLocsWithinRule->at(idx));
				}
				else {
					zoneReleventLocations amalgamated(sectionStart, pLocsWithinRule->at(idx).locIdxEnd);
					pLocsAmalgamated->push_back(amalgamated);
					sectionStart = -1;
					sectionEnd = -1;
				}
			}
		}
	}

	return pLocsAmalgamated;
}

bool fpClassifier::parseOneZoneForSimpleRules(std::vector<point3dMm> *pLocations, vector<ruleValidity* > *pSimpleRuleValidities, vector<clsfctnEntryOrExit *> *pOneZoneEntryExits, zone3d *zoneToParse) {
	bool atLeastOneFound = false;;
	vector<zoneReleventLocations> *pLocsBefore = new vector<zoneReleventLocations>();
	vector<zoneReleventLocations> *pLocsAfter = new vector<zoneReleventLocations>();
	vector<ruleValidity* > AdditionalSimpleRuleValidities;
	float preTimeTolerance = 0;
	float postTimeTolerance = 0;
	for each (movementRule *mvr in allMovementRules) {
		if (mvr->mType == movementRule::ruleType::ruleOutside) {
			if (mvr->mMaximum < 0 && mvr->mMaximum < preTimeTolerance) {
				preTimeTolerance = mvr->mMaximum;
			}
			else if (mvr->mMaximum > 0 && mvr->mMaximum > postTimeTolerance) {
				postTimeTolerance = mvr->mMaximum;
			}
		}
	}

	for each (movementRule *mvr in allMovementRules) {
		AdditionalSimpleRuleValidities.clear();
		bool oneMoreDone = false;
		if (mvr->scopeIsSubZone()) {
			if (((zone3dCompound *)zoneToParse)->hasSubZones()) {
				vector<zone3d *> *tmpSubZones = ((zone3dCompound *)zoneToParse)->subZones();
				for each (zone3d *sz in *tmpSubZones) {
					oneMoreDone = findAllPointsForOneRuleInOneZoneOrSubzone(pLocations, sz, mvr, pLocsBefore, pLocsAfter, pSimpleRuleValidities, &AdditionalSimpleRuleValidities, ZONE_AREA_TOLERANCE, preTimeTolerance, postTimeTolerance);
					atLeastOneFound = atLeastOneFound || oneMoreDone;
				}
			}
		}
		else {
			oneMoreDone = findAllPointsForOneRuleInOneZoneOrSubzone(pLocations, zoneToParse, mvr, pLocsBefore, pLocsAfter, pSimpleRuleValidities, &AdditionalSimpleRuleValidities, ZONE_AREA_TOLERANCE, preTimeTolerance, postTimeTolerance);
			atLeastOneFound = atLeastOneFound || oneMoreDone;
		}
		if (oneMoreDone) {
			// some points for one rule in this zone, so we will have an entry and exit point
			for each (ruleValidity *pval in AdditionalSimpleRuleValidities)
			{
				pOneZoneEntryExits->push_back(new clsfctnEntryOrExit(true, pval->mZoneWhereValid->angleFromCentreDegFromCartesianXY(pLocations->at(pval->mStartIdx).RWxSmoothed, pLocations->at(pval->mStartIdx).RWySmoothed),
					pLocations->at(pval->mStartIdx).fTime, pval->mZoneWhereValid->mID, mvr->mUserZoneType, mvr->mName));
				
				pOneZoneEntryExits->push_back(new clsfctnEntryOrExit(false, pval->mZoneWhereValid->angleFromCentreDegFromCartesianXY(pLocations->at(pval->mEndIdx).RWxSmoothed, pLocations->at(pval->mEndIdx).RWySmoothed),
					pLocations->at(pval->mEndIdx).fTime, pval->mZoneWhereValid->mID, mvr->mUserZoneType, mvr->mName));
			}
		}
	}
	return atLeastOneFound;
}
bool fpClassifier::findAllPointsForOneRuleInOneZoneOrSubzone(std::vector<point3dMm> *pLocations, zone3d *zoneToParse, movementRule *mvr, vector<zoneReleventLocations> *pLocsBefore, vector<zoneReleventLocations> *pLocsAfter, vector<ruleValidity* > *pSimpleRuleValidities, vector<ruleValidity* > *pAdditionalSimpleRuleValidities, float sizeTolerance, float preTimeTolerance, float postTimeTolerance) {
	bool atLeastOneFound = false;;
	vector<zoneReleventLocations> *pLocsWithinZone = new vector<zoneReleventLocations>();
	// first step is to find all points that coincide with the zone in question, and are outside to the degree required by the outside.timelimit
	findAllPointsForOneRuleInOneZone(pLocations, zoneToParse, mvr, pLocsWithinZone, pLocsBefore, pLocsAfter, ZONE_AREA_TOLERANCE, preTimeTolerance, postTimeTolerance);
	vector<zoneReleventLocations> *pLocsWithinRule = nullptr;
	switch (mvr->mType) {
	case movementRule::ruleType::ruleOutside:
		break;
	case movementRule::ruleType::ruleInside:
	case movementRule::ruleType::ruleDuration:
	case movementRule::ruleType::ruleDirection:
	case movementRule::ruleType::ruleDirectionR:
	case movementRule::ruleType::ruleSpeed:
	case movementRule::ruleType::ruleSpeedR:
	case movementRule::ruleType::ruleSpeedT:
	case movementRule::ruleType::ruleDirectionP:
	case movementRule::ruleType::ruleDirectionZ:
	case movementRule::ruleType::ruleDistanceZ:
	case movementRule::ruleType::ruleHalfTop:
	case movementRule::ruleType::ruleHalfBottom:
	case movementRule::ruleType::ruleHalfLeft:
	case movementRule::ruleType::ruleHalfRight:
		pLocsWithinRule = oneZoneSimpleRuleMultiple(pLocations, pLocsWithinZone, mvr, zoneToParse);
		break;
	}
	if (pLocsWithinRule != nullptr && pLocsWithinRule->size() > 0) {
		vector<zoneReleventLocations> *pLocsAmalgamated = amalgamate(pLocations, pLocsWithinRule, mvr);
		if (pLocsAmalgamated->size() == 0) {
			delete pLocsAmalgamated;
		}
		else {
			delete pLocsWithinRule;
			pLocsWithinRule = pLocsAmalgamated;
		}
		// the rule is valid for at least some points 
		for each (zoneReleventLocations pt in *pLocsWithinRule) {
			if (pt.locIdxEnd > pt.locIdxStart) { // skip singletons
				ruleValidity *tmpRuleValidity = new ruleValidity(pt.locIdxStart, pt.locIdxEnd, zoneToParse, mvr);
				pSimpleRuleValidities->push_back(tmpRuleValidity);
				pAdditionalSimpleRuleValidities->push_back(tmpRuleValidity);
				atLeastOneFound = true;
			}
		}
	}
	if (pLocsWithinRule != nullptr) delete pLocsWithinRule;
	return atLeastOneFound;
}
bool fpClassifier::compoundFromSimpleRules(std::vector<point3dMm> *pLocations, vector<ruleValidity* > *pSimpleRuleValidities) {
	bool atLeastOneFound = false;;
	for each (movementRule *mvr in allCompoundMovementRules) {
		if (mvr->testCompoundRuleValidity(pLocations, pSimpleRuleValidities)) {
		}
	}
	return atLeastOneFound;
}

/*
classification {unclassified, transition, reject, accept}

inzone [ininner] hoverzone hoverInner inZone unclass
transition - inzone unclass
transition - inzone ininner unclass
reject - inzone hoverzone inZone [hoverzone] [inZone] unclass
reject - inzone hoverzone inZone [hoverzone] [inZone] ininner  unclass
accept - inzone hoverzone inZone [hoverzone] [inZone] ininner hoverInner unclass

classification begins at entry, and ends with exit, at least this will be a transition
hovering will elevate potential to reject
hoverInner will elevate to accept
*/
void fpClassifier::parse(std::vector<point3dMm> *pLocations) {
	parse(pLocations, mZone3dCompounds);
	return;

	int classificationBeginIdx = 0;
	int classificationEnterIdx = -1;
	int classificationEnterInnerIdx = -1;
	int classificationExitInnerIdx = -1;
	bool fed = false;
	bool departedTarget = false;
	int zIdx = -1;
	classification currClass = classification::unclassified;
	classification newClass = classification::unclassified;
	clsfctnEvent locEvent;
	clsfctnEvent moveEvent;
	locationState newState = locationState::idleLocation;
	locationState currentLocState = locationState::idleLocation;
	motionState currentMoveState = motionState::idleMotion;
	motionState newMoveState = motionState::idleMotion;
	std::vector<clsfctnRecord *> clsfctnRecords;
	umZone *zoneEntered = NULL;
	umZone *zoneAmWithin = NULL;
	for (int idx = 0; idx < pLocations->size(); idx++) {
		bool noteChangeOfState = false;
		newState = currentLocState;
		newMoveState = currentMoveState;
		locEvent = enterOrLeaveZone(currentLocState, &(pLocations->at(idx)), true, &zoneEntered);
		moveEvent = checkSpeedChange(currentMoveState, &(pLocations->at(idx)));

		if (locEvent == clsfctnEvent::goaway) {
			// moving into the distance can happen from any state
			newState = locationState::away;
		}
		else if (locEvent == clsfctnEvent::comeclose) {
			newState = locationState::idleLocation;

		} else {
			switch (currentLocState) {
			case locationState::idleLocation:
				newState = (locEvent == clsfctnEvent::enterZone) ? locationState::withinZone : currentLocState;
				break;
			case locationState::withinZone:
				newState = (locEvent == clsfctnEvent::leaveZone) ? locationState::idleLocation : ((locEvent == clsfctnEvent::enterInner) ? locationState::withinInner : currentLocState);
				break;
			case locationState::withinInner:
				newState = (locEvent == clsfctnEvent::leaveInner) ? locationState::withinZone : ((locEvent == clsfctnEvent::leaveZone) ? locationState::idleLocation : currentLocState);
				break;
			case locationState::away:
				newState = (locEvent == clsfctnEvent::comeclose) ? locationState::idleLocation : currentLocState;
				break;
			}
		}
		if (moveEvent != clsfctnEvent::none) {
			switch (currentMoveState) {
			case motionState::idleMotion:
				newMoveState = (moveEvent == clsfctnEvent::startHover) ? motionState::hovering : ((moveEvent == clsfctnEvent::startWhizz) ? motionState::whizzing : motionState::normalMovement);
				break;
			case motionState::hovering:
				newMoveState = (moveEvent == clsfctnEvent::endHover) ? motionState::normalMovement : currentMoveState;
				break;
			case motionState::normalMovement:
				newMoveState = (moveEvent == clsfctnEvent::startHover) ? motionState::hovering : ((moveEvent == clsfctnEvent::startWhizz) ? motionState::whizzing : motionState::normalMovement);
				break;
			case motionState::whizzing:
				newMoveState = (moveEvent == clsfctnEvent::endWhizz) ? motionState::normalMovement : motionState::whizzing;
				break;
			}
		}
		if (locEvent != clsfctnEvent::none) {
			// some change, lookahead to check the change itsnt transient
			if (locationLookAheadNotRevert(pLocations, idx, currentLocState, zoneEntered)) {
				noteChangeOfState = true;
			}
		}
		if (moveEvent != clsfctnEvent::none) {
			// some change, lookahead to check the change itsnt transient
			if (speedLookAheadNotRevert(pLocations, idx, currentMoveState)) {
				noteChangeOfState = true;
			}
			else {
			}
		}
		if (idx >= (pLocations->size() - 1)) {
			// at the end of the list, so wrap up any ending class
			noteChangeOfState = true;
		}
		if (noteChangeOfState) {
			bool classChange = false;
			switch (currentLocState) {
			case locationState::idleLocation: {
				switch (newState) {
				case locationState::idleLocation:
					//currentClass = classification::unclassified;
					break;
				case locationState::withinZone:
					classChange = true; // was outside, and have now entered a zone, so start a new classification process
					newClass = classification::reject; // at least this will become a reject, but feeding can elevate it
					if (classificationEnterIdx == -1) classificationEnterIdx = idx;
					break;
				case locationState::withinInner:
					classChange = true; // was outside, and have now entered a zone, so start a new classification process
					newClass = classification::reject; // at least this will become a reject, but feeding can elevate it
					if (classificationEnterInnerIdx == -1) classificationEnterInnerIdx = idx;
					break;
				case locationState::away:
					classChange = true; // was somewhere, and have now backed off, so start a new classification process
					newClass = classification::distant;
				}
			}
											  break;
			case locationState::withinZone: {
				//if (newMoveState == motionState::hovering || currentMoveState == motionState::hovering) {
				//	if (currentClass == classification::transition) {
				//		currentClass = classification::reject;

				//	}
				//}
				switch (newState) {
				case locationState::idleLocation:
					classChange = true; // whatever classification has been accumulated is now complete
					departedTarget = true;
					newClass = classification::unclassified;
					break;
				case locationState::withinZone:
					if (classificationEnterIdx == -1) classificationEnterIdx = idx;
					break;
				case locationState::withinInner:
					if (classificationEnterInnerIdx == -1) classificationEnterInnerIdx = idx;
					break;
				case locationState::away:
					classChange = true; // whatever classification has been accumulated is now complete
					departedTarget = true;
					newClass = classification::distant;
				}
			}

											break;
			case locationState::withinInner: 
			{
				if (newMoveState == motionState::hovering || currentMoveState == motionState::hovering) {
					fed = true;;
				}
				switch (newState) {
				case locationState::idleLocation:
					classChange = true; // whatever classification has been accumulated is now complete
					departedTarget = true;
					if (classificationExitInnerIdx == -1) classificationExitInnerIdx = idx;
					newClass = classification::unclassified;
					break;
				case locationState::withinZone:
					if (classificationExitInnerIdx == -1) classificationExitInnerIdx = idx;
					break;
				case locationState::withinInner:
					break;
				case locationState::away:
					classChange = true; // whatever classification has been accumulated is now complete
					departedTarget = true;
					if (classificationExitInnerIdx == -1) classificationExitInnerIdx = idx;
					newClass = classification::distant;
				}
				break;

			}
			case locationState::away:
			{
				switch (newState) {
				case locationState::idleLocation:
					classChange = true; // was somewhere, and have now backed off, so start a new classification process
					newClass = classification::unclassified;
					break;
				case locationState::withinZone:
					classChange = true; // was outside, and have now entered a zone, so start a new classification process
					newClass = classification::reject; // at least this will become a reject, but feeding can elevate it
					if (classificationEnterIdx == -1) classificationEnterIdx = idx;
					break;
				case locationState::withinInner:
					classChange = true; // was outside, and have now entered a zone, so start a new classification process
					newClass = classification::reject; // at least this will become a reject, but feeding can elevate it
					if (classificationEnterInnerIdx == -1) classificationEnterInnerIdx = idx;
					break;
				case locationState::away:
					break;
				}
			}
			}
			if (idx >= (pLocations->size() - 1)) {
				// at the end of the list, so wrap up any ending class
				classChange = true; // was somewhere, and have now backed off, so start a new classification process
				newClass = classification::unclassified;
			}
			currentLocState = newState;
			currentMoveState = newMoveState;
			if (classChange) {
				bool doneSplit = false;
				if (departedTarget && currClass == classification::reject) { // covers any passage over the target, now we discriminate depending on what was done
					if (fed && (timePeriod(pLocations, classificationEnterInnerIdx, classificationExitInnerIdx) > mFeedPeriod)) {
						if (classificationEnterInnerIdx != -1 && classificationExitInnerIdx != -1) {
							for (int classIdx = classificationBeginIdx; classIdx < classificationEnterInnerIdx; classIdx++) {
								pLocations->at(classIdx).classification = classification::scanBefore;
							}
							for (int classIdx = classificationEnterInnerIdx; classIdx < classificationExitInnerIdx; classIdx++) {
								pLocations->at(classIdx).classification = classification::feed;
							}
							for (int classIdx = classificationExitInnerIdx; classIdx < idx; classIdx++) {
								pLocations->at(classIdx).classification = classification::scanAfter;
							}
							doneSplit = true;
						}
					}

				}
				if (!doneSplit) {
					for (int classIdx = classificationBeginIdx; classIdx < idx; classIdx++) {
						pLocations->at(classIdx).classification = currClass;
					}
				}
				classChange = false;
				classificationBeginIdx = idx;
				classificationEnterIdx = -1;
				classificationEnterInnerIdx = -1;
				classificationExitInnerIdx = -1;
				fed = false;
				departedTarget = false;
				currClass = newClass;
			}
		}
	}

}
clsfctnEntryOrExit::clsfctnEntryOrExit(bool isEntry, float angle, float eventTime, int zoneID, int zoneUserType, string ruleName) {
	mIsEntry = isEntry;
	mAngle = angle;
	mEventTime = eventTime;
	mZoneID = zoneID;
	mRuleName = ruleName;
	mZoneUserType = zoneUserType;
}
clsfctnTotaliser::clsfctnTotaliser() {
	mCurrentState = false;
	mStartTime = mEndTime = 0;
	mEventJustEnded = false;
	mLastUpdateIdx = 0;
	mEventSpeed = 0;
}
void clsfctnTotaliser::addEvent(int idx, float evtSpeed, float evtTime, float prevTime, bool active) {
	if (mCurrentState) {
		if (!active) {
			// 1st check if this NotActive is not the call-all 
			if (idx > mLastUpdateIdx) {
				//mEndTime = evtTime;
				mEventTime = mEndTime - mStartTime;
				mEventTimes.push_back(mEventTime);
				mEventSpeeds.push_back(mEventSpeed / mEventTime);
				mEventSpeed = 0;
				mCurrentState = false;
				mEventJustEnded = true;
			}
		}
		else {
			mEventSpeed += evtSpeed * (evtTime - mEndTime);
			mLastUpdateIdx = idx;
			mEndTime = evtTime; // ongoing event, so just accumulate end time
			mEventJustEnded = false;
		}
	}
	else {
		if (active) {
			mLastUpdateIdx = idx;
			mEndTime = evtTime;
			mStartTime = prevTime;
			mEventSpeed += evtSpeed * (evtTime - mStartTime);
			mCurrentState = true;
			mEventJustEnded = false;
		}
		else {
			mEventJustEnded = false;
			mEventSpeed = 0;
		}

	}
}
float clsfctnTotaliser::getCummulativeTime() {
	float retv = 0;
	if (mCurrentState)
		retv = mEndTime - mStartTime;
	return retv;
}
float clsfctnTotaliser::getEventTime() {
	if (mEventJustEnded) {
//		mEventJustEnded = false;
		return mEventTime;
	}
	else {
		return 0;
	}
}
float clsfctnTotaliser::getEventSpeed() {
	float retv = 0;
	if (mCurrentState || mEventJustEnded)
		retv = mEventSpeed / (mEndTime - mStartTime);
	return retv;
}

int clsfctnTotaliser::getCount() {
	return mEventTimes.size();
}
float clsfctnTotaliser::getTotalTime() {
	float retv = 0;
	for each(float tm in mEventTimes) {
		retv += tm;
	}
	return retv;
}
float clsfctnTotaliser::getAverageTime() {
	float retv = 0;
	if (mEventTimes.size() > 0) {
		retv = getTotalTime() / mEventTimes.size();
	}
	return retv;
}
float clsfctnTotaliser::getVarriance() {
	float retv = 0;
	float avg = getAverageTime();
	if (mEventTimes.size() > 0) {
		for each(float tm in mEventTimes) {
			retv += (tm - avg) * (tm - avg);
		}
		if (mEventTimes.size() > 1) {
			retv /= (mEventTimes.size() - 1);
		}
	}
	return retv;

}
float clsfctnTotaliser::getTotalSpeed() {
	float retv = 0;
	for each(float tm in mEventSpeeds) {
		retv += tm;
	}
	return retv;
}
float clsfctnTotaliser::getAverageSpeed() {
	float retv = 0;
	if (mEventSpeeds.size() > 0) {
		retv = getTotalSpeed() / mEventSpeeds.size();
	}
	return retv;
}
float clsfctnTotaliser::getSpeedVarriance() {
	float retv = 0;
	float avg = getAverageSpeed();
	if (mEventSpeeds.size() > 0) {
		for each(float tm in mEventSpeeds) {
			retv += (tm - avg) * (tm - avg);
		}
		if (mEventSpeeds.size() > 1) {
			retv /= (mEventSpeeds.size() - 1);
		}
	}
	return retv;

}


