#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "csvFile.h"
#include "CVcommon.h"

#include "zone3d.h"
#include "zonesFile.h"

using namespace System;

zonesFile::zonesFile() {
}
zonesFile::~zonesFile() {

}
umZone *zonesFile::parseLineZone(std::vector<std::string>   lineArray) {
	//		locFile << zn->mName << "," << zn->mCentreX << "," << zn->mCentreY << "," << zn->mRadiusInclude << "," << zn->mRadiusExclude << "," << (zn->mIsCircle) ? 1 : 0 << "\n";
	umZone *retv = NULL;
	if (lineArray.size() == 6) {
		retv = new umZone(std::stof(lineArray[1]),std::stof(lineArray[2]),std::stoi(lineArray[3]), std::stoi(lineArray[4]), (char *)(lineArray[0].c_str()));
		retv->mIsCircle = (std::stoi(lineArray[5]) == 1);
	}
	return retv;
}
bool zonesFile::parseLinePerspective(std::vector<std::string>   lineArray, bool *usePerspective, int *pTLX, int *pTLY, int *pTRX, int *pTRY, int *pBLX, int *pBLY, int *pBRX, int *pBRY) {
	//			locFile << usePInt << "," << TLX << "," << TLY << "," << TRX << "," << TRY << "," << BLX << "," << BLY << "," << BRX << "," << BRY;
	bool retv = false;
	*usePerspective = false;
	if (lineArray.size() == 9) {
		*usePerspective = (std::stoi(lineArray[0]) == 1);
		*pTLX = std::stoi(lineArray[1]);
		*pTLY = std::stoi(lineArray[2]);
		*pTRX = std::stoi(lineArray[3]);
		*pTRY = std::stoi(lineArray[4]);
		*pBLX = std::stoi(lineArray[5]);
		*pBLY = std::stoi(lineArray[6]);
		*pBRX = std::stoi(lineArray[7]);
		*pBRY = std::stoi(lineArray[8]);

		retv = true;
	}
	return retv;
}

int zonesFile::read(char *filepathname, std::vector<umZone *> *pZones, bool *usePerspective, int *pTLX, int *pTLY, int *pTRX, int *pTRY, int *pBLX, int *pBLY, int *pBRX, int *pBRY) {
	std::ifstream str;
	str.open(filepathname);
	for (;;) {
		std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		umZone *tmprec = parseLineZone(lineArray);
		if (tmprec != NULL) {
			pZones->push_back(tmprec);
		}
		else {
			parseLinePerspective(lineArray, usePerspective, pTLX, pTLY, pTRX, pTRY, pBLX, pBLY, pBRX, pBRY);
			break;
		}
	}
	str.close();
	return pZones->size();
}

void zonesFile::write(char *filepathname, std::vector<umZone *> *pZones, bool usePerspective, int TLX, int TLY, int TRX, int TRY, int BLX, int BLY, int BRX, int BRY) {
	ofstream locFile(filepathname, ios::out);
	int znIdx = 0;
	for each (umZone *zn in *pZones)
	{
		locFile << zn->mName << "," << zn->mCentreX << "," << zn->mCentreY << "," << zn->mRadiusOuter << "," << zn->mRadiusInner << "," << ((zn->mIsCircle) ? 1 : 0) << "\n";
		znIdx++;
	}
	int usePInt = usePerspective ? 1 : 0;
	locFile << usePInt << "," << TLX << "," << TLY << "," << TRX << "," << TRY << "," << BLX << "," << BLY << "," << BRX << "," << BRY;
	locFile.close();

}
int zonesFile::read(char *filepathname, std::vector<zone3dCompound *> *pZones, vector<string> *userDefinedZoneTypes) {
	std::ifstream str;
	str.open(filepathname);
	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	int userDefinedTypeCount = csvFile::parseLine1iSafely(lineArray[0], 0);
	userDefinedZoneTypes->clear();
	userDefinedZoneTypes->push_back(string("Default"));
	for (int idx = 0; idx < userDefinedTypeCount; idx++) {
		userDefinedZoneTypes->push_back(lineArray[idx + 1]);
	}
	for (;;) {
		lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		zone3dCompound *tmprec = new zone3dCompound();
		int subzoneCnt = tmprec->deSerialise(0, lineArray);
		if (subzoneCnt >= 0) {
			while (subzoneCnt > 0) {
				lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
				subzoneCnt = tmprec->deSerialise(subzoneCnt , lineArray);
			}
			pZones->push_back(tmprec);
		}
		else {
			break;
		}
	}
	str.close();
	return pZones->size();
}
void zonesFile::write(char *filepathname, std::vector<zone3dCompound *> *pZones, vector<string> *userDefinedZoneTypes) {
	ofstream locFile(filepathname, ios::out);
	int znIdx = 0;
	locFile << userDefinedZoneTypes->size() - 1;
	for (int idx = 1; idx < userDefinedZoneTypes->size(); idx++) {
		locFile << "," << userDefinedZoneTypes->at(idx);
	}
	locFile << "\n";
	for each (zone3dCompound *zn in *pZones)
	{
		string *oneZone = zn->serialise();
		locFile << *oneZone;
		delete oneZone;
		znIdx++;
	}
	locFile.close();

}
