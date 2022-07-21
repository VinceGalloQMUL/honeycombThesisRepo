#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "csvFile.h"
#include "CVcommon.h"

#include "beeTagLoc.h"
#include "beeTagLocFile.h"

using namespace System;


beeTagLocFile::beeTagLocFile() {
}
beeTagLocFile::~beeTagLocFile() {

}
beeTagLoc *beeTagLocFile::parseLine(std::vector<std::string>   lineArray) {
	beeTagLoc *retv = new beeTagLoc();
	if (!retv->parse(lineArray)) {
		delete retv;
		retv = nullptr;
	}
	return retv;
}

int beeTagLocFile::read(char *filepathname, std::vector<beeTagLoc *> *pZones) {
	std::ifstream str;
	str.open(filepathname);
	for (;;) {
		std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		beeTagLoc *tmprec = parseLine(lineArray);
		if (tmprec != NULL) {
			pZones->push_back(tmprec);
		}
	}
	str.close();
	return (int)pZones->size();
}

void beeTagLocFile::write(char *filepathname, std::vector<beeTagLoc *> *pTags) {
	ofstream locFile(filepathname, ios::out);
	int znIdx = 0;
	for each (beeTagLoc *tg in *pTags)
	{
		locFile << tg->toStr() << "\n";
		znIdx++;
	}
	locFile.close();

}
void beeTagLocFile::append(char *filepathname, std::vector<beeTagLoc *> *pTags) {
	ofstream locFile(filepathname, ios::out | ios::app);
	int znIdx = 0;
	for each (beeTagLoc *tg in *pTags)
	{
		locFile << tg->toStr() << "\n";
		znIdx++;
	}
	locFile.close();

}
